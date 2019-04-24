#include "game_init.h"
#include "game_logic.h"

/*
 *
 */
int main(int argc, char** argv){

    //the board is defined as a 2-Dimensional array of squares
    square board[NUM_ROWS][NUM_COLUMNS];

    //an array containing the players (MAX 6 players)
    player players[6];

    //the number of players
    int numPlayers =0;

    //creates the squares of the board
    initialize_board(board);

    //prints the board
    print_board(board);

    //creates the players
    numPlayers = initialize_players(players);

    if(numPlayers == 0){
        printf("Not enough players entered\n");
        return -1;
    }

    //asks each player to place their tokens
    //on the first column of the board
    place_tokens(board, players, numPlayers);


    //manages the turns of the game and identifies a winner
    int winner = play_game(board, players, numPlayers);

    printf("Congratulations, you won the game %s", players[winner].name);

    return 0;
}
