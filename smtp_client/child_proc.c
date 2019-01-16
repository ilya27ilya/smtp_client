//
//  child_proc.c
//  smtp_client
//
//  Created by mam on 15.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#include "child_proc.h"

int child_loop(int i, int attempts_number, int attempts_delay){
    write_log(INFO_LOG, "Start child proc %d",i);
    
    child_struct child;
    child.child_id = i;
    child.child_data.pid = getpid();
    child.child_data.connection = 0;
    child.child_data.messenge_number = 0;
    child.child_data.domain = malloc(MAX_DOMAIN_LIST_LEN);
    strcpy(child.child_data.domain,"");
    child.child_data.queue_name = malloc(7);
    memset(child.child_data.queue_name, 0, 7);
    strcpy(child.child_data.queue_name, "/");
    strcat(child.child_data.queue_name, itoa(child.child_data.pid, 10));
    
    
    sock_struct sock_connection[MAX_COUNT_DOMAIN];
    for(int i = 0; i< MAX_COUNT_DOMAIN;i++){
        strcat(sock_connection[i].domain,"");
        sock_connection[i].sock_descr = -1;
        strcat(sock_connection[i].message_list,"");
        sock_connection[i].count_try = 3;
    }
    
    
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
    
    
    fd_set read_fds;
    fd_set write_fds;
    
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int maxfd = 0;
    
    while (1) {
        char buffer[MAX_LOG_MES_SIZE + 1];
        memset(buffer, 0, sizeof(buffer));
        
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_LOG_MES_SIZE, NULL);
        
        struct file_list *mes_queue = NULL;
        mes_queue = (struct file_list *)malloc(sizeof(struct file_list));
        
        //Когда получили сообщение
        if (bytes_read > 0) {
            
            //прочитали сообщение
            printf("[%s]\n",buffer);
            char* istr = NULL;
            istr = strtok(buffer," ");
            strcat(mes_queue->file_name, istr);
            istr = strtok (NULL," ");
            strcat(mes_queue->domain, istr);
            write_log(INFO_LOG, "Get mess adr: [%s] domain: [%s]",mes_queue->file_name, mes_queue->domain);
            
            
            //если в списке доменов нет полученого, то добавили
            if(strstr(child.child_data.domain, mes_queue->domain) == NULL)
                strcpy(child.child_data.domain,mes_queue->domain);

            //определяем какому сокету отправить сообщение
            int res = -1;
            for(int i = 0; i<MAX_COUNT_DOMAIN;i++){
                char* istr = strstr(sock_connection[i].domain,mes_queue->domain);
                if(istr != NULL){
                    res = i;
                    break;
                }
            }
            //если такого подключения нет, то в первый доступный;
            if(res == -1){
                res = child.child_data.connection;
                child.child_data.connection+=1;

                //если уже все заняты, то ошибка и выход
                if(child.child_data.connection ==  MAX_COUNT_DOMAIN){
                    write_log(ERROR_LOG, "There is no empty domain");
                    return -1;
                }
                
                //если соктет не имеет подключения создаем его
                if(sock_connection[res].sock_descr < 0){
                    strcpy(sock_connection[res].domain,mes_queue->domain);
                    create_socket(get_mx(sock_connection[res].domain), PORT, attempts_number, attempts_delay,&sock_connection[res].sock_descr);
                    printf("Child proc sock creation [%d]\n",sock_connection[res].sock_descr);
                }
                
                if(sock_connection[res].sock_descr <= 0){
                    write_log(ERROR_LOG, "Cant create connection to domain: [%s]",sock_connection[res].domain);
                    memset(sock_connection[res].domain, 0,sizeof(sock_connection[res].domain));
                }
                else{
                    printf("Connection with %s establised sock descr is %d", sock_connection[res].domain,sock_connection[res].sock_descr);
                    if(maxfd<sock_connection[res].sock_descr){
                        maxfd = sock_connection[res].sock_descr;
                        sock_connection[res].state = INIT; //INIT
                        FD_SET(sock_connection[res].sock_descr, &write_fds);
                    }
                }
            }//создание подключения
            
            strcat(sock_connection[res].message_list,mes_queue->file_name);
            strcat(sock_connection[res].message_list,"@");
            
            write_log(INFO_LOG, "Add mess to sock [%d]  mess_list: [%s] domain: [%s]",sock_connection[res].sock_descr,sock_connection[res].message_list, sock_connection[res].domain);
            
            memset(mes_queue->domain, 0, sizeof(mes_queue->domain));
            memset(mes_queue->file_name, 0, sizeof(mes_queue->file_name));
            
            mes_queue->next = NULL;
            free(mes_queue);
            
        }//завершили прием сообщения
        
        
//        int act = select(maxfd + 1, &read_fds, &write_fds, NULL, NULL);
//        for (int i = 0; i < MAX_COUNT_DOMAIN; i++)
//        {
//            if(sock_connection[i].sock_descr > 2){
//                if (FD_ISSET(sock_connection[i].sock_descr, &read_fds)){
//                    //socket_work(sock_connection[i],READ,&read_fds, &write_fds,attempt_number);
//                }
//                if (FD_ISSET(sock_connection[i].sock_descr, &write_fds)){
//                    //socket_work(sock_connection[i],WRITE,&read_fds, &write_fds,attempt_number);
//                }
//
//            }
//
//        }
        
        
        
        //sleep(1);
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
            return 0;
        }
    }
    return 1;
}

