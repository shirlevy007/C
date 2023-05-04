#include "linked_list.h"
#include "markov_chain.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define WRONG_ARC "Usage: The program receives 3 or 4 arguments only.\n"
#define WRONG_INPUT_FILE "ERROR: The given file is invalid.\n"
#define TWEET_PRINT "Tweet %d:"
#define MAX_ARGS 5
#define MIN_ARGS 4
#define ARG_IN 3
#define ARG_TWEETS_NUM 2
#define ARG_SEED 1
#define WORDSIZE 100
#define LINESIZE 1200
#define SPACE " \n"
#define DOT '.'
#define MAX_WORDS_LENGTH 20
#define MIN_WORDS_LENGTH 2
#define BASE_10 10

Node * last = NULL;

/**
 *
 * @param argc num of args
 * @return fail if wrong
 */
static int valid_argc (int argc, char * argv[], int * num_of_words)
{
  if (argc != MAX_ARGS && argc != MIN_ARGS)
  {
    fprintf (stdout, WRONG_ARC);
    return EXIT_FAILURE;
  }
  if (argc == MAX_ARGS) ///gets num_of_words if we got it
  {
    long int in_num_of_words = strtol (argv[MAX_ARGS - 1], NULL, BASE_10);
    *num_of_words = (int) in_num_of_words; ///gets num_of_words assuming is OK
  }
  return EXIT_SUCCESS;
}

/**
 *
 * @param in_file pointer for input file
 * @return fail if wrong \ doesn't open
 */
static int valid_in_file (FILE *in_file)
{
  if (in_file == NULL)
  {
    fprintf (stdout, WRONG_INPUT_FILE);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/**
 *
 * @param word
 * @return 1 if ends in DOT, 0 else
 */
static int is_final (char *word)
{
  if ((word[strlen (word) - 1] == DOT) || word[strlen (word) - 1] == '\n')
  {
    return 1;
  }
  return 0;
}

int str_comp(void * data1, void * data2)
{
  char * str1 = data1;
  char * str2 = data2;
  return strcmp (str1, str2);
}

void * str_copy(void * data)
{
  char * source = data;
  char * dest = malloc (strlen(source)+1);
  if (dest == NULL)
  {
    fprintf (stdout, "%s", ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  strcpy (dest, source);
  return dest;
}

void str_print(void * data)
{
  char * str = data;
  fprintf (stdout, " %s", str);
//  if (!is_final (str))
//  {
//    fprintf (stdout, " ");
//  }
}


void str_free (void * data)
{
  char * str = data;
  free (str);
}

bool str_last (void * data)
{
  char * str = data;
  if (is_final (str))
  {
    return true;
  }
  return false;
}

static void str_markov_chain(MarkovChain * markov_chain)
{
  markov_chain->copy_func = str_copy;
  markov_chain->free_data = str_free;
  markov_chain->print_func = str_print;
  markov_chain->comp_func = str_comp;
  markov_chain->is_last = str_last;
}

/**
 * fills in the MarcovChain database
 * @param fp
 * @param read_max
 * @param markov_chain
 * @return 1 if fails, 0 else
 */
static int fill_database (FILE *fp, int read_max, MarkovChain *markov_chain)
{
  char line[LINESIZE];
  char *word = NULL, *next = NULL;
  int i = 0; ///counter for words to get to words_to_read
  Node *word_node = NULL, *next_node = NULL;
  while (fgets (line, sizeof (line), fp) != NULL && i != read_max)
  {
    word = strtok (line, SPACE);
    if (word == NULL)
    {
      return EXIT_FAILURE;
    }
    word_node = add_to_database (markov_chain, word);
    last = word_node;
    if (word_node == NULL)
    {
      return EXIT_FAILURE;
    }
    i++;
    next = strtok (NULL, SPACE);
    while (next != NULL && (i) != read_max)
    {
      next_node = add_to_database (markov_chain, next);
      last = next_node;
      if (next_node == NULL)
      {
        return EXIT_FAILURE;
      }
      i++;
      if (is_final (word) == 0)
      {
        if (add_node_to_counter_list (word_node->data, next_node->data,
                                      markov_chain) == false)
        {
          return EXIT_FAILURE;
        }
      }
      word = next;
      word_node = next_node;
      next = strtok (NULL, SPACE);
    }
  }
  return EXIT_SUCCESS;
}


int main (int argc, char *argv[])
{
  int num_of_words = -1;
  if (valid_argc (argc, argv, &num_of_words) == EXIT_FAILURE) ///check args num
  {
    return EXIT_FAILURE;
  }
  unsigned int seed = strtol (argv[ARG_SEED], NULL, BASE_10);
  int num_of_tweets = (int) strtol (argv[ARG_TWEETS_NUM], \
  NULL, BASE_10);   ///gets num assuming is OK
  FILE *in_file = fopen (argv[ARG_IN], "r");
  if (valid_in_file (in_file)) ///checks validity of file path
  {
    return EXIT_FAILURE;
  } ///finished all input checks
  MarkovChain *markov_chain = new_markov_chain ();
  if (markov_chain == NULL)
  {
    fclose (in_file);
    return EXIT_FAILURE;
  }
  str_markov_chain (markov_chain);
  if (fill_database (in_file, num_of_words, markov_chain))
  {
    fclose (in_file);
    free_markov_chain (&markov_chain);
//    free (markov_chain);
    return EXIT_FAILURE;
  }
  fclose (in_file);
//  fprintf (stdout, "%s", (char *)(last->data->data));
  srand (seed);
  for (int i = 1; i <= num_of_tweets; ++i)
  {
    fprintf (stdout, TWEET_PRINT, i);
    MarkovNode *first_random = get_first_random_node (markov_chain);
    while (is_final (first_random->data))
    {
      first_random = get_first_random_node (markov_chain);
    }
    generate_random_sequence (markov_chain, first_random, MAX_WORDS_LENGTH);
    fprintf (stdout, "\n");
  }
  free_markov_chain (&markov_chain);
//  free (markov_chain);
  return EXIT_SUCCESS;
}
