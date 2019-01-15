//
//  child_proc.c
//  smtp_client
//
//  Created by mam on 15.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#include "child_proc.h"

int child_loop(int i){
    child_struct child;
    child.child_id = i;
    child.child_data.pid = getpid();
    child.child_data.connection = 0;
    child.child_data.messenge_number = 0;
    child.child_data.domain = malloc(MAX_DOMAIN_LIST_LEN);
    strcpy(child.child_data.domain,"@");
    child.child_data.queue_name = malloc(6);
    strcpy(child.child_data.queue_name, "/");
    strcat(child.child_data.queue_name, itoa(child.child_data.pid, 10));
    
    fprintf(stdout,"Child %d %d %d %d %s %s \n",child.child_id,child.child_data.pid,child.child_data.connection,child.child_data.messenge_number,child.child_data.domain,child.child_data.queue_name);
    
    char *buffer  = malloc(MAX_LOG_MES_SIZE + 1);
    memset(buffer, 0, MAX_LOG_MES_SIZE + 1);
    
    while (1) {
        resv_mes_main(child.child_data.queue_name, buffer);
        if(buffer != NULL){
            write_log(INFO_LOG, "Chilp proc %d recv mess %s",child.child_data.pid, buffer);
        }
    }
    
    free(buffer);
    free(child.child_data.domain);
    free(child.child_data.queue_name);
    return 0;
}

int resv_mes_main(char* queue_name, char* buffer){
    static mqd_t mq = -1;
    if (mq == -1)
        mq = mq_open(queue_name, O_RDONLY);
    ssize_t bytes_read = mq_receive(mq, buffer, MAX_LOG_MES_SIZE, NULL);
    if (bytes_read > 0) {
        //fprintf(stdout, "%s\n", buffer);
        return 0;
    }
    return 1;
}
