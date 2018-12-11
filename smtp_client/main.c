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
#include "smtp.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <resolv.h>


int main(int argc, const char * argv[]) {
//    // insert code here...
//
////    rename("/Users/mam/xcode/smtp_client/maildir/new/1.txt", "/Users/mam/xcode/smtp_client/maildir/cur/1.txt");
//
//    struct message* message1 = read_message("/Users/mam/xcode/smtp_client/maildir/new/1.txt");
//    struct message* message2 = read_message("/Users/mam/xcode/smtp_client/maildir/new/2.txt");
//    struct message* message3 = read_message("/Users/mam/xcode/smtp_client/maildir/new/3.txt");
//    struct message* message4 = read_message("/Users/mam/xcode/smtp_client/maildir/new/4.txt");
//    struct message* message5 = read_message("/Users/mam/xcode/smtp_client/maildir/new/5.txt");
//
//
//    struct message_list* mes_list_1 = create_message_el_list(message1);
//    struct message_list* mes_2 = create_message_el_list(message2);
//    struct message_list* mes_3 = create_message_el_list(message3);
//
//    struct message_list* mes_list_4 = create_message_el_list(message4);
//    struct message_list* mes_5 = create_message_el_list(message5);
//
//    mes_list_1 = add_el_mes_list(mes_list_1, mes_2);
//    mes_list_1 = add_el_mes_list(mes_list_1, mes_3);
//    mes_list_4 = add_el_mes_list(mes_list_4, mes_5);
//
//    printf("%s\n",message2->id);
//
//    free_message_list(mes_list_4);
//    free_message_list(mes_list_1);
    
    //struct message* message1 = read_message("/Users/mam/xcode/smtp_client/maildir/new/1.txt");

    char sender[] = "orilbit@yandex.ru";
    char receiver[] = "c4.i1@yandex.ru";
    char subject[] = "diploma";
    char date[] = "diploma";
    char id[] = "1111";
    char body[] = "12345";
    
    struct message_envelope* envelope = create_envelope(sender, receiver, subject, date);
    struct message* message = create_message();
    message = fill_message(message, id, envelope, body);


    const char strServerName[] = "mx.yandex.ru";
    int sock;

    sock = connect_to_host(strServerName);


    int a;
    a = send_messages_high(message, sock);

    close(sock);
    free_message(message);
    
    
    
    return 0;
}
