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
            //result = child_loop(i);//если передавать i то он будет для этого процесса и в менеджере
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
        child_array[i].domain = malloc(MAX_DOMAIN_LIST_LEN);
        strcpy(child_array[i].domain,"@");
        child_array[i].queue_name = malloc(6);
        strcpy(child_array[i].queue_name, "/");
        strcat(child_array[i].queue_name, itoa(proc[i], 10));
    }
    
    for (int i = 0; i < proc_number; i++)
        write_log(INFO_LOG, "Child proc: %d %d %d queue_name %s dom %s",child_array[i].pid,child_array[i].messenge_number, child_array[i].connection, child_array[i].queue_name, child_array[i].domain);
    
//    while(1){
//    }
    
    free_child_info_array(child_array, proc_number);
    return 0;
    
}

char* itoa(int val, int base){
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
