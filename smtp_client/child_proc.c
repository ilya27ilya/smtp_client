//
//  child_proc.c
//  smtp_client
//
//  Created by mam on 15.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#include "child_proc.h"

int child_loop(int i, int attempts_number, int attempts_delay) {
  write_log(INFO_LOG, "Start child proc %d", i);

  child_struct child;
  child.child_id = i;
  child.child_data.pid = getpid();
  child.child_data.connection = 0;
  child.child_data.messenge_number = 0;
  child.child_data.domain = malloc(MAX_DOMAIN_LIST_LEN);
  strcpy(child.child_data.domain, "");
  child.child_data.queue_name = malloc(7);
  memset(child.child_data.queue_name, 0, 7);
  strcpy(child.child_data.queue_name, "/");
  strcat(child.child_data.queue_name, itoa(child.child_data.pid, 10));

  sock_struct sock_connection[MAX_COUNT_DOMAIN];
  for (int i = 0; i < MAX_COUNT_DOMAIN; i++) {
    strcat(sock_connection[i].domain, "");
    sock_connection[i].sock_descr = -1;
    strcat(sock_connection[i].message_list, "");
    sock_connection[i].count_try = 3;
  }

  fprintf(stdout, "Child %d %d %d %d %s %s \n", child.child_id,
          child.child_data.pid, child.child_data.connection,
          child.child_data.messenge_number, child.child_data.domain,
          child.child_data.queue_name);

  struct mq_attr attr;
  memset(&attr, 0, sizeof(struct mq_attr));
  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MSG_NUM;
  attr.mq_msgsize = MAX_LOG_MES_SIZE;
  attr.mq_curmsgs = 0;
  mqd_t mq = mq_open(child.child_data.queue_name,
                     O_CREAT | O_RDONLY | O_NONBLOCK, 0644, &attr);

  if (mq == -1) {
    write_log(ERROR_LOG, " Cant Create Mq %d with name [%s]", mq,
              child.child_data.queue_name);
    return -1;
  }
  write_log(INFO_LOG, "Create Mq %d with name [%s]", mq,
            child.child_data.queue_name);

  fd_set read_fds;
  fd_set write_fds;

  FD_ZERO(&read_fds);
  FD_ZERO(&write_fds);

  int maxfd = 0;

  while (1) {
    char buffer[MAX_LOG_MES_SIZE + 1];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytes_read = mq_receive(mq, buffer, MAX_LOG_MES_SIZE, NULL);
    
    if (strcmp(buffer, "STOP WORKING ")==0) {
        write_log(INFO_LOG, "GET STOP WORKING");
        for (int i = 0; i <= maxfd; i++)
            if (FD_ISSET(i, &read_fds) || FD_ISSET(i, &write_fds))
                close(i);
            return 0;
      }

    struct file_list *mes_queue = NULL;
    mes_queue = (struct file_list *)malloc(sizeof(struct file_list));

    //Когда получили сообщение
    if (bytes_read > 0) {

      //прочитали сообщение
      memset(mes_queue->domain, 0, sizeof(mes_queue->domain));
      memset(mes_queue->file_name, 0, sizeof(mes_queue->file_name));
      write_log(INFO_LOG, "Get mess adr: [%s] domain: [%s]",
                mes_queue->file_name, mes_queue->domain);

      char *istr = NULL;
      istr = strtok(buffer, " ");
      strcat(mes_queue->file_name, istr);
      istr = strtok(NULL, " ");
      strcat(mes_queue->domain, istr);
      write_log(INFO_LOG, "Get mess adr: [%s] domain: [%s]",
                mes_queue->file_name, mes_queue->domain);

      //если в списке доменов нет полученого, то добавили
      if (strstr(child.child_data.domain, mes_queue->domain) == NULL)
        strcpy(child.child_data.domain, mes_queue->domain);

      //определяем какому сокету отправить сообщение
      int res = -1;
      for (int i = 0; i < MAX_COUNT_DOMAIN; i++) {
        char *istr = strstr(sock_connection[i].domain, mes_queue->domain);
        if (istr != NULL) {
          res = i;
          break;
        }
      }
      //если такого подключения нет, то в первый доступный;
      if (res == -1) {
        res = child.child_data.connection;
        child.child_data.connection += 1;

        //если уже все заняты, то ошибка и выход
        if (child.child_data.connection == MAX_COUNT_DOMAIN) {
          write_log(ERROR_LOG, "There is no empty domain");
          goto bad_event;
        }

        //если соктет не имеет подключения создаем его
        if (sock_connection[res].sock_descr < 0) {
          strcpy(sock_connection[res].domain, mes_queue->domain);
          create_socket(get_mx(sock_connection[res].domain), PORT,
                        attempts_number, attempts_delay,
                        &sock_connection[res].sock_descr);
          read_response(sock_connection[i].sock_descr);
          printf("Child proc sock creation [%d]\n",
                 sock_connection[res].sock_descr);
        }

        if (sock_connection[res].sock_descr <= 0) {
          write_log(ERROR_LOG, "Cant create connection to domain: [%s]",
                    sock_connection[res].domain);
          memset(sock_connection[res].domain, 0,
                 sizeof(sock_connection[res].domain));
        } else {
          printf("Connection with %s establised sock descr is %d",
                 sock_connection[res].domain, sock_connection[res].sock_descr);
          if (maxfd < sock_connection[res].sock_descr) {
            maxfd = sock_connection[res].sock_descr;
            sock_connection[res].state = INIT; // INIT
            FD_SET(sock_connection[res].sock_descr, &write_fds);
          }
        }
      } //создание подключения

      strcat(sock_connection[res].message_list, mes_queue->file_name);
      strcat(sock_connection[res].message_list, "@");

      write_log(INFO_LOG, "Add mess to sock [%d]  mess_list: [%s] domain: [%s]",
                sock_connection[res].sock_descr,
                sock_connection[res].message_list, sock_connection[res].domain);

      memset(mes_queue->domain, 0, sizeof(mes_queue->domain));
      memset(mes_queue->file_name, 0, sizeof(mes_queue->file_name));

      mes_queue->next = NULL;
      free(mes_queue);

    } //завершили прием сообщения

    if (child.child_data.connection > 0) {
      select(maxfd + 1, &read_fds, &write_fds, NULL, NULL);
      for (int i = 0; i < MAX_COUNT_DOMAIN; i++) {
        int result_code = -1;
        int a = -1;
        int smtp_fsm_res = -1;

        if (sock_connection[i].sock_descr > 2) {
          if (FD_ISSET(sock_connection[i].sock_descr, &read_fds)) {
            child_state old_state = sock_connection[i].state;
            result_code = read_response(sock_connection[i].sock_descr);


            smtp_fsm_res =
                smtp_myfsm_advance(&sock_connection[i].state, result_code);
              
              if (sock_connection[i].state == QUIT_REC_STATE) {
                  result_code =
                  close_socket(sock_connection[i].sock_descr, &read_fds);
                  break;
              }

            //сообщение отправлено
            if ((smtp_fsm_res == 0) &&
                (sock_connection[i].state == READ_MES_STATE) &&
                ((old_state == BODDY_REC_STATE) || (old_state == ERROR_STATE))) {

            ///printf("FREE message\n");
              //free_message(sock_connection[i].message);
              sock_connection[i].message = NULL;
              //free_message(sock_connection[i].message);
              sock_connection[i].count_try = 0;
            }

            if (smtp_fsm_res < 0)
              goto bad_event;
            else {
              FD_CLR(sock_connection[i].sock_descr, &read_fds);
              FD_SET(sock_connection[i].sock_descr, &write_fds);
            }
          }

          //писатели
          if (FD_ISSET(sock_connection[i].sock_descr, &write_fds)) {

            if (sock_connection[i].state == ERROR_STATE) {
              int er =
                  error_func(&sock_connection[i].count_try, attempts_number);
              smtp_fsm_res = smtp_myfsm_advance(&sock_connection[i].state, er);
              if (smtp_fsm_res < 0)
                goto bad_event;
            }
            if (sock_connection[i].state == INIT) {
              smtp_fsm_res = smtp_myfsm_advance(&sock_connection[i].state, 1);
              if (smtp_fsm_res < 0)
                goto bad_event;
            }
            if (sock_connection[i].state == READ_MES_STATE) {

              int read = child_proc_read(&sock_connection[i]);
              smtp_fsm_res =
                  smtp_myfsm_advance(&sock_connection[i].state, read);
              if (smtp_fsm_res < 0)
                goto bad_event;
            }
              int bytes = 0;
            if (sock_connection[i].state == EHLO_STATE)
              a = send_command(sock_connection[i].sock_descr, EHLO_STATE,
                               MY_NAME, &bytes);
            if (sock_connection[i].state == MAIL_FROM_STATE)
              a = send_command(sock_connection[i].sock_descr, MAIL_FROM_STATE,
                               sock_connection[i].message->envelope->sender, &bytes);
            if (sock_connection[i].state == RCPT_TO_STATE)
              a = send_command(sock_connection[i].sock_descr, RCPT_TO_STATE,
                               sock_connection[i].message->envelope->receiver, &bytes);
            if (sock_connection[i].state == DATA_STATE)
              a = send_command(sock_connection[i].sock_descr, DATA_STATE, NULL, &bytes);
              if (sock_connection[i].state == BODDY_STATE) {
                  char *tmp_body = sock_connection[i].message->body;
                  char bf[BUFFER_SIZE];
                  a = send_command(sock_connection[i].sock_descr, BODDY_STATE, tmp_body, &bytes);
                  if (bytes > 0) {
                      tmp_body += bytes;
                      if(strlen(tmp_body) != 0){
                          snprintf(bf, BUFFER_SIZE, "%s", tmp_body);
                          snprintf(sock_connection[i].message->body, BUFFER_SIZE, "%s", bf);
                          FD_SET(sock_connection[i].sock_descr, &write_fds);
                          continue;
                      }
                  }
                  else if (errno == EWOULDBLOCK) {
                      FD_SET(sock_connection[i].sock_descr, &write_fds);
                      continue;
                  }
                  else {
                      a = -1;
                  }
              }
            if (sock_connection[i].state == RSET_STATE)
              a = send_command(sock_connection[i].sock_descr, RSET_STATE, NULL, &bytes);

            if (sock_connection[i].state == QUIT_STATE) {
              a = send_command(sock_connection[i].sock_descr, QUIT_STATE, NULL, &bytes);
              memset(sock_connection[i].domain, 0,
                     sizeof(sock_connection[i].domain));
            }

            smtp_fsm_res = smtp_myfsm_advance(&sock_connection[i].state, a);

            if (smtp_fsm_res < 0)
              return -1;
            else {
              FD_CLR(sock_connection[i].sock_descr, &write_fds);
              FD_SET(sock_connection[i].sock_descr, &read_fds);
            }
          }
        }
      } // select цикл по сокетам
    }   // end if

    //sleep(1);
  }

  free(child.child_data.domain);
  free(child.child_data.queue_name);
  return 0;
  
  bad_event:{
    for(int i = 0; i<child.child_data.connection;i++)
    {
      close(sock_connection[i].sock_descr);
      free_message(sock_connection[i].message);
    }
    return -1;
  }
}

