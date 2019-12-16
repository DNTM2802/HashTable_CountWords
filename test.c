#include <stdio.h>
#include <stdlib.h>

typedef struct file_data{
    // public data
    long word_pos;    // zero-based
    long word_num;    // zero-based
    char word[64];
    // private data
    FILE *fp;
    long current_pos; // zero-based
}
file_data_t;

//Representa cada palavra distinta num determinado ficheiro
typedef struct word{
    struct word* next;
    char word[64];
    int first_location;
    int last_location;
    int max_dist;
    int min_dist;
    int medium_dist;
    int count;
}
word_t;


typedef struct hash_table{
    unsigned int size;
    unsigned int count;
    word_t *table[2000];
}
hash_table_t;
 

int open_text_file(char *file_name,file_data_t *fd){
    fd->fp = fopen(file_name,"rb");
    if(fd->fp == NULL)
        return -1;
    fd->word_pos = -1;
    fd->word_num = -1;
    fd->word[0] ='\0';
    fd->current_pos = -1;
    return 0;
}

void close_text_file(file_data_t *fd){
    fclose(fd->fp);
    fd->fp = NULL;
}

int read_word(file_data_t *fd){
    int i,c;
    // skip white spaces
    do{
        c = fgetc(fd->fp);
        if(c == EOF)
            return -1;
        fd->current_pos++;
        }
    while(c <= 32);

    // record word
    fd->word_pos = fd->current_pos;
    fd->word_num++;
    fd->word[0] = (char)c;
    for(i = 1;i < (int)sizeof(fd->word) - 1;i++){
        c = fgetc(fd->fp);
        if(c == EOF)break; 
        // end of file
        fd->current_pos++;
        if(c <= 32)
            break; 
        // terminate word
        fd->word[i] = (char)c;
    }
    fd->word[i] ='\0';return 0;
}

unsigned long
hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void main(){
    file_data_t *fl;
    hash_table_t *hash_table;
    int hashcode;
    word_t word;
    word_t *head;
    bool is_in_hash;
    if(open_text_file("Teste.txt",fl)==-1){
        return EXIT_FAILURE;
    }

    while(read_word(fl) != -1){
        hashcode=hash(fl->word) % hash_table->size;
        head=hash_table->table[hashcode];
        while(*head->next != NULL){
            if (*head->next->word == fl->word){
                is_in_hash=true;
                int temp=*head->next->last_location;
                int dist=fl->current_pos-temp;
                *head->next->last_location=fl->current_pos;
                *head->next->count++;
                if (dist>*head->next->max_dist){
                    *head->next->max_dist=dist;
                }else if(dist<*head->next->max_dist){
                    *head->next->min_dist=dist;
                }else{

                }
                *head->next->medium_dist=*head->next->medium_dist+(dist-*head->next->medium)/ *head->next->count++;
            }
            head=head->next;
        }
        if (!is_in_hash){
            word_t new;
            head->next = &new;
            new->next=NULL;
            new->first_location=fl->current_pos;
            new->last_location=fl->current_pos;
            new->max_dist=0;
            new->min_dist=0;
            new->medium_dist=0;
            new->count=1;
        }
        
    }

    

}

