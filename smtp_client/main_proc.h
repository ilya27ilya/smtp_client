//
//  main_proc.h
//  smtp_client
//
//  Created by mam on 15.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#ifndef main_proc_h
#define main_proc_h

#include <stdio.h>
#include <string.h>
#include "logging.h"

int send_message_to_proc(char* queue_name, char* file_name, char* domain);

#endif /* main_proc_h */
