//
//  run.c
//  smtp_client
//
//  Created by mam on 14.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#include "run.h"

void signalHandler_logger(int signum)
{
    
    if (signum == SIGINT)
    {
    }
    
}

int initSignalHandler_l()
{
    struct sigaction actions;
    memset(&actions, 0, sizeof(actions));
    actions.sa_handler = signalHandler_logger;
    
    sigemptyset(&actions.sa_mask);
    sigaddset(&actions.sa_mask, SIGINT);
    sigaction(SIGINT, &actions, NULL);
    return 0;
}

int run(input_struct input_data) {
  int result = 0;
  if (fork() == 0) {
      initSignalHandler_l();
    result = logging_loop(input_data.logger_name);
  } else {
    write_log(0,INFO_LOG, "START SMTP CLIENT");
    result = init_manager(input_data);
  }
  // printf("%s",input_data.logger_name);
  return result;
}


