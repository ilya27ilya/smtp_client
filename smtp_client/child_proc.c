//
//  child_proc.c
//  smtp_client
//
//  Created by mam on 15.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#include "child_proc.h"

int child_loop(int i){
    write_log(INFO_LOG, "Start child proc %d",i);
    
    child_struct child;
    child.child_id = i;
    child.child_data.pid = getpid();
    child.child_data.connection = 0;
    child.child_data.messenge_number = 0;
    child.child_data.domain = malloc(MAX_DOMAIN_LIST_LEN);
    strcpy(child.child_data.domain,"@");
    child.child_data.queue_name = malloc(7);
    memset(child.child_data.queue_name, 0, 7);
    strcpy(child.child_data.queue_name, "/");
    strcat(child.child_data.queue_name, itoa(child.child_data.pid, 10));
    
    
    fprintf(stdout,"Child %d %d %d %d %s %s \n",child.child_id,child.child_data.pid,child.child_data.connection,child.child_data.messenge_number,child.child_data.domain,child.child_data.queue_name);
    
    struct mq_attr attr;
    memset(&attr, 0, sizeof(struct mq_attr));
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MSG_NUM;
    attr.mq_msgsize = MAX_LOG_MES_SIZE;
    attr.mq_curmsgs = 0;
    mqd_t mq = mq_open(child.child_data.queue_name, O_CREAT | O_RDONLY | O_NONBLOCK, 0644, &attr);
    
    if (mq == -1) {
        write_log(ERROR_LOG, " Cant Create Mq %d with name [%s]", mq,child.child_data.queue_name);
        return -1;
    }
    write_log(INFO_LOG, "Create Mq %d with name [%s]", mq,child.child_data.queue_name);
    
    
    while (1) {
        char buffer[MAX_LOG_MES_SIZE + 1];
        memset(buffer, 0, sizeof(buffer));
        
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_LOG_MES_SIZE, NULL);
        
        struct file_list *mes_queue = NULL;
        mes_queue = (struct file_list *)malloc(sizeof(struct file_list));
        if (bytes_read > 0) {
            char* istr = NULL;
            istr = strtok(buffer," ");
            strcat(mes_queue->file_name, istr);
            istr = strtok (NULL," ");
            strcat(mes_queue->domain, istr);
            
            write_log(INFO_LOG, "Get mess adr: [%s] domain: [%s] child %d",mes_queue->file_name, mes_queue->domain,child.child_data.pid);
            
            //write_log(INFO_LOG, "Recv mes %s",buffer);
            memset(mes_queue->domain, 0, sizeof(mes_queue->domain));
            memset(mes_queue->file_name, 0, sizeof(mes_queue->file_name));
            mes_queue->next = NULL;
            
        }
        
        free(mes_queue);
        write_log(INFO_LOG, "Main  loop child %d",child.child_data.pid);
        sleep(1);
    }
    

    free(child.child_data.domain);
    free(child.child_data.queue_name);
    return 0;
}

int resv_mes_main(char* queue_name, char* buffer){
    static mqd_t mq = -1;
    
    if (mq == -1){
        mq = mq_open(queue_name, O_RDONLY);
        write_log(INFO_LOG, "Mq %d pid %d", mq,getpid());
    }
    if(mq){
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_LOG_MES_SIZE, NULL);
        if (bytes_read > 0) {
            //fprintf(stdout, "%s\n", buffer);
            return 0;
        }
    }
    return 1;
}

struct file_list* parse_main_mes(char* buffer){
    struct file_list *mes_queue = NULL;
//    char bf[MAX_LOG_MES_SIZE + 1];
//    memset(bf, 0, sizeof(bf));
//    char sep[2] = " ";
//
//    char* istr = strtok (buffer,sep);
//    strcat(bf, istr);
//    write_log(INFO_LOG, "%s",bf);
//    strcat(mes_queue->file_name,bf);
    

    char tmp[BUFFER_SIZE];
    char *pointer;
    char domain[BUFFER_SIZE];
    unsigned long int len;
    pointer = strstr(tmp, "@");
    len = strlen(pointer);
    memset(domain, '\0', len - 2);
    strncpy(domain, pointer + 1, len - 3);
    write_log(INFO_LOG, "%s",domain);
    //strcat(mes_queue->domain,domain);
    
    
    return mes_queue;
}

