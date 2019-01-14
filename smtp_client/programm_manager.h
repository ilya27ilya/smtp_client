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



#define MAX_COUNT_DOMAIN 10


struct mail_process_dscrptr
{
    pid_t pid;         
    int queue_id;
    int domains_count;
    int mails_count;
    char *domains[MAX_COUNT_DOMAIN];
};


int init_manager(input_struct input_data);
int run_main_loop(long attempts_number, long attempts_delay);
void process_fork(int count);


#endif /* programm_manager_h */
