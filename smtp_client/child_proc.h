//
//  child_proc.h
//  smtp_client
//
//  Created by mam on 15.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#ifndef child_proc_h
#define child_proc_h

#include <stdio.h>
#include <string.h>
#include "logging.h"
#include "programm_manager.h"
#include "child_struct.h"


int child_loop(int i);
int resv_mes_main(char* queue_name, char* buffer);

struct string_list {
    char file_name[MAX_FILE_NAME_LEN];
    struct string_list * next;
    
};

typedef struct sock_connection{
    int state;
    char domen[MAX_DOMAIN_LEN];
    struct string_list message_list;
} sock_connection;

#endif /* child_proc_h */
