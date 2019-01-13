//
//  programm_manager.c
//  smtp_client
//
//  Created by mam on 11.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "programm_manager.h"

void init_manager(char* new_dir_name, char* cur_dir_name, char* log_filename, int attempt_delay, int attempt_number, int max_proc_number){
    
    Run(attempt_number, attempt_delay);
    
    
}



int Run(int attempts_number, int attempts_delay){


    while(1){
        //log_i("%s","init_manager function");
        log_e("%s","Worker for master proc successfully started");
        //send_log_message(ERROR, "Worker for master proc successfully started");
        //send_log_message(INFO, "init_manager function");
    }
    return 0;
    
}

void process_fork(int count) {
    for (int i = 0; i < count; i++) {
        if (fork() == 0) {
            break;
        }
    }
}
