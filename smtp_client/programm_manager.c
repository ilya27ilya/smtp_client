//
//  programm_manager.c
//  smtp_client
//
//  Created by mam on 11.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "programm_manager.h"

int init_manager(input_struct input_data){
    
    int result = 0;
    long count = input_data.max_proc_number;
    int pids[count];
    int parent = 1;
    int k;
    write_log(INFO_LOG, "Create child proc");
    for (int i = 0; i < count; i++) {
        k = fork();
        if (k == 0) {
            result = child_loop(i);//если передавать i то он будет для этого процесса и в менеджере
            parent = 0;
            break;
        }
        else{
            pids[i] = k;
        }
    }
    if(parent == 1){
        result = run_main_loop(input_data,pids);
    }
    return result;
}



int run_main_loop(input_struct input_data,int* proc){
    
    long proc_number = input_data.max_proc_number;
    child_info child_array[proc_number];
    

    for (int i = 0; i< proc_number; i++) {
        child_array[i].connection = 0;
        child_array[i].messenge_number = 0;
        child_array[i].pid = proc[i];
        child_array[i].domain = malloc(sizeof(char)*MAX_DOMAIN_LIST_LEN);
        strcpy(child_array[i].domain,"@");
        child_array[i].queue_name = malloc(sizeof(char)*7);
        memset(child_array[i].queue_name, 0, 7);
        strcpy(child_array[i].queue_name, "/");
        strcat(child_array[i].queue_name, itoa(proc[i], 10));
    }
    
    for (int i = 0; i < proc_number; i++)
        write_log(INFO_LOG, "Child proc: %d %d %d queue_name %s dom %s",child_array[i].pid,child_array[i].messenge_number, child_array[i].connection, child_array[i].queue_name, child_array[i].domain);
    
    //struct file_list *mes_queue = NULL;
    
//    if(create_connection(child_array, proc_number)){
//        write_log(ERROR_LOG, "Error in creation of connection");
//        return -1;
//    }
//    else
//        write_log(INFO_LOG, "Con btw proc establish");
    
    //usleep(100);
    while (1) {
        send_message_to_proc(child_array[1].queue_name, "22", "33");
//    mes_queue = get_name(input_data.new_dir, input_data.cur_dir);
//
//        struct file_list *next = mes_queue;
//        if(mes_queue != NULL){
//            while (next != NULL) {
//                next = next->next;
//                write_log(INFO_LOG, "New message %s  -  %s",mes_queue->file_name,mes_queue->domain);
//
//                mes_queue = next;
//            }
//        }
//        else{
//            printf("there is now file in new_dir \n");
//        }
        sleep(2);
    }
    
    free_child_info_array(child_array, proc_number);
    return 0;
    
}

char* itoa(int val, int base){
    if(val == 0){
        return "0";
    }
    static char buf[32] = {0};
    int i = 30;
    for(; val && i ; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];
    return &buf[i+1];
}

void free_child_info_array(child_info *child_array, long proc_number){
    for(int i = 0; i<proc_number; i++){
        free(child_array[i].queue_name);
        free(child_array[i].domain);
    }
}

int create_connection(child_info* child_array, long proc_number){
    int res = 0;
    
    for (int i = 0; i<proc_number; i++) {
        struct mq_attr attr;
        memset(&attr, 0, sizeof(struct mq_attr));
        attr.mq_flags = 0;
        attr.mq_maxmsg = MAX_MSG_NUM;
        attr.mq_msgsize = MAX_LOG_MES_SIZE;
        attr.mq_curmsgs = 0;
        mqd_t mq = mq_open(child_array[i].queue_name, O_CREAT | O_RDWR | O_NONBLOCK, 0666, &attr);
        
        if (mq == -1) {
            res = 1;
            break;
        }
        write_log(INFO_LOG, "Create Mq %d with name [%s]", mq,child_array[i].queue_name);
    }
    return res;
}
