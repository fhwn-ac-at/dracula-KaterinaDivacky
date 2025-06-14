#include <stdio.h>
#include <stdlib.h>
#include "game.h"

//Allocate a board and initialize all jump fields to 0
Board *create_board(int rows, int cols)
{
    int size = rows * cols + 1;  //+1 for field 0
    Board *board = malloc(sizeof(Board));
    board->jumps = calloc(size, sizeof(int));  //initialize all to 0
    board->size = size;
    return board;
}

//Add a ladder from start to end (only if valid and no overlap)
void add_ladder(Board *board, int start, int end)
{
    if (start < end && start < board->size - 1 && end < board->size && board->jumps[start] == 0 && board->jumps[end] == 0)
    {
        board->jumps[start] = end - start;
    }
}

//Add a snake from start to end (only if valid and no overlap)
void add_snake(Board *board, int start, int end)
{
    if (start > end && start < board->size - 1 && end < board->size && board->jumps[start] == 0 && board->jumps[end] == 0)
    {
        board->jumps[start] = end - start;
    }
}

//Roll a die with the given number of sides
static int roll_die(int sides)
{
    return (rand() % sides) + 1;
}

//Simulate a single game and return the result
SimulationResult simulate_game(Board *board, GameConfig config, int *snake_hits, int *ladder_hits)
{
    SimulationResult result;
    result.roll_sequence = malloc(config.max_steps * sizeof(int));
    result.max_rolls = config.max_steps;
    result.rolls = 0;

    int position = 0;
    while (position < board->size - 1 && result.rolls < config.max_steps)
    {
        int roll = roll_die(config.die_sides);
        result.roll_sequence[result.rolls++] = roll;

        int next = position + roll;
        if (next < board->size)
        {
            position = next;
        }
        else if (!config.exact_win)
        {
            position = board->size - 1;  //allow overshooting the goal
        }

        //Apply jump (snake or ladder) if present
        if (position < board->size && board->jumps[position] != 0)
        {
            int jump_to = position + board->jumps[position];
            if (jump_to < board->size)
            {
                if (board->jumps[position] < 0 && snake_hits)
                    snake_hits[position]++;
                else if (board->jumps[position] > 0 && ladder_hits)
                    ladder_hits[position]++;
                position = jump_to;
            }
        }
    }
    return result;
}

//Free the memory allocated for the board
void free_board(Board *board)
{
    free(board->jumps);
    free(board);
}
