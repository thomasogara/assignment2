#include "game_init.h"
#include "game_logic.h"

/*
 * Returns the first letter associated with the color of the token
 *
 * Input: t - pointer to a token
 * Output: initial of the color of the token
 */

char print_token(token *t){
    if(t -> col == PINK) return 'P';
    if(t ->col == RED) return 'R';
    if(t -> col == BLU) return 'B';
    if(t -> col == GREEN) return 'G';
    if(t -> col == ORANGE) return 'O';
    if(t -> col == YELLOW) return 'Y';
    return '\0';
}

/*
 * Prints the board
 *
 * Input: the board to be printed.
 */
void print_board(square board[NUM_ROWS][NUM_COLUMNS]){
    printf("                THE BOARD\n");
    for(int i =0; i < NUM_ROWS; i++){

        //prints an horizontal line
        printLine();
        //prints the row number
        printf(" %d ", i);
        char c = '\0' ;
        //if the square (i,j) is occupied,
        //c is assigned the initial of the color of the token that occupies the square
        for (int j = 0; j < NUM_COLUMNS; j++){
            if(board[i][j].stack != NULL){
                c = print_token(board[i][j].stack);
            }
            //if the square (i,j) is empty
            else{
                //c is assigned 'X' if the square represents an obstacle
                if(board[i][j].type == OBSTACLE)
                    c = 'X';
                //c is assigned an empty space otherwise
                else c = ' ';
            }
            printf("| %c ", c);
        }
        printf ("|\n");
    }
    printLine();
    //prints the number of the columns at the end of the board
    printf("     0   1   2   3   4   5   6   7   8\n");
}

/*
 * Prints a horizontal line of hyphen "-" characters
 * Input: void
 */
void printLine(void){
  printf("   -------------------------------------\n");
}

/*
 * Place tokens in the first column of the board
 *
 * Input: board - a 6x9 array of squares that represents the board
 *        players - the array of the players
 *        numPlayers - the number of players
 */
void place_tokens(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers){
    //the min number of tokens placed on a square in the first coloumn of the board
    int minNumOfTokens = 0;
    int selectedSquare = 0;

    for(int i=0; i <4; i++){

        for(int j=0; j <numPlayers; j++){

            print_board(board); // prints board

            printf("Player %d select a square.\n", j+1);
            scanf("%d", &selectedSquare); // user selects square in first coloumn

            if(selectedSquare > -1 && selectedSquare < 6){ // ensures player selects a square between 0 and 5

                if(board[selectedSquare][0].stack == NULL) // if selected square is empty
                {
                   board[selectedSquare][0].stack = (token *)malloc(sizeof(token)); // allocates memory
                   board[selectedSquare][0].stack->col = players[j].col;
                   board[selectedSquare][0].numTokens++;
                }
                else
                {
                    printf("This square is full.\nPlease select an empty square.\n"); // error messaage

                }

            }
            else{
                printf("\n\nPlease select a square between 0-5.\n"); // error message if user selects square outside of range
            }


            // to be implemented: if the square contains the min number of tokens and
            // does not have a token of the same colour of the player


            // updates the minimum number of Tokens
            if(((numPlayers * i) + j + 1)%NUM_ROWS ==0)
                minNumOfTokens++;

            //print_board(board); // prints board after each token is placed
        }
    }

}


/*
 * Place tokens in the first column of the board
 *
 * Input: board - a 6x9 array of squares that represents the board
 *        players - the array of the players
 *        numPlayers - the number of players
 */

