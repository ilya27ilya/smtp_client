//
//  programm_manager.c
//  smtp_client
//
//  Created by mam on 11.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "programm_manager.h"

int init_manager(input_struct input_data){
    int result;
    write_log(INFO_LOG, "START SMTP SERVER");
    result = run_main_loop(input_data.attempt_number, input_data.attempt_number);
    return result;
}



int run_main_loop(long attempts_number, long attempts_delay){
    
    

    while(1){
        //sleep(2);
        write_log(ERROR_LOG, "HELLO");
        write_log(ERROR_LOG, "its");
        write_log(ERROR_LOG, "work");
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
