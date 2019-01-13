//
//  main.c
//  smtp_client
//
//  Created by mam on 07.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//
/*
 список писем - > письмо -> нагрузка, конврет -> (отправитель, список получателей -> получателей)
 работать с файлами только на клиенте
*/

#include <stdio.h>
#include "message.h"
#include "read_message.h"
#include "message_list.h"
#include "smtp.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <resolv.h>
#include "programm_manager.h"

#include "maildir.h"
#include <string.h>
#include "input_struct.h"
#include "logging.h"

#define ARG_NUM_ERROR -7
#define ARG_NUM 7


int main(int argc, const char * argv[]) {
    
    if(argc != ARG_NUM){
        printf("not enough paramentrs");
        return ARG_NUM_ERROR;
    }
    struct input_struct input_data;
    input_data.new_dir = argv[1];
    input_data.cur_dir = argv[2];
    input_data.logger_name = argv[3];
    input_data.attempt_delay =  atoi(argv[4]);
    input_data.attempt_number = atoi(argv[5]);
    input_data.max_proc_number = atoi(argv[6]);
    
    stop_logger();
    
    if (fork() == 0){
        start_logger(input_data.logger_name);
    }
    else{
        
        init_manager(input_data.new_dir, input_data.cur_dir, input_data.logger_name, input_data.attempt_delay, input_data.attempt_number, input_data.max_proc_number);
    }
    
    //log_i("%s", "Worker for master proc successfully started");
    
//    if (fork() == 0){
//        start_logger(input_data.logger_name);
//        log_i("%s", "Worker for master proc successfully started");
//    }
//    else
//    {
//
//    }
    

    //get_name(input_data.new_dir,input_data.cur_dir);
    
    
    return 0;
}
