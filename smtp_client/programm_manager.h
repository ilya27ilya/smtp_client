//
//  programm_manager.h
//  smtp_client
//
//  Created by mam on 11.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#ifndef programm_manager_h
#define programm_manager_h

#include "child_proc.h"
#include "child_struct.h"
#include "input_struct.h"
#include "logging.h"
#include "maildir.h"
#include "main_proc.h"
#include "read_message.h"
#include "smtp.h"
#include "smtp_fsm.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <signal.h>

#define MAX_COUNT_DOMAIN 10
#define MAX_DOMAIN_LIST_LEN 1024

int *pids;
int proc_num;
extern int exitcode;

int init_manager(input_struct input_data);
int run_main_loop(input_struct input_data, int *proc);
char *itoa(int val, int base);
void free_child_info_array(child_info *child_array, long proc_number);
int find_proc(child_info *child_array, char *domain, long proc_number);

int initSignalHandler();

#endif /* programm_manager_h */