void play_game(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers){
    srand(time(NULL));//seed the random() function
    //a number of counter variables
    int i = 0;//outer loops
    int j = 0;//inner loops
    int k = 0;//innermost loops
    //variables to store a variety of user inputs
    int row = 0;//row number
    int column = 0;//col number
    /*bool*/int up = 0;//whether the user would like to move a token up/down (1 - up; 0 - down)
    /*bool*/int impassable = 1;//whether an obstacle square can be exited (default 1 - this square cannot be passed)
    int diceRoll = 0;//used to store the result of the user's dice roll
    int choice;//used to store the user choice
    while(players[(i-1) % numPlayers].numTokensLastCol != 3){
        //print user's name and indicate start of turn
        printf("It is your turn to play %s", players[i].name);
        //clear input buffer
        while(getchar()!='\n');
        puts("Please roll the dice by pressing enter");
        //wait for the user to hit enter
        while(getchar()!='\n');
        //simulate a dice roll
        diceRoll = rand()%6;
        //print out the result of the dice roll
        printf("You must move a token from row %d\n", diceRoll);
        //ask if the user would like to move a token up/down
        puts("Would you like to move one of your tokens up/down?");
        puts("(1)Yes\n(0)No");
        scanf("%d", &choice);
        //switch to handle user choice
        switch(choice){
            case 0:
                break;
            case 1:
                //request user input
                puts("Please input the row and column number of the token you would like to move");
                //scan in user input
                scanf("%d", &row);
                scanf("%d", &column);
                //ensure that this token belongs to the user and is not in an obstacle square
                while(board[row][column].stack->col != players[i].col || board[row][column].type == OBSTACLE){
                    puts("That square cannot be selected, please choose another");
                    puts("Please input the row and column number of the token you would like to move");
                    scanf("%d", &row);
                    scanf("%d", &column);
                }
                //request user input
                puts("Would you like to move up or down?\n(1)Up\n(0)Down");
                scanf("%d", &up);
                //esnure 'up' is within the allowed range of 0 - 1
                while(up > 1 || up < 0){
                    puts("That is not a recognised input");//error message
                    puts("Would you like to move up or down?\n(1)Up\n(0)Down");
                    scanf("%d", &up);
                }
                //switch statement to handle user input
                switch(up){
                    case 0:
                        //edge case handling (can't move down from row 5)
                        if(row == 5){
                            puts("You cannot move down from this row");
                            break;
                        }
                        //allocate memory for the square the user would like to move to
                        //if that square is empty
                        if(board[row+1][column].stack == NULL){
                            board[row+1][column].stack = (token*)malloc(sizeof(token));
                        }
                        board[row+1][column].stack->col = board[row][column].stack->col;
                        board[row+1][column].numTokens++;
                        board[row][column].numTokens--;
                        if(board[row][column].numTokens == 0){
                            free(board[row][column].stack);
                            board[row][column].stack = NULL;
                        }
                        print_board(board);
                        break;
                    case 1:
                        if(row == 0){
                            puts("You cannot move up from this row");
                            break;
                        }
                        if(board[row-1][column].stack == NULL){
                            board[row-1][column].stack = (token*)malloc(sizeof(token));
                        }
                        board[row-1][column].stack->col = board[row][column].stack->col;
                        board[row-1][column].numTokens++;
                        board[row][column].numTokens--;
                        if(board[row][column].numTokens == 0){
                            free(board[row][column].stack);
                            board[row][column].stack = NULL;
                        }
                        print_board(board);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        puts("Please enter the column number of the token you would like to move");
        scanf("%d", &column);
        while(board[diceRoll][column].stack == NULL){
            puts("This square is empty");
            puts("Please enter the column number of the token you would like to move");
            scanf("%d", &column);
        }
        j = 0;
        k = 0;
        impassable = 0;
        while(board[diceRoll][column].type == OBSTACLE){
            for(j = 0; j < NUM_ROWS; j++){
                if(impassable){
                    break;
                }
                for(k = 0; k < column; k++){
                    if(board[j][k].stack != NULL){
                        impassable = 1;
                        break;
                    }
                }
            }
            if(impassable){
                break;
            }
            else{
                puts("This obstacle square cannot yet be passed");
                puts("Please enter the column number of the token you would like to move");
                scanf("%d", &column);
            }
        }
        if(board[diceRoll][column+1].stack == NULL){
            board[diceRoll][column+1].stack = (token*)malloc(sizeof(token));
        }
        board[diceRoll][column+1].stack->col = board[diceRoll][column].stack->col;
        free(board[row][column].stack);
        board[diceRoll][column].stack = NULL;
        board[diceRoll][column+1].numTokens++;
        board[diceRoll][column].numTokens--;
        print_board(board);
        ++i;
        i %= numPlayers;
    }
}
