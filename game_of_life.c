/**
 * @file game_of_life.c
 * @author earendelmir
 * @date 05 March 2021
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// uncomment the following line if you are compiling this on a Windows machine
// #define WINDOWS
#ifdef WINDOWS
#include <windows.h>
#define CLEAR_SCREEN "cls"
#else
#include <unistd.h>
#define CLEAR_SCREEN "clear"
#endif

#define ROWS 30
#define COLS 40
#define NUM_CELLS ROWS *COLS
#define ALIVE_CHAR '#'
#define DEAD_CHAR ' '
// the following is used to stop the simulation after a fixed amount of runs
#define NUM_GENERATIONS 50
// the following represents the percentage of probability
// any cell has to be alive when it is first created
#define INITIAL_LIFE_FACTOR 2


enum Statuses { alive, dead };

struct Cell {
  short int x, y;
  enum Statuses current_state;
  enum Statuses future_state;
};


struct Cell *init_board();
void print_board(const struct Cell *board);
void next_generation(struct Cell *board);
void cell_future_state(struct Cell *board, struct Cell *cell);
bool is_alive(const struct Cell *board, short int x, short int y);
const struct Cell *find_cell(const struct Cell *board, short int x, short int y);
bool population_dead(const struct Cell *board);
void SLEEP(unsigned int seconds);



int main() {
  srand(time(NULL));
  struct Cell *board = init_board();

  for (unsigned int generation = 0; generation < NUM_GENERATIONS; generation++) {
    print_board(board);
    if (population_dead(board))
      break;
    next_generation(board);
    // uncomment the following function call if you want to wait
    // a certain amount of seconds before updating the screen.
    SLEEP(1);
  }

  printf("\n");
  return (EXIT_SUCCESS);
}



struct Cell *init_board() {
  struct Cell *board = (struct Cell *)malloc(NUM_CELLS * sizeof(struct Cell));
  short int x = 0, y = 0;

  for (unsigned int n = 0; n < NUM_CELLS; n++) {
    board[n].x = x;
    board[n].y = y;
    x = (x + 1) % COLS;
    // if x reaches 0 again it means it belongs to the next row
    if (x == 0)
      y++;
    // decides wether to let the current cell be alive or not
    board[n].current_state = ((rand() % 100) < INITIAL_LIFE_FACTOR) ? alive : dead;
  }

  return board;
}



void print_board(const struct Cell *board) {
  system(CLEAR_SCREEN);

  for (unsigned int i = 0; i < NUM_CELLS; i++) {
    if (board[i].x == 0)
      printf("\n");
    printf("%c ", (board[i].current_state == alive) ? ALIVE_CHAR : DEAD_CHAR);
  }
}



void next_generation(struct Cell *board) {
  // determines each cell's future state
  for (unsigned int i = 0; i < NUM_CELLS; i++)
    cell_future_state(board, &board[i]);

  // it then updates the whole board
  for (unsigned int i = 0; i < NUM_CELLS; i++)
    board[i].current_state = board[i].future_state;
}



// this function calculates the state of a cell for the next generation.
// It does not change its value right away because, in doing so, the
// following cell would be updated considering a "different" board
void cell_future_state(struct Cell *board, struct Cell *cell) {
  unsigned short int alive_neighbours = 0;

  // checks all the 8 different neighbours with coordinates (x+i, y+j)
  // i and j will both get the following values: -1, 0, 1
  for (short int i = -1; i < 2; i++)
    for (short int j = -1; j < 2; j++) {
      // it shouldn't check the cell itself which is at (x+0, y+0) coordinates
      if (i == 0 && j == 0)
        continue;
      // can't operate check on out of bounds coordinates
      if (cell->x + i < 0 || cell->x + i > COLS - 1)
        continue;
      if (cell->y + j < 0 || cell->y + j > ROWS - 1)
        continue;
      // the function called will return 1 if the neighbour is alive, 0 otherwise
      else
        alive_neighbours += is_alive(board, cell->x + i, cell->y + j);
    }

  // the following are the game rules i found online.
  // you can probably tweak them as you please and get differrent results.
  // the minimum range of alive neighbours must be 1 and the maximum is 8.
  if (cell->current_state == alive) {
    // if a living cell has less than 2 alive neighbours it dies by isolation.
    // if a living cell has more than 3 alive neighbours it dies by overpopulation.
    if (alive_neighbours < 2 || alive_neighbours > 3)
      cell->future_state = dead;
  }
  // if the cell is currently dead it comes back to life
  // with 3 and only 3 alive neighbours.
  else if (alive_neighbours == 3)
    cell->future_state = alive;
  // in any other case the cell preserves its current state
  else
    cell->future_state = cell->current_state;
}



bool is_alive(const struct Cell *board, short int x, short int y) {
  return (find_cell(board, x, y)->current_state == alive);
}



// finds and returns a cell from the board, given its x and y coordinates
const struct Cell *find_cell(const struct Cell *board, short int x, short int y) {
  for (unsigned int i = 0; i < NUM_CELLS; i++)
    if (board[i].x == x && board[i].y == y)
      return &board[i];
  return NULL;
}



bool population_dead(const struct Cell *board) {
  for (unsigned int i = 0; i < NUM_CELLS; i++)
    if (board[i].current_state == alive)
      return false;
  return true;
}



void SLEEP(unsigned int seconds) {
#ifdef WINDOWS
  // the argument is in milliseconds
  Sleep(seconds * 1000);
#else
  sleep(seconds);
#endif
}
