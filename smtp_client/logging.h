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
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>


#define LOG_QUEUE_NAME "/logging_queue"
#define LOG_SLEEP_MCS (100)
#define MAX_LOG_MES_SIZE 5000
#define LOG_MSG_STOP "LOGGING STOP"
#define MAX_MSG_NUM 10
#define ERROR_LOG "[ERROR] "
#define INFO_LOG "[INFO] "
#define LOG_MSG_STOP "LOGGING STOP"
#define LOG_NO_TAG ""
#define LOG_TIME_SIZE 22



int logging_loop(char *log_name);
int save_log_mes(char *message, char *log_name);
void write_log(char *tag, char *message, ...);
void log_stop();


#endif /* logging_h */
