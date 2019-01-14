//
//  programm_manager.c
//  smtp_client
//
//  Created by mam on 11.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "programm_manager.h"

void init_manager(char* new_dir_name, char* cur_dir_name, char* log_filename, int attempt_delay, int attempt_number, int max_proc_number){
    send_log_message(INFO_LOG, "I");
    Run(attempt_number, attempt_delay);
}



int Run(int attempts_number, int attempts_delay){
    
    usleep(100);
    send_log_message(INFO_LOG, "am");
    while(1){
        usleep(100);
        send_log_message(INFO_LOG, "log");
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
