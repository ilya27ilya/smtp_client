//
//  child_struct.h
//  smtp_client
//
//  Created by mam on 15.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#ifndef child_struct_h
#define child_struct_h

typedef struct child_info{
    int pid;            // Массив id процессов
    char* queue_name;    // Массив названия очередей процессов
    int messenge_number; // Массив массив количества сообщений процессов
    int connection;      // Массив количества подключений процессов
    char* domain;        // Массив строк содержащий домены для процессов
}child_info;

typedef struct child_struct {
    int child_id;
    child_info child_data;
}child_struct;


#endif /* child_struct_h */
