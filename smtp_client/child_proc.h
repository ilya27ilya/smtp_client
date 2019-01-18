//
//  child_proc.h
//  smtp_client
//
//  Created by mam on 15.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#ifndef child_proc_h
#define child_proc_h

#include "child_struct.h"
#include "logging.h"
#include "message.h"
#include "programm_manager.h"
#include "read_message.h"
#include "smtp.h"
#include "smtp_fsm.h"
#include <stdio.h>
#include <string.h>
#include "maildir.h"

#define MAX_MESSAGE_LIST_NAME 2048

int child_loop(int i, int attempts_number, int attempts_delay);
int resv_mes_main(char *queue_name, char *buffer);

struct string_list {
  char file_name[MAX_FILE_NAME_LEN];
  struct string_list *next;
};

typedef struct sock_struct {
  child_state state;
  int sock_descr;
  int count_try;
  char domain[MAX_DOMAIN_LEN];
  char message_list[MAX_MESSAGE_LIST_NAME];

  struct message *message;
} sock_struct;

int child_proc_read(sock_struct *sock_connection);
int error_func(int *count_try, int attempt_number);
int close_socket(int socket, fd_set *read_fds);

#endif /* child_proc_h */
