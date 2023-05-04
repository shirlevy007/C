
#include "linked_list.h"
#include "markov_chain.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool
#include <string.h>

#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate"\
            "new memory\n"
#define DOT '.'
#define SPACE ' '
#define MAX_WORDS_LENGTH 20
#define MIN_WORDS_LENGTH 2
#define MAX_WORDSIZE 100

/**
 *
 */
int is_final (char *word)
{
  if ((word[strlen (word) - 1] == DOT) || word[strlen (word) - 1] == '\n')
  {
    return 1;
  }
  return 0;
}

int validate_length (int length)
{
  if (length < MIN_WORDS_LENGTH)
  {
    return MIN_WORDS_LENGTH;
  }
  if (length > MAX_WORDS_LENGTH)
  {
    return MAX_WORDS_LENGTH;
  }
  return length;
}

void new_markov_node (MarkovNode *new, char *data)
{
  new->data = malloc (sizeof (char) * MAX_WORDSIZE);
  strcpy (new->data, data);
  new->counter_list = NULL;
  new->nexts_size = 0;
}

MarkovChain *new_markov_chain ()
{
  MarkovChain *markov_chain = malloc (sizeof (MarkovChain));
  if (markov_chain == NULL)
  {
    fprintf (stdout, "%s", ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  markov_chain->database = malloc (sizeof (LinkedList));
  if (markov_chain->database == NULL)
  {
    fprintf (stdout, "%s", ALLOCATION_ERROR_MASSAGE);
    free (markov_chain);
    return NULL;
  }
  markov_chain->database->first = NULL;
  markov_chain->database->last = NULL;
  markov_chain->database->size = 0;
  return markov_chain;

}

/**
* Get random number between 0 and max_number [0, max_number).
* @param max_number maximal number to return (not including)
* @return Random number
*/
int get_random_number (int max_number)
{
  return rand () % max_number;
}

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  int final = 1;
  int num = 0; /// final: finishes with a dot
  Node *current;
  while (final == 1)
  {
    current = markov_chain->database->first;
    num = get_random_number (markov_chain->database->size);
    for (int i = 0; i < num; i++)
    {
      current = current->next;
    }
    char *word = current->data->data;
    final = is_final (word);
  }
  return current->data;
}

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  int num = get_random_number (state_struct_ptr->nexts_size);
  NextNodeCounter *current = state_struct_ptr->counter_list;
//  if (current != NULL)
//  {
  int i = current->frequency;
  while (i <= num)
  {
    current++;
    i = i + current->frequency;
  }
  return current->markov_node;
//  }
  return NULL;
}

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  if (first_node == NULL)
  {
    first_node = get_first_random_node (markov_chain);
  }
  max_length = validate_length (max_length);
  int i = 0;
  MarkovNode *current = first_node;
  while (i < MAX_WORDS_LENGTH && is_final (current->data) == 0)
  {
    if ( i == 0)
    {
      fprintf (stdout, "%s", current->data);
    }
    else
    {
      fprintf (stdout, " %s", current->data);
    }
    current = get_next_random_node (current);
    i++;
  }
  if (is_final (current->data) == 0)
  {
    fprintf (stdout, " %s\n", current->data);
  }
  else
  {
    fprintf (stdout, " %s\n", current->data);
  }
}

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_markov_chain (MarkovChain **ptr_chain)
{
  Node *current = (*ptr_chain)->database->first;
  Node *prev;
  int chain_size = (*ptr_chain)->database->size;
  for (int i = 0; i < chain_size; i++)
  {
    free (current->data->data);
    free (current->data->counter_list);
//    free (current->data->nexts_size);
    prev = current;
    current = current->next;
    free (prev->data);
    free (prev);
  }
}


/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node
 * @param second_node
 * @param markov_chain
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool
add_node_to_counter_list (MarkovNode *first_node, MarkovNode *second_node)
{
  NextNodeCounter *current;
  if (first_node->counter_list == NULL)
  {
    first_node->counter_list = malloc (sizeof (NextNodeCounter));
    if (first_node->counter_list == NULL)
    {
      fprintf (stdout, "%s", ALLOCATION_ERROR_MASSAGE);
      return false;
    }
    current = first_node->counter_list;
    current->markov_node = second_node;
    current->frequency = 1;
    (first_node->nexts_size)++;
    return true;
  }
  current = first_node->counter_list;
  int freq;
  int j = 0;
  for (int i = 0; i < first_node->nexts_size; i = i + freq)
  {
    if (strcmp (first_node->counter_list[j].markov_node->data,
                second_node->data) == 0)
    {
      (first_node->counter_list[j].frequency)++;
      (first_node->nexts_size)++;
      return true;
    }
    freq = first_node->counter_list[j].frequency;
    j++;
  }
  ///to check if curr maloc or first or whatever. first->list==null or cur==nul
  current = (NextNodeCounter *) realloc (first_node->counter_list, \
  (sizeof (NextNodeCounter) * (j + 1)));
  if (current == NULL)
  {
    fprintf (stdout, "%s", ALLOCATION_ERROR_MASSAGE);
    return false;
  }
  first_node->counter_list = current;
  first_node->counter_list[j].markov_node =
      second_node;
  first_node->counter_list[j].frequency = 1;
  (first_node->nexts_size)++;
  return true;
}

/**
* Check if data_ptr is in database. If so, return the node wrapping it in
 * the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */

Node *get_node_from_database (MarkovChain *markov_chain, char *data_ptr)
{
  Node *current = markov_chain->database->first;
  for (int i = 0; i < markov_chain->database->size; i++)
  {
    if (strcmp (current->data->data, data_ptr) == 0)
    {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

/**
* If data_ptr in markov_chain, return it's node. Otherwise, create new
 * markov_node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node *add_to_database (MarkovChain *markov_chain, char *data_ptr)
{
  Node *current = get_node_from_database (markov_chain, data_ptr);
  if (current != NULL)
  {
    return current;
  }
  MarkovNode *new = malloc (sizeof (MarkovNode));
  if (new == NULL)
  {
    fprintf (stdout, "%s", ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  new_markov_node (new, data_ptr);
  if (add (markov_chain->database, new) == EXIT_FAILURE)
  {
    return NULL;
  }
  return markov_chain->database->last;
}