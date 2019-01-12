//
//  input_struct.h
//  smtp_client
//
//  Created by mam on 12.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#ifndef input_struct_h
#define input_struct_h

struct input_struct {
    char*   new_dir;
    char*   cur_dir;
    char*   logger_name;
    int     attempt_number;
    int     attempt_delay;
};

#endif /* input_struct_h */
