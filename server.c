#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

#include "redirect.h"

#define PROC_FUNC_NUM (sizeof(msg_proc_list) / sizeof(msg_proc_list[0]))

int fd_out_bak = -1;
int fd_in_bak  = -1;

typedef void (*MSG_PROC_FUNC) (const char *param);

typedef struct
{
    const char *cmd;
    MSG_PROC_FUNC msg_func;
}MSG_PROC;

void redirect_print(const char *param)
{
    int fd_out = open(param, O_WRONLY);
    if (fd_out < 0)
    {
        COMMON_LOG("Fail to open[%s].error:%s", param, strerror(errno));
        return;
    }
    
    dup2(fd_out, STDOUT_FILENO);
    close(fd_out);
    return;
} 

void restore_print(const char *param)
{
    dup2(fd_out_bak, STDOUT_FILENO);
    return;
} 

void redirect_input(const char *param)
{
    int fd_in = open(param, O_RDONLY);

    if (fd_in < 0)
    {
        COMMON_LOG("Fail to open[%s].error:%s", param, strerror(errno));
        return;
    }
    
    dup2(fd_in,STDIN_FILENO);
    close(fd_in);
    return;
} 

void restore_input(const char *param)
{
    dup2(fd_in_bak,STDIN_FILENO);
    return;
} 

void help(const char *param);

static MSG_PROC msg_proc_list[] =
{
    {"help", help},
    {"redirect_print", redirect_print},
    {"restore_print", restore_print},
    {"redirect_input", redirect_input},
    {"restore_input", restore_input},
};

void help(const char *param)
{
    int index;

    COMMON_LOG("%s", "support cmd:");
    for (index = 0; index < PROC_FUNC_NUM; index++)
    {
        COMMON_LOG("%s", msg_proc_list[index].cmd);
    }

    return;
}

void* shell_server(void* ptr)
{
    int index;
    int ret;
    int msg_id = -1;
    key_t msg_key = -1;

    SHELL_CMD cmd_info;

    char cmd[128];
    
    //备份原始重定向描述符，以备还原
    fd_out_bak = dup(STDOUT_FILENO);
    fd_in_bak = dup(STDIN_FILENO);

    if(access(MSG_PATH, F_OK)==-1)
    {
        creat(MSG_PATH, S_IRWXU | S_IRWXG | S_IRWXO);
    }

    while(1)
    {
        msg_key = ftok(MSG_PATH, 1);
        if (msg_key == -1)
        {
            COMMON_LOG("ftok fail. err:%s", strerror(errno));
            continue;
        }
        
        msg_id = msgget(msg_key, IPC_CREAT | 0666);
        if (msg_id == -1)
        {
            COMMON_LOG("msgget fail. err:%s", strerror(errno));
            continue;
        }

        memset(&cmd_info, 0, sizeof(cmd_info));
        ret = msgrcv(msg_id, &cmd_info, sizeof(cmd_info), 0, MSG_NOERROR);
        if (ret == -1)
        {
            COMMON_LOG("msgrcv fail. err:%s", strerror(errno));
            continue;
        }

        for (index = 0; index < PROC_FUNC_NUM; index++)
        {
            if (strcmp(msg_proc_list[index].cmd, cmd_info.cmd) == 0 && msg_proc_list[index].msg_func != NULL)
            {
                msg_proc_list[index].msg_func(cmd_info.param);
                break;
            }
        }

        /* 交给系统来处理*/        
        if (index == PROC_FUNC_NUM)
        {
            memset(cmd, 0, sizeof(cmd));
            snprintf(cmd, sizeof(cmd) - 1, "%s%s", cmd_info.cmd, cmd_info.param);
            system(cmd);
        }
    }

    return NULL;
}

void init_redirect_module()
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, shell_server, NULL);
    return;
}
