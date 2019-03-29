#include "game_init.h"


/*
 * This function creates the board for the first time
 *
 * Input: board - a 6x9 array of squares
 *
 */
void initialize_board(square board[NUM_ROWS][NUM_COLUMNS]){

    for (int i =0; i< NUM_ROWS; i++){
        for(int j =0; j < NUM_COLUMNS; j++){
            //creates an obstacle square at positions (0,3), (1,6), (2,4), (3,5), (4,2) and (5,7)
            if((i == 0 && j==3) || (i == 1 && j == 6) || (i ==2 && j ==4)
                    || (i == 3 && j ==5) || (i==4 && j==2) || (i==5 && j==7)){
                board[i][j].type = OBSTACLE;
            } else{
                //creates a normal square otherwise
                board[i][j].type = NORMAL;
            }
            board[i][j].stack = NULL;
        }
    }
}

 /*
 * This function creates players for the first time
 *
 * Input: the array of players to be initialized
 * Output: The number of players of the game
 */
int initialize_players(player players[]){
    //initialise counter variable i
    int i = 0;
    for(; i < 6; i++){
        //intialise player name and color
        char name[80];
        enum color col;
        printf("If you would like to stop inputting players, press enter\n");

        //ask for user input
        printf("What is the name of player %d\n?:", i + 1);
        fgets(name, 79, stdin);
        //name[80] = '\0';

        //confirm that the user has not stopped entering player names
        if(name[0] == '\n'){
            if(i >= 2){
                return i;
            }
            else{
                return 0;
            }
        }

        //ask for user input
        printf("What is the color of this player\n");
        printf("0 = RED, 1 = BLU, 2 = GREEN, 3 = YELLOW, 4 = PINK, 5 = ORANGE\n");
        printf("?:");
        scanf("%d", &col);
        //clear input buffer
        while(getchar() != '\n');

        //check that the value given for the user colour is correct
        while(col > 5 || col < 0){
            printf("This is not a valid colour\nPlease try again\n");
            printf("What is the color of this player\n");
            printf("0 = RED, 1 = BLU, 2 = GREEN, 3 = YELLOW, 4 = PINK, 5 = ORANGE\n");
            printf("?:");
            scanf("%d", &col);
            //clear input buffer
            while(getchar() != '\n');
        }

        //if the name and color are valid,
        //append them to the array as a ney player
        strcpy(players[i].name, name);
        players[i].col = col;
    }
    return i;
}
