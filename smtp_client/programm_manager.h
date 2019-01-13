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


#define MAX_COUNT_DOMAIN 10


struct mail_process_dscrptr
{
    pid_t pid;         
    int queue_id;
    int domains_count;
    int mails_count;
    char *domains[MAX_COUNT_DOMAIN];
};


void init_manager(char* new_dir_name, char* cur_dir_name, char* log_filename, int attempt_delay, int attempt_number, int max_proc_number);
int Run(int attempts_number, int attempts_delay);
void process_fork(int count);


#endif /* programm_manager_h */
