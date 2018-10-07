//
//  main.c
//  smtp_client
//
//  Created by mam on 07.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//
/*
 список писем - > письмо -> нагрузка, конврет -> (отправитель, список получателей -> получателей)
*/

#include <stdio.h>
#include "envelope.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    struct message_envelope* envelope = create_envelope("ilya@gd.ru", "fhf@fhf.ru", "hello", "12.12");
    return 0;
}
