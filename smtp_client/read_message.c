//
//  read_message.c
//  smtp_client
//
//  Created by mam on 08.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "read_message.h"

struct message* read_message(char* filename){
    FILE* file = fopen(filename,"r");
    if (!file) {
        return NULL;
    }
    struct message* message  = create_message();
    char tmp[BUFFER_SIZE];
    unsigned char fl = 0;
    
//    while (fgets(tmp, BUFFER_SIZE, file)!=NULL ){
//        if(strlen(tmp)==1){
//            fl = 1
//        }
//        if(fl){
//
//        }
//        else{
//
//        }
//
//
//    }
//
//    message->body =
//    message->envelope =
//    message->id =
    return message;
}
// конверт - на вход строка в зависимости от того, с чего начитается (рег выр), то и возвращает


