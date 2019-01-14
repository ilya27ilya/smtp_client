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
#define ERROR_LOG "[ERROR] "
#define INFO_LOG "[INFO] "


typedef struct queue_msg
{
    long mtype;
    char mtext[MAX_LOG_MES_SIZE];
} queue_msg;

int start_logger(char *log_filename_base);
int stop_logger(void);
int send_log_message(char* log, char *message);
int save_log(char *message, char *logs_dir);







#endif /* logging_h */
