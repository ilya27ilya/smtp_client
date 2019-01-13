//
//  logging.h
//  smtp_client
//
//  Created by mam on 13.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#ifndef logging_h
#define logging_h


#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#define MAX_LOG_MES_SIZE 1024


typedef struct queue_msg
{
    long mtype;
    char mtext[MAX_LOG_MES_SIZE];
} queue_msg;

typedef enum log_level_type
{
    ERROR,
    INFO,
} log_level;

int start_logger(char *log_filename_base);
int stop_logger(void);
int send_log_message(log_level log_lvl, char *message);
int save_log(char *message, char *logs_dir);

extern log_level cur_lvl;

#define LOG(lvl, format_, ...) {                    \
    int total_size = 1024;                        \
    char *prefix;                            \
    if (lvl <= cur_lvl) {                            \
        switch (lvl) {                            \
            case INFO:                            \
                prefix = "INFO";                        \
                break;                            \
            case ERROR:                            \
                prefix = "ERROR";                        \
                break;                            \
            default:                            \
                abort();                        \
        };                                \
        char *msg = malloc(sizeof(char) * total_size);            \
        snprintf(msg, total_size, "%s "format_"\n", prefix, __VA_ARGS__);\
        send_log_message(lvl, msg);                    \
        free(msg);                            \
    }                                    \
}

#define log_i(format, ...) LOG(INFO, format, __VA_ARGS__);
#define log_e(format, ...) LOG(ERROR, format, __VA_ARGS__);


#endif /* logging_h */
