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

#define SOCKET_FD 0
#define FILE_FD 1

struct FileDescSet
{
    fd_set set;
    struct FileDescList *list;
    int count;
};

struct FileDesc
{
    int id;
    int type;
    struct sockaddr_in addr;
    int context;
    int goal;
};

struct FileDescList
{
    struct FileDesc fd;
    struct FileDescList *next;
};




//инициализация
//работа
//остановка

#endif /* programm_manager_h */
