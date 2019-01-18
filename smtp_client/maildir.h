//
//  maildir.h
//  smtp_client
//
//  Created by mam on 11.11.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#ifndef maildir_h
#define maildir_h

#include "read_message.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_NAME_LEN 256
#define MAX_DOMAIN_LEN 256

/*
Когда почтовый клиент находит сообщения в каталоге new, он перемещает их в cur
(с помощью rename())
 */

struct file_list {
  char file_name[MAX_FILE_NAME_LEN];
  char domain[MAX_DOMAIN_LEN];
  struct file_list *next;
};

void file_rename(char *src, char *dest);
struct file_list *get_name(char *new_maildir_name, char *cur_maildir_name);
int get_domain(char *file_name, char *domain);

#endif /* maildir_h */
