//
//  list.h
//  smtp_client
//
//  Created by mam on 12.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#ifndef list_h
#define list_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct string_list
{
    char* value;
    struct string_list* next;
};

struct string_list* create_string_list_item(char* value);
void free_string_list(struct string_list* list);
struct string_list* add_to_string_list(struct string_list* list, struct string_list* element);
void print_string_list(struct string_list* list);

#endif /* list_h */
