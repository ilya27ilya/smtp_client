//
//  smtp.c
//  smtp_client
//
//  Created by mam on 11.11.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "smtp.h"

int send_command(int sock, child_state command_type, char *load, int *bytes) {
  char command[DATA_BUFFER];
  bzero(command, DATA_BUFFER);

  if (command_type == EHLO_STATE) {
    strncpy(command, EHLO, strlen(EHLO));
    strcat(command, load);
    printf("\nSend command EHLO");
  } else if (command_type == MAIL_FROM_STATE) {
    strncpy(command, MAIL_FROM, strlen(MAIL_FROM));
    strcat(command, OPEN_BRACKET);
    strcat(command, load);
    strcat(command, CLOSE_BRACKET);
    printf("Send command MAIL_FROM");
  } else if (command_type == RCPT_TO_STATE) {
    strncpy(command, RCPT_TO, strlen(RCPT_TO));
    strcat(command, OPEN_BRACKET);
    strcat(command, load);
    strcat(command, CLOSE_BRACKET);
    printf("Send command RCPT_TO");
  } else if (command_type == DATA_STATE) {
    strncpy(command, DATA, strlen(DATA));
    printf("Send command DATA");
  } else if (command_type == BODDY_STATE) {
    strncpy(command, load, strlen(load));
    strncat(command, DATA_END, strlen(DATA_END));
    printf("Send command BODY");
  } else if (command_type == RSET_STATE) {
    strncpy(command, RSET, strlen(RSET));
    printf("Send command RSET");
  } else if (command_type == QUIT_STATE) {
    strncpy(command, QUIT, strlen(QUIT));
    printf("Send command QUIT");
  }

  strcat(command, TERN);
  long a = write(sock, command, strlen(command));
  (*bytes) = a;
  printf(" %lu  ", a);
  return (a > 0) ? 1 : 0;
}

int create_socket(const char *host, int port, int attempts_number,
                  int attempts_delay, int *sock) {
  if(strcmp(host, "")==0)
      return -1;
  struct hostent *he;
  struct in_addr **addr_list;
  int socket_found = 0;
  char buffer[DATA_BUFFER];
  bzero(buffer, DATA_BUFFER);

  if ((he = gethostbyname(host)) == NULL)
    return -1;
  addr_list = (struct in_addr **)he->h_addr_list;

  for (int i = 0; addr_list[i] != NULL && !socket_found; i++) {
    struct sockaddr_in server;
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(inet_ntoa(*addr_list[i]));
    server.sin_port = htons(PORT);

    *sock = try_connect_to_socket(server, attempts_number, attempts_delay);
    if (*sock > 3) {
      socket_found = 1;
    }
  }
  if (socket_found == 0) {
    return -1;
  }
  fcntl(*sock, F_SETFL, O_NONBLOCK);
  // printf("create_sock, fun %d\n",*sock);
  return 0;
}

int try_connect_to_socket(struct sockaddr_in server, int attempts_number,
                          int attempts_delay) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  int i = 0;
  int result = -1;
  int connection_established = 0;
  while (i < attempts_number && !connection_established) {
    // printf("try to connect... \n");
    int connect_result =
        connect(sock, (struct sockaddr *)&server, sizeof(server));
    if (connect_result == 0) {
      connection_established = 1;
      result = sock;
    } else {
      sleep(attempts_delay);
    }
    i++;
  }
  // printf("try_con sock %d\n",result);
  return result;
}

long read_response(int sock) {
  char buffer[DATA_BUFFER];
  bzero(buffer, DATA_BUFFER);

  read(sock, buffer, DATA_BUFFER);
  long result_code = strtol(buffer, NULL, 10);
  // printf("%s\n",buffer);
  printf("READ code %lu \n", result_code);

  return ((result_code == SUCCESS_EHLO_CODE) ||
          (result_code == SUCCESS_OPERATION_CODE) ||
          (result_code == SUCCESS_QUIT_CODE) ||
          (result_code == SUCCESS_DATA_SEND_CODE))
             ? 1
             : 0;
}

char *get_mx(char *domain) {

  //    static char mx_server[512];
  //    u_char mx_buf[512];
  //    ns_msg msg;
  //    ns_rr rr;
  //    int r;
  //
  //    r = res_query(domain, ns_c_any, ns_t_mx, mx_buf, sizeof(mx_buf));
  //    if (r < 0)
  //    {
  //        printf("res q\n");
  //        return NULL;
  //    }
  //    else
  //    {
  //        ns_initparse(mx_buf, r, &msg);
  //        ns_parserr(&msg, ns_s_an, 0, &rr);
  //
  //        const size_t size = NS_MAXDNAME;
  //        unsigned char name[size];
  //        int t = ns_rr_type(rr);
  //
  //        const u_char *data = ns_rr_rdata(rr);
  //        if (t == T_MX)
  //        {
  //            ns_name_unpack(mx_buf, mx_buf + r, data + sizeof(u_int16_t),
  //            name, size); ns_name_ntop(name, mx_server, 4096);
  //        }
  //    }
  if (strcmp(domain, "localhost.com") == 0)
    return "localhost";
  else if (strcmp(domain, "gmail.com") == 0)
    return "ALT3.ASPMX.L.GOOGLE.COM.";

  else if (strcmp(domain, "yandex.ru") == 0)
    return "mx.yandex.ru";
  else if (strcmp(domain, "mail.ru") == 0)
    return "emx.mail.ru.";

  return "";
}
