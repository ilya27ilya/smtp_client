//
//  input_struct.h
//  smtp_client
//
//  Created by mam on 12.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#ifndef input_struct_h
#define input_struct_h

typedef struct input_struct {
    char   new_dir[1024];
    char   cur_dir[1024];
    char   logger_name[1024];
    long int     attempt_number;
    long int     attempt_delay;
    long int     max_proc_number;
} input_struct;

#endif /* input_struct_h */
