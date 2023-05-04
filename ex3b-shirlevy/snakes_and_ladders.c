#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define MAX_LEN 60


#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

#define WRONG_ARC "Usage: The program receives 2 arguments only.\n"
#define WRONG_SEED "ERROR: The given seed is invalid.\n"
#define WRONG_TRAILS_NUM "ERROR: The given trails number is invalid.\n"
#define TRAIL_PRINT "Random Walk %d: "
#define TRAIL_NUM "[%d]"
#define LADDER_TO "-ladder to %d"
#define SNAKE_TO "-snake to %d"
#define ARROW " -> "
#define SPACE " "



#define ARG_SEED 1
#define ARG_TRAILS_NUM 2
#define MAX_ARGS 3
#define BASE_10 10
#define LAST_CELL 100
#define FIRST_CELL 1



/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in case
    int snake_to;  // snake_to represents the jump of the snake in case
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_markov_chain(database);
    }
    return EXIT_FAILURE;
}


static int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE,NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        }
        else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain *markov_chain)
{
    Cell* cells[BOARD_SIZE];
    if(create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        add_to_database(markov_chain, cells[i]);
    }

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node = get_node_from_database(markov_chain,cells[i])->data;

        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to,cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain, cells[index_to])
                    ->data;
            add_node_to_counter_list(from_node, to_node, markov_chain);
        }
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell*) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database(markov_chain, cells[index_to])
                        ->data;
                add_node_to_counter_list(from_node, to_node, markov_chain);
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}


int cell_comp(void * data1, void * data2)
{
  Cell * cell1 = data1;
  Cell * cell2 = data2;
  return cell1->number - cell2->number;
}

void * cell_copy(void * data)
{
  Cell * source = data;
  Cell * dest = malloc (sizeof (Cell));
  if (dest == NULL)
  {
    fprintf (stdout, "%s", ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  dest->number = source->number;
  dest->ladder_to = source->ladder_to;
  dest->snake_to = source->snake_to;
  return dest;
}

bool cell_last (void * data)
{
  Cell * cell = data;
  if (LAST_CELL == cell->number)
  {
    return true;
  }
  return false;
}

void cell_print(void * data)
{
  Cell * cell = data;
  fprintf (stdout, TRAIL_NUM, cell->number);
  if (cell->ladder_to != EMPTY)
  {
    fprintf (stdout, LADDER_TO, cell->ladder_to);
  }
  if (cell->snake_to != EMPTY)
  {
    fprintf (stdout, SNAKE_TO, cell->snake_to);
  }
  if (!cell_last (cell))
  {
    fprintf (stdout, ARROW);
  }
}

void cell_free (void * data)
{

  Cell * cell =  data;
//  free (&(cell->ladder_to));
//  free (&(cell->snake_to));
//  free (&(cell->number));
  free (cell);

}


static void cell_markov_chain(MarkovChain * markov_chain)
{
  markov_chain->copy_func = cell_copy;
  markov_chain->free_data = cell_free;
  markov_chain->print_func = cell_print;
  markov_chain->comp_func = cell_comp;
  markov_chain->is_last = cell_last;
}

static int valid_argc (int argc)
{
  if (argc != MAX_ARGS)
  {
    fprintf (stdout, WRONG_ARC);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
  if (valid_argc (argc) == EXIT_FAILURE) ///check args num
  {
    return EXIT_FAILURE;
  }
  unsigned int seed = strtol (argv[ARG_SEED], NULL, BASE_10);
  int num_of_trails = (int) strtol (argv[ARG_TRAILS_NUM], \
  NULL, BASE_10);   ///gets num assuming is OK TODO: unsigned?
  MarkovChain * markov_chain = new_markov_chain ();
  if (markov_chain == NULL)
  {
    return EXIT_FAILURE;
  }
  cell_markov_chain (markov_chain);
  if (fill_database (markov_chain))
  {
    free_markov_chain (&markov_chain);
    free (markov_chain);
    return EXIT_FAILURE;
  }
  srand (seed);
  for (int i = 1; i <= num_of_trails; ++i)
  {
    fprintf (stdout, TRAIL_PRINT, i);
    MarkovNode * first = markov_chain->database->first->data;
    generate_random_sequence (markov_chain, first, MAX_LEN);
    fprintf (stdout, "\n");
  }
  free_markov_chain (&markov_chain);
//  free (markov_chain);
  return EXIT_SUCCESS;
}

