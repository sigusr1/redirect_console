#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>

#include "redirect.h"

void send_msg(SHELL_CMD *cmd)
{
    int msg_id;
    key_t msg_key=ftok(MSG_PATH, 1);

    msg_id = msgget(msg_key, IPC_CREAT | 0666);
    if (-1 == msgsnd(msg_id, cmd, sizeof(SHELL_CMD), 0))
    { 
        COMMON_LOG("Send msg fail. error:%s", strerror(errno));
        return;
    }

    return;
}

int main(int argc, char **argv)
{
    int i;
    SHELL_CMD cmd_info;

    memset(&cmd_info, 0, sizeof(cmd_info));

    if (argc < 2)
    {
        COMMON_LOG("Please use format: %s <cmd> [param]", argv[0]);
        return 0;
    }

    strncpy(cmd_info.cmd, argv[1], sizeof(cmd_info.cmd) - 1);

    for(i = 2; i < argc; i++)
    {
        snprintf(cmd_info.param + strlen(cmd_info.param), sizeof(cmd_info.param) - strlen(cmd_info.param) - 1, " %s", argv[i]);
    }

    if (strcmp(argv[1], "redirect_print") == 0)
    {
        strcpy(cmd_info.param, ttyname(STDOUT_FILENO)); 
    }
    else if (strcmp(argv[1], "redirect_input") == 0)
    {
        strcpy(cmd_info.param, ttyname(STDIN_FILENO)); 
    }
    
    send_msg(&cmd_info);
}
