#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"

#define DEFAULT_SIMULATIONS 10000  //Default number of simulations if not provided via CLI

#define BOLD "\033[1m"
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define BOLDRED "\033[1;31m"
#define BOLDGREEN "\033[1;32m"
#define BOLDBLUE "\033[1;34m"

//Parse command line arguments into GameConfig and simulation count
int parse_args(int argc, char *argv[], GameConfig *config, int *simulations)
{
    config->rows = 10;
    config->cols = 10;
    config->die_sides = 6;
    config->max_steps = 1000;
    config->exact_win = 1;
    *simulations = DEFAULT_SIMULATIONS;

    //Parse command line options
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-r") == 0 && i + 1 < argc)
            config->rows = atoi(argv[++i]);
        else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc)
            config->cols = atoi(argv[++i]);
        else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc)
            config->die_sides = atoi(argv[++i]);
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc)
            *simulations = atoi(argv[++i]);
        else if (strcmp(argv[i], "--no-exact") == 0)
            config->exact_win = 0;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));  //Seed random number generator

    GameConfig config;
    int simulations;
    parse_args(argc, argv, &config, &simulations);  //Parse arguments

    printf("Arguments are being parsed...\n");
    printf(BOLD "Parsed arguments:\n" RESET);
    printf(BOLD "  Board size:" RESET " %d\n", config.rows * config.cols);
    printf(BOLD "  Die sides:" RESET " %d\n", config.die_sides);
    printf(BOLD "  Max steps:" RESET " %d\n", config.max_steps);
    printf(BOLD "  Simulations:" RESET " %d\n\n", simulations);

    //Create the board and add ladders/snakes
    printf("Jumps are being added as Snakes and Ladders...\n");
    Board *board = create_board(config.rows, config.cols);
    add_ladder(board, 3, 22);
    add_ladder(board, 20, 38);
    add_snake(board, 95, 75);
    add_snake(board, 87, 24);

    //Initialize statistics
    int total_rolls = 0;
    int min_rolls = config.max_steps;
    int *best_sequence = malloc(config.max_steps * sizeof(int));

    int snake_hits[board->size];
    int ladder_hits[board->size];
    memset(snake_hits, 0, sizeof(snake_hits));
    memset(ladder_hits, 0, sizeof(ladder_hits));

    //Run multiple simulations
    for (int i = 0; i < simulations; ++i)
    {
        SimulationResult result = simulate_game(board, config, snake_hits, ladder_hits);
        total_rolls += result.rolls;

        //Track the shortest win
        if (result.rolls < min_rolls)
        {
            min_rolls = result.rolls;
            memcpy(best_sequence, result.roll_sequence, result.rolls * sizeof(int));
        }
        free(result.roll_sequence);
    }

    //Print statistics
    printf(BOLDBLUE "=== 🎲 Dice Roll Statistics ===\n" RESET);
    printf("Average rolls to win the game: %.2f\n", (float)total_rolls / simulations);
    printf("Shortest win: %d rolls\n", min_rolls);
    printf("Sequence: ");
    for (int i = 0; i < min_rolls; ++i) printf("%d ", best_sequence[i]);
    printf("\n\n");

    printf(BOLDBLUE "=== Snakes and Ladders Usage Statistics ===\n\n" RESET);

    //Snake usage summary
    printf("--" BOLDRED " 🐍 Snake usage " RESET "--\n");
    for (int i = 0; i < board->size; ++i)
        if (snake_hits[i])
            printf("Snake at field %d used %d times\n", i, snake_hits[i]);

    //Ladder usage summary
    printf("\n--" BOLDGREEN " 🪜 Ladder usage " RESET "--\n");
    for (int i = 0; i < board->size; ++i)
        if (ladder_hits[i])
            printf("Ladder at field %d used %d times\n", i, ladder_hits[i]);

    //Free memory
    free(best_sequence);
    free_board(board);
    return 0;
}
