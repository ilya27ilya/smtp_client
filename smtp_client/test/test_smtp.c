//
//  test_smtp.c
//  smtp_client
//
//  Created by mam on 18.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#include "test_smtp.h"

TEST_FUNCT(read_file_correct) {
    struct message* mesage = NULL;
    mesage = read_message("./test/read_test/5.txt");
    char* sender = "galchenkova.mari@gmail.com";
    char* recv = "ilya_chizh@mail.ru";
    char* subj = "kurt";
    char* id = "CAH3bCGoq2yeMdVO_dVKzARRt+jturUkLQkon10Mk+F3e=ybQ+Q@mail.gmail.com";
    
    int res = strcmp(mesage->envelope->sender, sender);
    CU_ASSERT_EQUAL(res,0);
    res = strcmp(mesage->envelope->receiver, recv);
    CU_ASSERT_EQUAL(res,0);
    res = strcmp(mesage->envelope->subject, subj);
    CU_ASSERT_EQUAL(res,0);
    res = strcmp(mesage->id, id);
    CU_ASSERT_EQUAL(res,0);
    free_message(mesage);
}
TEST_FUNCT(read_file_incorrect) {
    struct message* mesage = NULL;
    mesage = read_message("./test/read_test/9.txt");
    CU_ASSERT_EQUAL(mesage,NULL);
    free_message(mesage);
}

TEST_FUNCT(fsm_correct) {
    int res;
    
    child_state state = INIT;
    res = smtp_myfsm_advance(&state, 1);
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(state,EHLO_STATE);
    
    state = EHLO_STATE;
    res = smtp_myfsm_advance(&state, 1);
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(state,EHLO_REC_STATE);
    
    state = EHLO_REC_STATE;
    res = smtp_myfsm_advance(&state, 1);
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(state,READ_MES_STATE);
    
    state = MAIL_FROM_STATE;
    res = smtp_myfsm_advance(&state, 1);
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(state,MAIL_FROM_STATE_REC);
    
    state = MAIL_FROM_STATE_REC;
    res = smtp_myfsm_advance(&state, 1);
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(state,RCPT_TO_STATE);
    
    state = RCPT_TO_STATE;
    res = smtp_myfsm_advance(&state, 1);
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(state,RCPT_TO_STATE_REC);
    
    state = RCPT_TO_STATE_REC;
    res = smtp_myfsm_advance(&state, 1);
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(state,DATA_STATE);
    
    state = DATA_STATE;
    res = smtp_myfsm_advance(&state, 1);
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(state,DATA_REC_STATE);
    
    state = DATA_REC_STATE;
    res = smtp_myfsm_advance(&state, 1);
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(state,BODDY_STATE);
    
    state = BODDY_STATE;
    res = smtp_myfsm_advance(&state, 1);
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(state,BODDY_REC_STATE);
    
    state = READ_MES_STATE;
    res = smtp_myfsm_advance(&state, 1);
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(state,MAIL_FROM_STATE);
    
    state = READ_MES_STATE;
    res = smtp_myfsm_advance(&state, 0);
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(state,QUIT_STATE);
    
    state = QUIT_STATE;
    res = smtp_myfsm_advance(&state, 1);
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(state,QUIT_REC_STATE);
}
TEST_FUNCT(fsm_incorrect) {
    int res;
    
    child_state state = INIT;
    res = smtp_myfsm_advance(&state, 2);
    CU_ASSERT_EQUAL(res,-3);

    
    state = EHLO_STATE;
    res = smtp_myfsm_advance(&state, 2);
    CU_ASSERT_EQUAL(res,-3);
    
    state = EHLO_REC_STATE;
    res = smtp_myfsm_advance(&state, 2);
    CU_ASSERT_EQUAL(res,-3);
    
    state = MAIL_FROM_STATE;
    res = smtp_myfsm_advance(&state, 2);
    CU_ASSERT_EQUAL(res,-3);
    
    state = MAIL_FROM_STATE_REC;
    res = smtp_myfsm_advance(&state, 2);
    CU_ASSERT_EQUAL(res,-3);
    
    state = RCPT_TO_STATE;
    res = smtp_myfsm_advance(&state, 2);
    CU_ASSERT_EQUAL(res,-3);
    
    state = RCPT_TO_STATE_REC;
    res = smtp_myfsm_advance(&state, 2);
    CU_ASSERT_EQUAL(res,-3);
    
    state = DATA_STATE;
    res = smtp_myfsm_advance(&state, 2);
    CU_ASSERT_EQUAL(res,-3);
    
    state = DATA_REC_STATE;
    res = smtp_myfsm_advance(&state, 2);
    CU_ASSERT_EQUAL(res,-3);
    
    state = BODDY_STATE;
    res = smtp_myfsm_advance(&state, 2);
    CU_ASSERT_EQUAL(res,-3);
    
    state = READ_MES_STATE;
    res = smtp_myfsm_advance(&state, 2);
    CU_ASSERT_EQUAL(res,-3);
    
    state = READ_MES_STATE;
    res = smtp_myfsm_advance(&state, 2);
    CU_ASSERT_EQUAL(res,-3);
    
    state = QUIT_STATE;
    res = smtp_myfsm_advance(&state, 2);
    CU_ASSERT_EQUAL(res,-3);
}