int resv_mes_main(char *queue_name, char *buffer) {
  static mqd_t mq = -1;

  if (mq == -1) {
    mq = mq_open(queue_name, O_RDONLY);
    write_log(INFO_LOG, "Mq %d pid %d", mq, getpid());
  }
  if (mq) {
    ssize_t bytes_read = mq_receive(mq, buffer, MAX_LOG_MES_SIZE, NULL);
    if (bytes_read > 0) {
      return 0;
    }
  }
  return 1;
}

int child_proc_read(sock_struct *sock_connection) {
  if (strcmp(sock_connection->message_list, "") == 0) {
    return 0;
  }

  int ch = '@';
  char *beg = strchr(sock_connection->message_list, ch);

  char filename[BUFFER_SIZE];
  char last_list[BUFFER_SIZE];
  memset(filename, 0, BUFFER_SIZE);
  memset(last_list, 0, BUFFER_SIZE);

  long len = strlen(sock_connection->message_list);

  for (int i = 0; i < beg - sock_connection->message_list; i++) {
    filename[i] = sock_connection->message_list[i];
  }

  for (long i = beg - sock_connection->message_list + 1; i < len; i++) {
    last_list[i - (beg - sock_connection->message_list + 1)] =
        sock_connection->message_list[i];
  }

  sock_connection->message = read_message(filename);
  memset(sock_connection->message_list, 0,
         sizeof(sock_connection->message_list));
  strcat(sock_connection->message_list, last_list);

  if (sock_connection->message == NULL) {
    return 0;
  } else
    return 1;
}

int error_func(int *count_try, int attempt_number) {
  *count_try += 1;
  return (*count_try == attempt_number) ? 0 : 1;
}

int close_socket(int socket, fd_set *read_fds) {
  FD_CLR(socket, read_fds);
  return (close(socket) == 0) ? 1 : 0;
}
