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
            board[i][j].numTokens = 0;
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
    //initialise counter variables i, j
    int i = 0;
    int j = 0;

    //an int array indicating whether a given color has already been
    //chosen by another user
    int colChosen[6] = {0};

    //a char pointer array storing string literals which contain the
    //stringified names of the colors available
    char *colorNames[] = {"RED", "BLUE", "GREEN", "YELLOW", "PINK", "ORANGE"};
    for(; i < 6; i++){
        //intialise player name and color
        char name[80];
        //variable 'col' of type enum color to store the chosen color of the user
        enum color col;
        //print instructions to console
        printf("If you would like to stop inputting players, press enter\n");

        //ask for user input
        printf("What is the name of player %d\n?:", i + 1);
        //scan in user input
        fgets(name, 79, stdin);

        //confirm that the user has not stopped entering player names
        if(name[0] == '\n'){
            //the minimum number of players allowed is 2
            if(i >= 2){
                return i;
            }
            //if less than 2 players are initialized, indicate that the function has failed
            else{
                return 0;
            }
        }

        //ask for user input
        printf("What is the color of this player\n");

        //loop over available colors and print each to the console
        j = 0;
        while(j < 6){
            //if this color has not been chose, print its name to screen
            //along with the value of that color
            if(colChosen[j] == 0)
                printf("%s = %d\n", colorNames[j], j);
            j++;
        }
        //scan in user input
        printf("?:");
        scanf("%d", &col);

        //clear input buffer
        while(getchar() != '\n');

        //check that the value given for the user colour is in the accepted range
        //and that is has not been chosen yets
        while(col > 5 || col < 0 || colChosen[col] != 0){
            printf("This is not a valid colour\nPlease try again\n");//error message
            //request user input
            printf("What is the color of this player\n");

            //loop over available colors and print each to the console
            j = 0;
            while(j < 6){
                //if this color has not been chose, print its name to screen
                //along with the value of that color
                if(colChosen[j] == 0)
                    printf("%s = %d\n", colorNames[j], j);
                j++;
            }
            //scan in user input
            printf("?:");
            scanf("%d", &col);

            //clear input buffer
            while(getchar() != '\n');
        }

        //if the color chosen is available and a valid color,
        //set colChosen[col] = 1 so that is cannot be chosen again
        //by a different user
        colChosen[col] = 1;

        //if the name and color are valid,
        //append them to the array as a new player
        strcpy(players[i].name, name);
        players[i].col = col;

        //confirmation message
        printf("\nPlayer %d successfully created with colour %s and name %s\n", i+1, colorNames[players[i].col], players[i].name);
    }
    return i;
}
