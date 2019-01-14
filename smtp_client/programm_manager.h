//
//  programm_manager.h
//  smtp_client
//
//  Created by mam on 11.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#ifndef programm_manager_h
#define programm_manager_h

#include <stdlib.h>
#include <netinet/in.h>
#include "smtp.h"
#include "read_message.h"
#include <sys/select.h>
#include "smtp_fsm.h"
#include "logging.h"
#include "input_struct.h"
#include <stdio.h>
#include "maildir.h"

#define MAX_COUNT_DOMAIN 10
#define MAX_DOMAIN_LIST_LEN 1024


typedef struct child_info{
    int pid;            // Массив id процессов
    char* queue_name;    // Массив названия очередей процессов
    int messenge_number; // Массив массив количества сообщений процессов
    int connection;      // Массив количества подключений процессов
    char* domain;        // Массив строк содержащий домены для процессов
    
}child_info;


int init_manager(input_struct input_data);
int run_main_loop(input_struct input_data,int* proc);
char* itoa(int val, int base);
void free_child_info_array(child_info *child_array, long proc_number);


#endif /* programm_manager_h */
