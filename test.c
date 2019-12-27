
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
  struct word *next;
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
  word_t *prev;
  head = (word_t *) malloc (sizeof (word_t));
  prev = (word_t *) malloc (sizeof (word_t));

  int hashcode;
  int word_counter;
  while (read_word (fl) != -1)
    {
      //DYNAMIC RESIZE 
      if (hash_table->count >= hash_table->size / 2)
	{
	  word_t **table, *curr, *next;
	  size_t i, k;
	  next = (word_t *) malloc (sizeof (word_t));
	  curr = (word_t *) malloc (sizeof (word_t));
	  int new_size = hash_table->size * 2;
	  table = malloc (new_size * sizeof (word_t *));
	  if (!table)
	    return -1;		/* Out of memory */

	  /* Initialize new entry array to empty. */
	  for (i = 0; i < new_size; i++)
	    {
	      table[i] = NULL;
	    }
	  for (i = 0; i < hash_table->size; i++)
	    {
	      /* Detach the singly-linked list. */

	      next = hash_table->table[i];

	      while (next)
		{
		  /* Detach the next element, as 'curr' */
		  curr = next;
		  next = next->next;

		  /* k is the index to this hash in the new array */
		  k = curr->hash % new_size;

		  /* Prepend to the list in the new array */
		  curr->next = hash_table->table[k];
		  hash_table->table[k] = curr;
		}
	    }
	  free (next);
	  free (curr);
	  /* Old array is no longer needed, */
	  //free(hash_table->table);
	  /* so replace it with the new one. */
	  hash_table->table = table;
	  hash_table->size = new_size;
	  printf ("Dei resize\n");

	}

      int flag = 0;
      word_counter++;
      hashcode = hash (fl->word) % hash_table->size;
      head = hash_table->table[hashcode];
      if (head == NULL)
	{
	  word_t *new;
	  new = (word_t *) malloc (sizeof (word_t));
	  new->next = NULL;
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
	}
      else
	{
	  while (head != NULL)
	    {
	        if (strcmp (head->word, fl->word) == 0)
		    {
		        flag = 1;
		        int temp = head->last_location;
		        int dist = fl->current_pos - temp;
		        head->last_location = fl->current_pos;
		        if (dist > head->max_dist)
		        {
		            head->max_dist = dist;
                }
                if (dist < head->min_dist)
		        {
		            head->min_dist = dist;
		        }	      
                head->medium_dist = head->medium_dist + (dist - head->medium_dist) / head->count;
                head->count++;
		        break;		
		    }
	    prev = head;
	    head = head->next;

	    }
	  if (flag == 0)
	    {
	      word_t *new;
	      new = (word_t *) malloc (sizeof (word_t));
	      new->next = NULL;
	      new->hash = hash (fl->word);
	      new->first_location = fl->current_pos;
	      new->last_location = fl->current_pos;
	      new->max_dist = 0;
	      new->min_dist = 999999999;
	      new->medium_dist = 0;
	      new->count = 1;
	      strcpy (new->word, fl->word);
	      new->hash = hashcode;
	      prev->next = new;
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
	  while (word->next != NULL)
	    {
	      printf ("%s (Hash: %d First Loc: %d Max Dist: %d Min Dist: %d Medium Dist: %d Count: %d) \n ", word->word,word->hash,word->first_location,word->max_dist,word->min_dist,word->medium_dist,word->count);
	      word = word->next;
	    }
	  printf ("%s (Hash: %d First Loc: %d Max Dist: %d Min Dist: %d Medium Dist: %d Count: %d) \n ", word->word,word->hash,word->first_location,word->max_dist,word->min_dist,word->medium_dist,word->count);
	}
    printf("==================\n");
    }
  printf ("Words read: %d\n", word_counter);
  printf ("Hash elements count: %d\n", hash_table->count);
  printf ("Hash elements size: %d\n", hash_table->size);

  return 0;

}
