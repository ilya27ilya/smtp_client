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
#include <libconfig.h>
#include "run.h"
#include "smtp.h"
#include "child_proc.h"
#include "string.h"
#include "programm_manager.h"

#define ARG_NUM_ERROR -7
#define ARG_NUM 1

void lookup_string(config_t cfg, char *lookup, char *dest, size_t size) {
    const char *tmp_string = NULL;
    config_lookup_string(&cfg, lookup, &tmp_string);
    snprintf(dest, size, "%s", tmp_string);
}

int read_config(char *cfg_filename, input_struct *input_data) {
    config_t cfg;
    config_init(&cfg);
    // config_read_file(&cfg, cfg_filename);
    printf("CONF FILENAME: %s\n", cfg_filename);
    if (!config_read_file(&cfg, cfg_filename)) {
        printf("CONF FILE NOT READ\n");
        config_destroy(&cfg);
        return -1;
    }
    //input_struct input_data_tmp;
    memset(input_data, 0, sizeof(input_struct));
    lookup_string(cfg, "new_dir", input_data->new_dir,sizeof(input_data->new_dir));
    lookup_string(cfg, "cur_dir", input_data->cur_dir,sizeof(input_data->new_dir));
    lookup_string(cfg, "logger_name", input_data->logger_name,sizeof(input_data->new_dir));
    config_lookup_int(&cfg, "attempt_number", &input_data->attempt_number);
    config_lookup_int(&cfg, "attempt_delay", &input_data->attempt_delay);
    config_lookup_int(&cfg, "max_proc_number", &input_data->max_proc_number);

    config_destroy(&cfg);
    return 0;
    
    
}
void print_config(input_struct input_data) {
    printf("\n*** CONFIG ***\n\n");
    printf("*** NEW_DIR: %s\n", input_data.new_dir);
    printf("*** CUR_DIR: %s\n", input_data.cur_dir);
    printf("*** LOGGER_DIR: %s\n", input_data.logger_name);
    printf("*** ATTEMPT_NUMBER: %lu\n", input_data.attempt_number);
    printf("*** ATTEMPT_DELAY: %lu\n", input_data.attempt_delay);
    printf("*** MAX_PROC_NUMBER: %lu\n", input_data.max_proc_number);
    printf("\n*** END CONFIG ***\n\n");
}

int main(int argc, const char * argv[]) {
    
//    if(argc != ARG_NUM){
//        printf("Enter config file\n");
//        return ARG_NUM_ERROR;
//    }
   
        int result = 0;

    input_struct input_data;
    result = read_config("confl.cfg", &input_data);
    if (!result) {
        //print_config(input_data);
        result = run(input_data);
        return result;
    }
    

//    sock_struct sock_connection[MAX_COUNT_DOMAIN];
//    for(int i = 0; i< MAX_COUNT_DOMAIN;i++){
//        strcat(sock_connection[i].domain,"");
//        sock_connection[i].sock_descr = -1;
//        strcat(sock_connection[i].message_list,"");
//    }
//
//    int free_sock_connection = 0;
//    char *filename = "../maildir/cur/2.txt";
//    char *domen  = "yandex.ru";
//
//    if(fork() == 0){
//        int res = -1;
//        for(int i = 0; i<MAX_COUNT_DOMAIN;i++){
//            char* istr = strstr(sock_connection[i].domain,domen);
//            if(istr != NULL){
//                res = i;
//                break;
//            }
//        }
//        if(res == -1){
//            res = free_sock_connection;
//            free_sock_connection+=1;
//            sock_connection[res].state = INIT;
//        }
//        strcat(sock_connection[res].domain,domen);
//        strcat(sock_connection[res].message_list,filename);
//        //strcat(sock_connection[res].message_list,"@");
//
//        create_socket(get_mx(sock_connection[res].domain), PORT, 3, 2,&sock_connection[res].sock_descr);
//        sock_connection[res].state = INIT;
//        //printf("Sock: [%d]\n",sock_connection[res].sock_descr);
//
//        //Состояние INIT в READ
//        sock_connection[res].state = READ_MES_STATE;
//
//        //READ_MES_STATE sock_connection[res].message!=NULL -> EHLO_STATE
//        //READ_MES_STATE sock_connection[res].message==0 -> READ_MES_STATE
//
//        if(sock_connection[res].state == READ_MES_STATE)
//        {
//            if(strcmp(sock_connection[res].message_list,"")!=0){
////                char *file_name = NULL;
////                file_name = strtok(sock_connection[res].message_list,"@");
////                printf("FILENAME [%s]\n",file_name);
////                strcpy(sock_connection[res].message_list, strtok (NULL," "));
////                printf("NEW FILENAMELIST [%s]\n",sock_connection[res].message_list);
////
////                char buff[BUFFER_SIZE];
////                strcat(buff, file_name);
////                printf("[%s]",buff);
//                sock_connection[res].message = read_message("../maildir/cur/2.txt");
//                if(sock_connection[res].message == NULL){
//                    printf("%d\n",sock_connection[res].sock_descr);
//                }
//            }
//        }
//
//
//        //EHLO_STATE a>0 -> REC_EHLO
//        int a = send_command(sock_connection[res].sock_descr, EHLO_COMMAND, MY_NAME);
//        sleep(5);
//
//        //REC_EHLO result_code==220 -> MAIL_FROM_STATE
//        int result_code = read_response(sock_connection[res].sock_descr);
//        printf("EHLO send = %d read = %d \n",a,result_code);
//
//        //MAIL_FROM_STATE a>0 -> REC_MAIL_FROM
//        a = send_command(sock_connection[res].sock_descr, MAIL_FROM_COMMAND, sock_connection[res].message->envelope->sender);
//        sleep(5);
//
//        //REC_MAIL_FROM result_code==250 -> RCPT_TO_STATE
//        result_code = read_response(sock_connection[res].sock_descr);
//        printf("MAIL FROM send = %d read = %d \n",a,result_code);
//
//        //RCPT_TO_STATE a>0 -> REC_RCPT_TO
//        a = send_command(sock_connection[res].sock_descr, RCPT_TO_COMMAND, sock_connection[res].message->envelope->receiver);
//        sleep(5);
//
//        //REC_RCPT_TO result_code==250 -> DATA_STATE
//        result_code = read_response(sock_connection[res].sock_descr);
//        printf("RCPT TO send = %d read = %d \n",a,result_code);
//
//        //DATA_STATE a>0 -> REC_DATA
//        a = send_command(sock_connection[res].sock_descr, DATA_COMMAND, NULL);
//        sleep(5);
//
//        //REC_DATA result_code==354 -> BODY_STATE
//        result_code = read_response(sock_connection[res].sock_descr);
//        printf("DATA send = %d read = %d \n",a,result_code);
//
//        //BODY_STATE a>0 -> REC_BODY
//        a = send_command(sock_connection[res].sock_descr, MESSAGE_BODY_COMMAND, sock_connection[res].message->body);
//        sleep(3);
//
//        //REC_BODY result_code==250 -> QUIT_STATE
//        result_code = read_response(sock_connection[res].sock_descr);
//        printf("BODY send = %d read = %d \n",a,result_code);
//
//        //QUIT_STATE a>0 -> REC_QUIT
//        a = send_command(sock_connection[res].sock_descr, QUIT_COMMAND, NULL);
//        sleep(3);
//        //REC_QUIT result_code==221 -> READ_MES_STATE
//        result_code = read_response(sock_connection[res].sock_descr);
//        printf("QUIT send = %d read = %d \n",a,result_code);
//
//
//
//    }//конец форка

    return result;
}
