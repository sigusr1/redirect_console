
#ifndef __REDIRECT_CONSOLE_H__
#define __REDIRECT_CONSOLE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MSG_PATH "/var/tmp/console_msg"
#define COMMON_LOG(fmt, ...)    printf("%s:%d: " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__)

typedef struct
{
    char cmd[32];
    char param[64];
}SHELL_CMD;

void init_redirect_module(void);

#ifdef __cplusplus
}
#endif

#endif

