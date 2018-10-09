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
#include "message_list.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    struct message* message1 = read_message("/Users/mam/xcode/smtp_client/maildir/1.txt");
    struct message* message2 = read_message("/Users/mam/xcode/smtp_client/maildir/2.txt");
    struct message* message3 = read_message("/Users/mam/xcode/smtp_client/maildir/3.txt");
    struct message* message4 = read_message("/Users/mam/xcode/smtp_client/maildir/4.txt");
    struct message* message5 = read_message("/Users/mam/xcode/smtp_client/maildir/5.txt");
    
    struct message_list* message_list = create_message_list(message1);
    message_list  = add_message_element(message_list, message2);
    message_list  = add_message_element(message_list, message3);
    message_list  = add_message_element(message_list, message4);
    message_list  = add_message_element(message_list, message5);
    
    printf("%s\n",message2->id);
    
//    free_message_list(message_list);
//
//    free_message(message1);
//    free_message(message2);
//    free_message(message3);
//    free_message(message4);
//    free_message(message5);
    return 0;
}