TEST_FUNCT(connection_correct) {
    int sock;
    int port = 25;
    char *dm  = "yandex.ru";
    int attempts_number = 3;
    int attempts_delay = 3;
    int sock_c;
    int res;

    sock_c = -1;
    res = create_socket(get_mx(dm), port, attempts_number, attempts_delay, &sock);
    if(sock > 2) sock_c = 0;
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(sock_c,0);
    close(sock);
    
    sock_c = -1;
    res = create_socket(get_mx("gmail.com"), port, attempts_number, attempts_delay, &sock);
    if(sock > 2) sock_c = 0;
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(sock_c,0);
    close(sock);
    
    sock_c = -1;
    res = create_socket(get_mx("mail.ru"), port, attempts_number, attempts_delay, &sock);
    if(sock > 2) sock_c = 0;
    CU_ASSERT_EQUAL(res,0);
    CU_ASSERT_EQUAL(sock_c,0);
    close(sock);
}

TEST_FUNCT(connection_incorrect) {
    int sock;
    int port = 25;
    char *dm  = "yandex.ru";
    int attempts_delay = 3;
    int res;
    
    res = create_socket(get_mx(dm), port, 0, attempts_delay, &sock);
    CU_ASSERT_EQUAL(res,-1);
    
    res = create_socket(get_mx("sds"), port, 2, attempts_delay, &sock);
    CU_ASSERT_EQUAL(res,-1);
}

TEST_FUNCT(ehlo_correct) {
    int sock;
    int port = 25;
    char *dm  = "yandex.ru";
    int attempts_delay = 3;
    int attempts_number = 3;
    int a;

    create_socket(get_mx(dm), port, attempts_number, attempts_delay, &sock);
    
    a = send_command(sock, EHLO_COMMAND, MY_NAME);
    CU_ASSERT_EQUAL(a,1);
    sleep(1);
    a = read_response(sock);
    CU_ASSERT_EQUAL(a,1);
    close(sock);
}

TEST_FUNCT(ehlo_incorrect) {
    int sock = -7;
    int a;
    
    
    a = send_command(sock, EHLO_STATE, MY_NAME);
    CU_ASSERT_EQUAL(a,0);
    sleep(1);
    a = read_response(sock);
    CU_ASSERT_EQUAL(a,0);
    close(sock);
}

TEST_FUNCT(ehlo_incorrect_send) {
    int sock;
    int port = 25;
    char *dm  = "yandex.ru";
    int attempts_delay = 3;
    int attempts_number = 3;
    int a;
    
    create_socket(get_mx(dm), port, attempts_number, attempts_delay, &sock);
    
    a = send_command(sock, MAIL_FROM_STATE, MY_NAME);
    CU_ASSERT_EQUAL(a,1);
    sleep(1);
    a = read_response(sock);
    CU_ASSERT_EQUAL(a,1);
    close(sock);
}

