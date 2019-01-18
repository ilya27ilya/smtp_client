//
//  main.c
//  smtp_client
//
//  Created by mam on 07.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//
/*
 список писем - > письмо -> нагрузка, конврет -> (отправитель, список
 получателей -> получателей) работать с файлами только на клиенте
*/
#include "child_proc.h"
#include "programm_manager.h"
#include "run.h"
#include "smtp.h"
#include "smtp_fsm.h"
#include "string.h"
#include <libconfig.h>

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
  // input_struct input_data_tmp;
  memset(input_data, 0, sizeof(input_struct));
  lookup_string(cfg, "new_dir", input_data->new_dir,
                sizeof(input_data->new_dir));
  lookup_string(cfg, "cur_dir", input_data->cur_dir,
                sizeof(input_data->new_dir));
  lookup_string(cfg, "logger_name", input_data->logger_name,
                sizeof(input_data->new_dir));
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

int main(int argc, const char *argv[]) {

  //    if(argc != ARG_NUM){
  //        printf("Enter config file\n");
  //        return ARG_NUM_ERROR;
  //    }

  int result = 0;
  input_struct input_data;
  result = read_config("confl.cfg", &input_data);
  if (!result) {
    // print_config(input_data);
    result = run(input_data);
    return result;
  }

  return result;
}
