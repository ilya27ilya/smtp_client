//
//  run.c
//  smtp_client
//
//  Created by mam on 14.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#include "run.h"


int run(input_struct input_data){
    int result = 0;
    if (fork() == 0){
        result = logging_loop(input_data.logger_name);
    }
    else{
        result = init_manager(input_data);
    }
    //printf("%s",input_data.logger_name);
    return result;
}


