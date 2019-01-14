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
#include </usr/local/Cellar/libconfig/1.7.2/include/libconfig.h>

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

void lookup_string(config_t cfg, char *lookup, char *dest, size_t size) {
    const char *tmp_string = NULL;
    config_lookup_string(&cfg, lookup, &tmp_string);
    snprintf(dest, size, "%s", tmp_string);
}

int read_config(char *cfg_filename, input_struct *input_data) {
    config_t cfg;
    config_init(&cfg);
    // config_read_file(&cfg, cfg_filename);
    printf("CONF FILENAME: %s\n", cfg_filename);
    if (!config_read_file(&cfg, cfg_filename)) {
        printf("CONF FILE NOT READ\n");
        config_destroy(&cfg);
        return -1;
    }
    
    //input_struct input_data_tmp;
    memset(input_data, 0, sizeof(input_struct));
    lookup_string(cfg, "new_dir", input_data->new_dir,sizeof(input_data->new_dir));
    lookup_string(cfg, "cur_dir", input_data->cur_dir,sizeof(input_data->new_dir));
    lookup_string(cfg, "logger_name", input_data->logger_name,sizeof(input_data->new_dir));
    config_lookup_int(&cfg, "attempt_number", &input_data->attempt_number);
    config_lookup_int(&cfg, "attempt_delay", &input_data->attempt_delay);
    config_lookup_int(&cfg, "max_proc_number", &input_data->max_proc_number);
    

    config_destroy(&cfg);
    return 0;
    
    
}

int main(int argc, const char * argv[]) {
    
//    if(argc != ARG_NUM){
//        printf("not enough paramentrs\n");
//        return ARG_NUM_ERROR;
//    }
    
    
    input_struct input_data;
    read_config("confl.cfg", &input_data);
    
    printf("%s \n",input_data.new_dir);
    
    
//    
//    input_data.new_dir = argv[1];
//    input_data.cur_dir = argv[2];
//    input_data.logger_name = argv[3];
//    input_data.attempt_delay =  atoi(argv[4]);
//    input_data.attempt_number = atoi(argv[5]);
//    input_data.max_proc_number = atoi(argv[6]);
    
//    if (fork() == 0){
//        start_logger(input_data.logger_name);
//    }
//    else{
//        sleep(1);
//        send_log_message(INFO_LOG, "HI");
//
//        init_manager(input_data.new_dir, input_data.cur_dir, input_data.logger_name, input_data.attempt_delay, input_data.attempt_number, input_data.max_proc_number);
//
//    }
//
    //get_name(input_data.new_dir,input_data.cur_dir);
    
    
    return 0;
}
