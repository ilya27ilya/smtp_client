//
//  test_smtp.h
//  smtp_client
//
//  Created by mam on 18.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#ifndef test_smtp_h
#define test_smtp_h

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "test_utils.h"
#include "../smtp.h"
#include "../read_message.h"
#include <string.h>
#include <libconfig.h>
#include "../child_proc.h"

#define BUF 400

//typedef struct input_struct {
//    char new_dir[1024];
//    char cur_dir[1024];
//    char logger_name[1024];
//    long int attempt_number;
//    long int attempt_delay;
//    long int max_proc_number;
//} input_struct;

int add_to_suit_smtp_test(CU_pSuite suite);

#endif /* test_smtp_h */
