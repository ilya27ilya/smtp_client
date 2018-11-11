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
    
    struct message* message1 = read_message("/Users/mam/xcode/smtp_client/maildir/new/1.txt");
    struct message* message2 = read_message("/Users/mam/xcode/smtp_client/maildir/new/2.txt");
    struct message* message3 = read_message("/Users/mam/xcode/smtp_client/maildir/new/3.txt");
    struct message* message4 = read_message("/Users/mam/xcode/smtp_client/maildir/new/4.txt");
    struct message* message5 = read_message("/Users/mam/xcode/smtp_client/maildir/new/5.txt");
    
    struct message_list* mes_list = create_message_el_list(message1);
    struct message_list* mes_2 = create_message_el_list(message2);
    struct message_list* mes_3 = create_message_el_list(message3);
    struct message_list* mes_4 = create_message_el_list(message4);
    struct message_list* mes_5 = create_message_el_list(message5);
    
    mes_list = add_el_mes_list(mes_list, mes_2);
    mes_list = add_el_mes_list(mes_list, mes_3);
    mes_list = add_el_mes_list(mes_list, mes_4);
    mes_list = add_el_mes_list(mes_list, mes_5);
    
    printf("%s\n",message2->id);
    
    free_message_list(mes_list);
    return 0;
}
