//
//  main.c
//  smtp_client
//
//  Created by mam on 07.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//
/*
 список писем - > письмо -> нагрузка, конврет -> (отправитель, список получателей -> получателей)
 работать с файлами только на клиенте
*/

#include <stdio.h>
#include "message.h"
#include "read_message.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    struct message* message = read_message("/Users/mam/xcode/smtp_client/smtp_client/mail.txt");
    printf("%s\n",message->envelope->receiver);
    return 0;
}
