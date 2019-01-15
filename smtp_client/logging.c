//
//  logging.c
//  smtp_client
//
//  Created by mam on 13.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#include "logging.h"


int logging_loop(char *log_name){
    
    struct mq_attr attr;
    memset(&attr, 0, sizeof(struct mq_attr));
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MSG_NUM;
    attr.mq_msgsize = MAX_LOG_MES_SIZE;
    attr.mq_curmsgs = 0;
    
    mqd_t mq = mq_open(LOG_QUEUE_NAME, O_CREAT | O_RDONLY | O_NONBLOCK, 0644, &attr);

    if (mq == -1) {
        return -1;
    }
    else {
        int logging_stop = 0;
        while (!logging_stop) {
            char buffer[MAX_LOG_MES_SIZE + 1];
            memset(buffer, 0, sizeof(buffer));
            ssize_t bytes_read = mq_receive(mq, buffer, MAX_LOG_MES_SIZE, NULL);
            
            if (!strcmp(buffer, LOG_MSG_STOP)) {
                logging_stop = 1;
            }
            else if (bytes_read > 0) {
                //fprintf(stdout, "%s\n", buffer);
                save_log_mes(buffer, log_name);
            }
            
            usleep(LOG_SLEEP_MCS);
        }
        mq_close(mq);
        mq_unlink(LOG_QUEUE_NAME);
    }

    return 0;
}

int save_log_mes(char *message, char *log_name){
    
    struct tm *timenow;
    time_t now = time(NULL);
    timenow = gmtime(&now);
    
    char timeString[12];
    strftime(timeString, sizeof(timeString), "%H:%M:%S", timenow);
    
    char *log_path = malloc(strlen(log_name) + 50);
    strcpy(log_path, log_name);
    
    char filename[40];
    strftime(filename, sizeof(filename), "log_%Y-%m-%d", timenow);
    strcat(log_path, filename);
    strcat(log_path, ".log");
    
    FILE *fp = fopen(log_path, "a+");
    if (fp != NULL){
        char *full_log_msg = malloc(strlen(timeString) + 2 + strlen(message));
        strcpy(full_log_msg, timeString);
        strcat(full_log_msg, " ");
        strcat(full_log_msg, message);
        strcat(full_log_msg, "\n");
        fputs(full_log_msg, fp);
        free(full_log_msg);
        fclose(fp);
    }
    else{
        printf("Error in openning of log file");
        return -1;
    }
    
    free(log_path);
    return 0;
    
}

void write_log(char *tag, char *message, ...) {
    static mqd_t mq = -1;
    static int log_stop = 0;
    
    if (mq == -1)
        mq = mq_open(LOG_QUEUE_NAME, O_WRONLY );
    
    if (!log_stop && mq) {
        
        char tmp_message[MAX_LOG_MES_SIZE];
        memset(tmp_message, 0, sizeof(tmp_message));
        va_list args;
        va_start(args, message);
        vsnprintf(tmp_message, sizeof(tmp_message), message, args);
        va_end(args);
        
        char buffer[MAX_LOG_MES_SIZE];
        memset(buffer, 0, sizeof(buffer));
        if (strcmp(LOG_MSG_STOP, message))
            snprintf(buffer, sizeof(buffer), "[%d] %s %s", getpid(), tag, tmp_message);
        else
            snprintf(buffer, sizeof(buffer), "%s", message);
        
        mq_send(mq, buffer, MAX_LOG_MES_SIZE, 0);
        
        if (!strcmp(LOG_MSG_STOP, message)) {
            int wait_status = 0;
            wait(&wait_status);
            log_stop = 1;
            mq_close(mq);
        }
    }
}

void log_stop() {
    write_log(LOG_NO_TAG, LOG_MSG_STOP);
}





