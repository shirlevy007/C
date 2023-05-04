#include "linked_list.h"
#include "markov_chain.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define WRONG_ARC "USAGE: The program receives 3 or 4 arguments only.\n"
#define WRONG_INPUT_FILE "ERROR: The given file is invalid.\n"
#define MAX_ARGS 5
#define MIN_ARGS 4
#define ARG_IN 3
#define ARG_TWEETS_NUM 2
#define ARG_SEED 1
#define WORDSIZE 100
#define LINESIZE 1200
#define SPACE " \n"
#define DOT "."
#define MAX_WORDS_LENGTH 20
#define MIN_WORDS_LENGTH 2
#define BASE_10 10




// your code goes here

int valid_argc (int argc)
/**
 *
 * @param argc num of args
 * @return fail if wrong
 */
{
  if (argc != MAX_ARGS && argc != MIN_ARGS)
  {
    fprintf (stdout, WRONG_ARC);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
int valid_in_file (FILE *in_file)
/**
 *
 * @param in_file pointer for input file
 * @return fail if wrong \ doesn't open
 */
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
 * @param line from file
 * @param words_to_read max words to read
 * @param i pointer to the counter of words
 * @param markov_chain to update
 * @return SUCCESS if no problems (memory issues), FAILURE else
 */

/**
 * fills in the MarcovChain database
 * @param fp
 * @param words_to_read
 * @param markov_chain
 */
int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  char line[LINESIZE];
  char *word, *next;
  int i = 0; ///counter for words to get to words_to_read
  Node *word_node, *next_node;
  while (fgets (line, sizeof (line), fp) != NULL && i != words_to_read)
  {
    word = strtok (line, SPACE);
    if (word == NULL)
    {
      return EXIT_FAILURE;
    }
    word_node = add_to_database (markov_chain, word);
    if (word_node == NULL)
    {
      return EXIT_FAILURE;
    }
    i++;
    next = strtok (NULL, SPACE);
    while (next != NULL && (i) != words_to_read)
    {
      next_node = add_to_database (markov_chain, next);
      if (next_node == NULL)
      {
        return EXIT_FAILURE;
      }
      i++;
      if (is_final (word) == 0)
      {
        if (add_node_to_counter_list (word_node->data, next_node->data) \
 == false)
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
  if (valid_argc (argc) == EXIT_FAILURE) ///check arfs num
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
  }
  int num_of_words = -1;
  if (argc == MAX_ARGS) ///gets num_of_words if we got it
  {
    long int in_num_of_words = strtol (argv[MAX_ARGS - 1], NULL, BASE_10);
    num_of_words = (int) in_num_of_words; ///gets num_of_words assuming is OK
  }///finished all input checks
  MarkovChain *markov_chain = new_markov_chain ();
  if (markov_chain == NULL)
  {
    fclose (in_file);
    return EXIT_FAILURE;
  }
  if (fill_database (in_file, num_of_words, markov_chain))
  {
    fclose (in_file);
    free_markov_chain (&markov_chain);
    free (markov_chain);
    return EXIT_FAILURE;
  }
  fclose (in_file);
  srand (seed);
  for (int i = 1; i <= num_of_tweets; ++i)
  {
    fprintf (stdout, "Tweet %d: ", i);
    generate_random_sequence (markov_chain, NULL, MAX_WORDS_LENGTH);
  }
  free_markov_chain (&markov_chain);
  free (markov_chain);
}