TEST_FUNCT(mail_send) {
    int sock;
    int port = 25;
    char *dm  = "yandex.ru";
    int attempts_delay = 3;
    int attempts_number = 3;
    int a;
    struct message* mesage = NULL;
    mesage = read_message("./test/read_test/1.txt");
    
    create_socket(get_mx(dm), port, attempts_number, attempts_delay, &sock);
    
    a = send_command(sock, EHLO_STATE, MY_NAME);
    CU_ASSERT_EQUAL(a,1);
    sleep(1);
    a = read_response(sock);
    CU_ASSERT_EQUAL(a,1);
    
    a = send_command(sock, MAIL_FROM_STATE, mesage->envelope->sender);
    CU_ASSERT_EQUAL(a,1);
    sleep(1);
    a = read_response(sock);
    CU_ASSERT_EQUAL(a,1);
    
    a = send_command(sock, RCPT_TO_STATE, mesage->envelope->receiver);
    CU_ASSERT_EQUAL(a,1);
    sleep(1);
    a = read_response(sock);
    CU_ASSERT_EQUAL(a,1);
    
    a = send_command(sock, DATA_STATE, NULL);
    CU_ASSERT_EQUAL(a,1);
    sleep(1);
    a = read_response(sock);
    CU_ASSERT_EQUAL(a,1);
    
    a = send_command(sock, BODDY_STATE, mesage->body);
    CU_ASSERT_EQUAL(a,1);
    sleep(5);
    a = read_response(sock);
    CU_ASSERT_EQUAL(a,1);
    
    a = send_command(sock, QUIT_STATE, NULL);
    CU_ASSERT_EQUAL(a,1);
    sleep(1);
    a = read_response(sock);
    CU_ASSERT_EQUAL(a,1);
    close(sock);
    free_message(mesage);
}

TEST_FUNCT(mail_send_rset_fsm) {
    int sock;
    int port = 25;
    char *dm  = "yandex.ru";
    int attempts_delay = 3;
    int attempts_number = 3;
    int a;
    int count = 0;
    int try = 0;

    create_socket(get_mx(dm), port, attempts_number, attempts_delay, &sock);
    child_state state = EHLO_STATE;
    
    if(state == EHLO_STATE){
        a = send_command(sock, state, MY_NAME);
        CU_ASSERT_EQUAL(a,1);
        smtp_myfsm_advance(&state, a);
    }
    sleep(1);
    if(state == EHLO_REC_STATE){
        a = read_response(sock);
        CU_ASSERT_EQUAL(a,1);
        smtp_myfsm_advance(&state, a);
    }
    
    state = MAIL_FROM_STATE;
    while (state != QUIT_STATE) {
        if(state == MAIL_FROM_STATE){
            a = send_command(sock, state, MY_NAME);
            CU_ASSERT_EQUAL(a,1);
            smtp_myfsm_advance(&state, a);
        }
        sleep(1);
        if(state == MAIL_FROM_STATE_REC){
            a = read_response(sock);
            CU_ASSERT_EQUAL(a,0);
            smtp_myfsm_advance(&state, a);
        }
        if (state == ERROR_STATE) {
            a = error_func(&try, attempts_number);
            smtp_myfsm_advance(&state,a);
        }
        if (state == READ_MES_STATE) {
            state = QUIT_STATE;
        }
        if(state == RSET_STATE){
            a = send_command(sock, state, NULL);
            smtp_myfsm_advance(&state, a);
        }
        
        if(state == RSET_REC_STATE){
            a = read_response(sock);
            smtp_myfsm_advance(&state, a);
        }
        
        count+=1;
        sleep(1);
    }
    CU_ASSERT_EQUAL(count,5);
    
    a = send_command(sock, QUIT_STATE, NULL);
    CU_ASSERT_EQUAL(a,1);
    sleep(1);
    a = read_response(sock);
    CU_ASSERT_EQUAL(a,1);
    
    close(sock);
}



int add_to_suit_smtp_test(CU_pSuite suite) {
    if (suite) {
        ADD_SUITE_TEST(suite, read_file_correct)
        ADD_SUITE_TEST(suite, read_file_incorrect)
        ADD_SUITE_TEST(suite, fsm_correct)
        ADD_SUITE_TEST(suite, fsm_incorrect)
        ADD_SUITE_TEST(suite, connection_correct)
        ADD_SUITE_TEST(suite, connection_incorrect)
        ADD_SUITE_TEST(suite, ehlo_correct)
        ADD_SUITE_TEST(suite, ehlo_incorrect)
        ADD_SUITE_TEST(suite, ehlo_incorrect_send)
        ADD_SUITE_TEST(suite, mail_send)
        ADD_SUITE_TEST(suite, mail_send_rset_fsm)
        
    }
    return CU_get_error();
}

