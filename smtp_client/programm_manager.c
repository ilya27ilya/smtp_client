//
//  programm_manager.c
//  smtp_client
//
//  Created by mam on 11.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "programm_manager.h"



int Run(int attempts_number, int attempts_delay){
    
    int sock_ya = create_socket("mx.yandex.ru", PORT, attempts_number, attempts_delay);
    if(sock_ya < 0){
        return -1;
    }
    int sock_go = create_socket("gmail-smtp-in.l.google.com", PORT, attempts_number, attempts_delay);
    if(sock_go < 0){
        return -1;
    }
    int sock_ma = create_socket("mxs.mail.ru", PORT, attempts_number, attempts_delay);
    if(sock_ma < 0){
        return -1;
    }
    
    
    close(sock_ya);
    close(sock_go);
    close(sock_ma);
    
    return 0;
}
