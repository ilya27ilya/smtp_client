//
//  main_proc.c
//  smtp_client
//
//  Created by mam on 15.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#include "main_proc.h"

int send_message_to_proc(char* queue_name, char* file_name, char* domain){
    char buffer[MAX_LOG_MES_SIZE];
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, file_name);
    strcat(buffer, " ");
    strcat(buffer, domain);
    
    static mqd_t mq = -1;
    static int log_stop = 0;
    
    if (mq == -1)
        mq = mq_open(queue_name, O_WRONLY);
    
    if (!log_stop && mq) {
        write_log(INFO_LOG, "Send info [%s] to queue: %s",buffer,queue_name);
        int a =  mq_send(mq, buffer, MAX_LOG_MES_SIZE, 0);
        if(a != 0)
            write_log(ERROR_LOG, "In send info [%s] to queue: %s, a = %d",buffer,queue_name,a);
        
        
        if (!strcmp(LOG_MSG_STOP, file_name)) {
            int wait_status = 0;
            wait(&wait_status);
            log_stop = 1;
            mq_close(mq);
        }
    }
    
    
    return 0;
}
