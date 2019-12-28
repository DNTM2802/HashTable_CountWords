
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct file_data
{
  // public data
  long word_pos;		// zero-based
  long word_num;		// zero-based
  char word[64];
  // private data
  FILE *fp;
  long current_pos;		// zero-based
}
file_data_t;

//Representa cada palavra distinta num determinado ficheiro
typedef struct word
{
  struct word *left;
  struct word *right;
  char word[64];
  int hash;
  int first_location;
  int last_location;
  int max_dist;
  int min_dist;
  int medium_dist;
  int count;
}
word_t;


typedef struct hash_table
{
  unsigned int size;
  unsigned int count;
  word_t **table;
}
hash_table_t;


int
open_text_file (char *file_name, file_data_t * fd)
{
  fd->fp = fopen (file_name, "r");	// ---->>> Erro C) aqui
  if (fd->fp == NULL)
    return -1;
  fd->word_pos = -1;
  fd->word_num = -1;;
  fd->word[0] = '\0';
  fd->current_pos = -1;
  return 0;
}

void
close_text_file (file_data_t * fd)
{
  fclose (fd->fp);
  fd->fp = NULL;
}

int
read_word (file_data_t * fd)
{
  int i, c;
  // skip white spaces
  do
    {
      c = fgetc (fd->fp);
      if (c == EOF)
	return -1;
      fd->current_pos++;

    }
  while (c <= 32);
  //record word
  fd->word_pos = fd->current_pos;
  fd->word_num++;
  fd->word[0] = (char) c;
  for (i = 1; i < (int) sizeof (fd->word) - 1; i++)
    {
      c = fgetc (fd->fp);
      if (c == EOF)
	break;
      // end of file
      fd->current_pos++;
      if (c <= 32)
	break;
      // terminate word
      fd->word[i] = (char) c;
    }
  fd->word[i] = '\0';
  return 0;
}

unsigned long
hash (unsigned char *str)
{
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c;	/* hash * 33 + c */

  return hash;
}

void visit(word_t *word){
  printf ("%s (Hash: %d First Loc: %d Max Dist: %d Min Dist: %d Medium Dist: %d Count: %d)\n ", word->word,word->hash,word->first_location,word->max_dist,word->min_dist,word->medium_dist,word->count);
  }

word_t *search_recursive(word_t *link,char *data){
  if(link == NULL || strcmp(link->word,data)==0)
    return link;
  if(strcmp(link->word,data)<0)
    return search_recursive(link->left,data);
  else
    return search_recursive(link->right,data);
  }

void insert_non_recursive(word_t **link, word_t **insert,char *data){
  word_t *parent = NULL;
  while(*link != NULL){
    parent = *link;
    link = (strcmp((*link)->word,data)<0) ? &((*link)->left) : &((*link)->right); 
    }
    *link = *insert;
  }

void traverse_in_order_recursive(word_t *link){
  if(link != NULL){
    traverse_in_order_recursive(link->left);
    visit(link);
    traverse_in_order_recursive(link->right);
    }
  }

int
main (int argc, char *argv[])
{
  file_data_t *fl;
  fl = (file_data_t *) malloc (sizeof (file_data_t));

  if (open_text_file ("Teste.txt", fl) == -1)
    {
      return EXIT_FAILURE;
    }

  // HASHTABLE INITIALIZATION
  hash_table_t *hash_table = NULL;
  hash_table = malloc (sizeof (hash_table_t));
  hash_table->table = malloc (2000 * sizeof (word_t *));
  hash_table->size = 2000;
  hash_table->count = 0;
  for (int i = 0; i < 2000; i++)
    {
      hash_table->table[i] = NULL;
    }

  // HEAD DECLARATION TO USE INSIDE WHILE CYCLE
  word_t *head;
  head = (word_t *) malloc (sizeof (word_t));
  long int counterrr;
  int hashcode;
  int word_counter;
  while (read_word (fl) != -1)
    {
      
      word_counter++;
      hashcode = hash (fl->word) % hash_table->size;
      head = hash_table->table[hashcode];
    if (head == NULL){
      word_t *new;
      new = (word_t *) malloc (sizeof (word_t));
      new->left = NULL;
      new->right = NULL;
      new->hash = hash (fl->word);
      new->first_location = fl->current_pos;
      new->last_location = fl->current_pos;
      new->max_dist = 0;
      new->min_dist = 999999999;
      new->medium_dist = 0;
      new->count = 1;
      strcpy (new->word, fl->word);
      hash_table->table[hashcode] = new;
      new->hash = hashcode;
      hash_table->count += 1;
      counterrr++;
	  }
    else{        
    word_t *this_one;
    this_one=search_recursive(head,fl->word);
	  if (this_one != NULL){
      counterrr++;
		        int temp = this_one->last_location;
		        int dist = fl->current_pos - temp;
		        this_one->last_location = fl->current_pos;
		        if (dist > this_one->max_dist)
		        {
		            this_one->max_dist = dist;
                }
                if (dist < this_one->min_dist)
		        {
		            this_one->min_dist = dist;
		        }	      
                this_one->medium_dist = this_one->medium_dist + (dist - this_one->medium_dist) / this_one->count;
                this_one->count++;
		    }else{
	      word_t *new;
	      new = (word_t *) malloc (sizeof (word_t));
	      new->left = NULL;
        new->right = NULL;
	      new->hash = hash (fl->word);
	      new->first_location = fl->current_pos;
	      new->last_location = fl->current_pos;
	      new->max_dist = 0;
	      new->min_dist = 999999999;
	      new->medium_dist = 0;
	      new->count = 1;
	      strcpy (new->word, fl->word);
	      new->hash = hashcode;
	      insert_non_recursive(&head,&new,new->word);
        
	      //printf ("Adicionada ao fim: %s\n", new->word);
	    }
	}

    }

  word_t *word;
  for (int k = 0; k < hash_table->size; k++){
      printf("==================\n");
      if (hash_table->table[k] == NULL)
	{
	  printf ("NULL\n");
	}
      else
	{
	  word = hash_table->table[k];
    traverse_in_order_recursive(word);
	}
    printf("==================\n");
  }
  printf ("Words read: %d\n", word_counter);
  printf ("Hash elements count: %d\n", hash_table->count);
  printf ("Hash elements size: %d\n", hash_table->size);
printf("Number of words inside hash table: %ld\n", counterrr);
  return 0;

}
