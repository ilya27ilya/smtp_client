//
//  programm_manager.c
//  smtp_client
//
//  Created by mam on 11.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "programm_manager.h"

int init_manager(input_struct input_data) {

  int result = 0;
  long count = input_data.max_proc_number;
  int pids[count];
  int parent = 1;
  int k;
  write_log(INFO_LOG, "Create child proc");
  for (int i = 0; i < count; i++) {
    k = fork();
    if (k == 0) {
      result = child_loop(
          i, input_data.attempt_number,
          input_data.attempt_number); //если передавать i то он будет для этого
                                      //процесса и в менеджере
      parent = 0;
      break;
    } else {
      pids[i] = k;
    }
  }
  if (parent == 1) {
    initSignalHandler();
    result = run_main_loop(input_data, pids);
  }
  return result;
}

int run_main_loop(input_struct input_data, int *proc) {

  long proc_number = input_data.max_proc_number;
  child_info child_array[proc_number];
  
  pids = malloc(proc_number*sizeof(int));
  for(int i = 0; i<proc_number; i++){
        pids[i] = proc[i];
    }
  proc_num = proc_number;

  for (int i = 0; i < proc_number; i++) {
    child_array[i].connection = 0;
    child_array[i].messenge_number = 0;
    child_array[i].pid = proc[i];
    child_array[i].domain = malloc(sizeof(char) * MAX_DOMAIN_LIST_LEN);
    strcpy(child_array[i].domain, "@");
    child_array[i].queue_name = malloc(sizeof(char) * 7);
    memset(child_array[i].queue_name, 0, 7);
    strcpy(child_array[i].queue_name, "/");
    strcat(child_array[i].queue_name, itoa(proc[i], 10));
  }

  for (int i = 0; i < proc_number; i++)
    write_log(INFO_LOG, "Child proc: %d %d %d queue_name %s dom %s",
              child_array[i].pid, child_array[i].messenge_number,
              child_array[i].connection, child_array[i].queue_name,
              child_array[i].domain);

  int id;

  sleep(1);
  while (1) {
    // send_message_to_proc(child_array[1].queue_name, "22", "33");
    struct file_list *mes_queue = NULL;
    mes_queue = get_name(input_data.new_dir, input_data.cur_dir);
    struct file_list *next = mes_queue;
    
    if (mes_queue != NULL) {
      while (next != NULL) {
        write_log(INFO_LOG, "New message %s  -  %s", mes_queue->file_name,
                  mes_queue->domain);
        id = find_proc(child_array, mes_queue->domain, proc_number);
        int i = id / 10;
        if (id % 10 == 1) {
          strcat(child_array[i].domain, mes_queue->domain);
          child_array[i].connection += 1;
        }

        send_message_to_proc(child_array[i].queue_name, mes_queue->file_name,
                             mes_queue->domain);

        next = next->next;
        mes_queue = next;
      }
    } else {
      printf("there is now file in new_dir \n");
    }
    sleep(2);
  }
  free_child_info_array(child_array, proc_number);
  return 0;
}

int find_proc(child_info *child_array, char *domain, long proc_number) {
  int id = -1;
  char *istr = NULL;
  int new_domain_flag = 1;
  int min_connection = MAX_COUNT_DOMAIN + 1;

  for (int i = 0; i < proc_number; i++) {
    istr = strstr(child_array[i].domain, domain);
    if (istr != NULL) {
      write_log(INFO_LOG, "Domain alredy exist in proc %s",
                child_array[i].queue_name);
      id = i;
      new_domain_flag = 0;
      break;
    }
    if (min_connection > child_array[i].connection) {
      min_connection = child_array[i].connection;
      id = i;
    }
  }

  return id * 10 + new_domain_flag;
}

char *itoa(int val, int base) {
  if (val == 0) {
    return "0";
  }
  static char buf[32] = {0};
  int i = 30;
  for (; val && i; --i, val /= base)
    buf[i] = "0123456789abcdef"[val % base];
  return &buf[i + 1];
}

void free_child_info_array(child_info *child_array, long proc_number) {
  for (int i = 0; i < proc_number; i++) {
    free(child_array[i].queue_name);
    free(child_array[i].domain);
  }
}

void signalHandler(int signum)
{
    
    if (signum == SIGINT)
    {
        char mes[BUFFER_SIZE];
        printf("\nCatch SIGINT\n");
        
        for(int i = 0; i<proc_num;i++){
           strcpy(mes, "/");
           strcat(mes, itoa(pids[i], 10));
           send_message_to_proc(mes,"STOP","WORKING" );
           memset(mes, 0, BUFFER_SIZE);
           
        }
        write_log(ERROR_LOG, LOG_MSG_STOP);
        free(pids);
        int wait_status;
        while (wait(&wait_status) > 0);
        exit(EXIT_FAILURE);
    }
}

int initSignalHandler()
{
    struct sigaction actions;
    memset(&actions, 0, sizeof(actions));
    actions.sa_handler = signalHandler;
    
    sigemptyset(&actions.sa_mask);
    sigaddset(&actions.sa_mask, SIGINT);
    sigaction(SIGINT, &actions, NULL);
    return 0;
}

