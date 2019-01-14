//
//  maildir.c
//  smtp_client
//
//  Created by mam on 11.11.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "maildir.h"



struct file_list* get_name(char* new_maildir_name, char* cur_maildir_name){
    
    struct dirent *de;
    DIR *dr = opendir(new_maildir_name);
    
    if (dr == NULL)
    {
        printf("Could not open current directory" );
        return NULL;
    }
    
    struct file_list *mes_queue = NULL;
    struct file_list *next = mes_queue;
    
    char buf[MAX_FILE_NAME_LEN];
    char domain[MAX_DOMAIN_LEN];
    
    while ((de = readdir(dr)) != NULL){
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0 || strcmp(de->d_name, ".DS_Store") == 0)
        {
            continue;
        }
        
        memset(buf, 0, MAX_FILE_NAME_LEN);
        memset(domain, 0, MAX_DOMAIN_LEN);
        
        if (mes_queue == NULL)
        {
            mes_queue = (struct file_list *)malloc(sizeof(struct file_list));
            
            
            strcpy(buf, new_maildir_name);
            strcat(buf, de->d_name);
            
            strcpy(mes_queue->file_name,cur_maildir_name);
            strcat(mes_queue->file_name, de->d_name);
            file_rename(buf, mes_queue->file_name);
            
            get_domain(mes_queue->file_name, domain);
            strcpy(mes_queue->domain,domain);
            
            mes_queue->next = NULL;
            next = mes_queue;
            continue;
        }
        if (next->next == NULL)
        {
            struct file_list *file_item;
            file_item = (struct file_list *)malloc(sizeof(struct file_list));
            
            strcpy(buf, new_maildir_name);
            strcat(buf, de->d_name);
   
            strcpy(file_item->file_name,cur_maildir_name);
            strcat(file_item->file_name, de->d_name);
            file_rename(buf, file_item->file_name);
            
            get_domain(file_item->file_name, domain);
            strcpy(file_item->domain,domain);
            
            file_item->next = NULL;
            next->next = file_item;
            next = next->next;
            continue;
        }
        
    }
    
    closedir(dr);

//    next = mes_queue;
//    while (next != NULL) {
//        next = next->next;
//        printf("%s",mes_queue->file_name);
//        printf("   -    %s \n",mes_queue->domain);
//        mes_queue = next;
//    }
    return mes_queue;
}


int get_domain(char* filename, char* domain){
    char tmp[BUFFER_SIZE];
    char *pointer;
    unsigned long int len;
    FILE* file = fopen(filename,"r");
    if (!file) {
        return -1;
    }
    while (fgets(tmp, BUFFER_SIZE, file)!=NULL ){
        if (strncmp(tmp, "To: ",4) == 0) {
            pointer = strstr(tmp, "@");
            len = strlen(pointer);
            memset(domain, '\0', len - 2);
            strncpy(domain, pointer + 1, len - 3);
        }
    }
    fclose(file);
    return 0;
}

void file_rename(char* src, char* dest)
{
    if (src && dest)
    {
        rename(src, dest);
    }
}
