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
// the min number of tokens placed on a square in the first coloumn of the board
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
                   board[selectedSquare][0].stack->next = NULL;
                   board[selectedSquare][0].numTokens++;
                }
                else
                {
                    printf("This square is full.\nPlease select an empty square.\n"); // error messaage

                    board[selectedSquare][0].stack = (token *)malloc(sizeof(token)); // allocates memory
                    board[selectedSquare][0].stack->col = players[j].col;
                    board[selectedSquare][0].stack->next = NULL;
                    board[selectedSquare][0].numTokens++;

                }

            }
            else
            {
                printf("\n\nPlease select a square between 0-5.\n"); // error message if user selects square outside of range
                printf("Player %d select a square.\n", j+1);
                scanf("%d", &selectedSquare); // user selects square in first coloumn

                if(board[selectedSquare][0].stack == NULL) // if selected square is empty
                {
                   board[selectedSquare][0].stack = (token *)malloc(sizeof(token)); // allocates memory
                   board[selectedSquare][0].stack->col = players[j].col;
                   board[selectedSquare][0].stack->next = NULL;
                   board[selectedSquare][0].numTokens++;
                }

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

int play_game(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers){
    /*
        TODO: compartmentalise input validation
    */
    srand(time(NULL));//seed the random() function

    //a number of counter variables
    int userIndex = 0;//used to index the current user
    int j = 0;//inner loopp
    int k = 0;//innermost loops

    //variables to store a variety of user inputs
    int row = 0;//store user input of row number
    int column = 0;//store user input of column number
    enum DIR direction = 0;//whether the user would like to move a token up/down (0 - up; 1 - down)
    int choice;//used to store the user choice
    int movePossible;//integer user to indicate if a move can be made for a given dice roll
    int obstaclePresent;

    //miscellaneous variables
    /*bool*/int impassable = 1;//whether an obstacle square can be exited (default 1 - this square cannot be passed)
    int diceRoll = 0;//used to store the result of the user's dice roll

    //iterate over the players[] array until a player has 3 token in the last column
    while(players[(userIndex-1) % numPlayers].numTokensLastCol != 3){
        //print user's name and indicate start of turn
        //note: that all user names are terminated by '\n\0'
        printf("It is your turn to play %s", players[userIndex].name);
        while(getchar()!='\n');//clear input buffer
        puts("Please roll the dice by pressing enter");//request user input
        //wait for the user to hit enter
        while(getchar()!='\n');
        //simulate a dice roll
        diceRoll = rand()%6;
        //print out the result of the dice roll
        printf("You must move a token from row %d\n", diceRoll);

        impassable = 0;
        obstaclePresent = 0;
        int i = 0;
        j = 0;
        for(i = 0; i < NUM_COLUMNS; i++){
            if(impassable){
                break;
            }
            //if a non-empty obstacle square is encountered, and no columns left of it are non-empty, skip this turn
            if(board[diceRoll][i].type == OBSTACLE && board[diceRoll][i].stack != NULL){
                obstaclePresent = 1;
                //reset impassable variable indicating if an obstacle square can be passed
                impassable = 0;

                for(j = 0; j < NUM_ROWS; j++){
                    //if this square cannot be passed, exit loop
                    if(impassable){
                        break;
                    }
                    //iterate over all squares of the rows behind the current column
                    for(k = 0; k < i; k++){
                        //if the square is non-empty, this obstacles square cannot be passed
                        if(board[j][k].stack != NULL){
                            impassable = 1;
                            break;
                        }
                    }
                }
            }
        }
        if(impassable && obstaclePresent){
            puts("No moves currently possible from this row");
            //increment the userIndex
            ++userIndex;
            //perform the modulo operation on userIndex to ensure it remains within
            //the bounds of the players[] array (of length numPlayers)
            userIndex %= numPlayers;
            continue;
        }

        /***************************************************************************/
        //the following code handles the option for the user to move a token up/down
        puts("Would you like to move one of your tokens up/down?");//request user input
        puts("(1)Yes\n(0)No");
        scanf("%d", &choice);//scan user input

        //switch to handle user choice
        if(choice == 1){
            //request user input
            puts("Please input the row and column number of the token you would like to move");
            //scan in user input
            scanf("%d", &row);//scan in row index
            scanf("%d", &column);//scan in column index
            //ensure that this token belongs to the user and is not in an obstacle square nor an empty square
            //and that this token is not in the final column
            while(column == 8 || board[row][column].stack == NULL || board[row][column].stack->col != players[userIndex].col || board[row][column].type == OBSTACLE){
                puts("That square cannot be selected, please choose another");//error message
                puts("Please input the row and column number of the token you would like to move");//request user input
                scanf("%d", &row);//scan in user input
                scanf("%d", &column);//scan in user input
            }
            //request user input
            puts("Would you like to move up or down?\n(1)Up\n(0)Down");
            scanf("%d", &direction);//direction is of type enum DIR {UP, DOWN}
            //esnure 'direction' is within the allowed range of 0 - 1
            while(direction > 1 || direction < 0){
                puts("That is not a recognised input");//error message
                puts("Would you like to move up or down?\n(1)Up\n(0)Down");
                scanf("%d", &direction);
            }
            //switch statement to handle user input of direction
            switch(direction){
                case DOWN:
                    //edge case handling (can't move down from row 5)
                    if(row == 5){
                        puts("You cannot move down from this row");
                        break;
                    }
                    //move token down one row (rows numbered from 0 - 5 starting at top)
                    moveToken(board, row, column, row + 1, column);
                    print_board(board);
                    break;
                case UP:
                    //edge case handling (can't move up from row 0)
                    if(row == 0){
                        puts("You cannot move up from this row");
                        break;
                    }
                    //move token up one row (rows numbers from 0 - 5 starting at top)
                    moveToken(board, row, column, row - 1, column);
                    print_board(board);
                    break;
                default:
                    break;
            }
        }

        /***************************************************************/
        //the following code handles the mandatory component of the move
        //the user can move a token from any column in the row number that they rolled using the dice
        puts("Please enter the column number of the token you would like to move");//request user input
        scanf("%d", &column);//scan in user input
        //the user cannot select an empty square or a square in the final column
        while(column == 8 || board[diceRoll][column].stack == NULL){
            puts("This column number is invalid, please try again");
            puts("Please enter the column number of the token you would like to move");//request user input
            scanf("%d", &column);//scan in user input
        }

        j = 0;//reset counter variable j
        k = 0;//reset counter variable k
        impassable = 0;//reset bool variable impassable

        //iterate over all squares behind this obstacle square, if any are non-empty
        //then this square cannot be passed. also confim selected square is not empty
        while(board[diceRoll][column].type == OBSTACLE && board[diceRoll][column].stack != NULL){
            //reset impassable variable indicating if an obstacle square can be passed
            impassable = 0;

            //if the square is an obstacle square, iterate over all squares behind this square
            //and if anyone of them are non-empty, set impassable to 1, indicating that this
            //obstacle square cannot yet be passed
            if(board[diceRoll][column].type == OBSTACLE){
                for(j = 0; j < NUM_ROWS; j++){
                    //if this square cannot be passed, exit loop
                    if(impassable){
                        break;
                    }
                    //iterate over all squares of the row behind the current column
                    for(k = 0; k < column; k++){
                        //if the square is non-empty, this obstacles square cannot be passed
                        if(board[j][k].stack != NULL){
                            impassable = 1;
                            break;
                        }
                    }
                }
            }
            //if this square can be passed, continue program flow
            if(!impassable){
                break;
            }
            //if not passable, request that the user chooses a different column
            else{
                puts("This obstacle square cannot yet be passed");//error message
                puts("Please enter the column number of the token you would like to move");//request user input
                scanf("%d", &column);//scan user input
                //check that the user has not selected an empty square
                while(board[diceRoll][column].stack == NULL){
                    puts("This square is empty and cannot be chosen, please choose another");
                    puts("Please enter the column number of the token you would like to move");//request user input
                    scanf("%d", &column);//scan in user input
                }
            }
        }

        //once all relavant checks have been made, move the chosen token forward
        moveToken(board, diceRoll, column, diceRoll, column+1);

        //if the token moved is now in the final column, increment numTokensLastCol
        if(column == 8){
            players[userIndex].numTokensLastCol++;
        }

        //print the board after the token has been moved
        print_board(board);
        //increment the userIndex
        ++userIndex;
        //perform the modulo operation on userIndex to ensure it remains within
        //the bounds of the players[] array (of length numPlayers)
        userIndex %= numPlayers;
    }
    return userIndex-1;
}

/*
*   Allows a token to be moved from one square to another
*
*  Input: board - a a 6x9 array of squares that represents the board
*         srcRow, srcCol - the index of the square from which the token is to be moved
*         destRow, destCol - the index of the square to which the token is to be moved
*/
void moveToken(square board[NUM_ROWS][NUM_COLUMNS], int srcRow, int srcCol, int destRow, int destCol){
    token *temp;//a temporary pointer to the token on top of the stack
    //pop the token on top of the source stack and temporarily store it in the 'temp' variable
    temp = pop(&board[srcRow][srcCol].stack);
    //push the token which was on top of the source stack to the top of the destination stack
    push(&board[destRow][destCol].stack, temp);
    //increment number of tokens at destination
    board[destRow][destCol].numTokens++;
    //decrement number of token as source
    board[srcRow][srcCol].numTokens--;
    //if the source stack is now empty
    if(board[srcRow][srcCol].numTokens == 0){
        board[srcRow][srcCol].stack = NULL;//set the stack pointer of the source square to NULL
    }
}

/*
*   Pop a token from the top of the stack, and return a pointer to this token
*
*   input:
*   token **stack - a pointer to a pointer to a stack of tokens, which the top
*                   token will be removed from
*
*   output:
*   token* - a pointer to the token that was removed from the stack
*/
token* pop(token **stack){
    if(stack != NULL){
        token *temp = (token*)malloc(sizeof(token));
        temp = *stack;
        *stack = (*stack)->next;
        return temp;
    }
    return NULL;
}

/*
*   Push a token to the top of a stack of token
*
*   input:
*   token **stack - a pointer to a pointer to a stack of tokens, which the top
*                   token will be removed from
*   token *insert - the token to be pushed to the stack
*
*   output:
*   void
*/
void push(token **stack, token *insert){
    if (insert != NULL){
        insert->next = *stack;
        *stack = insert;
    }
    else {
        puts("Token must exist if it is to be moved onto the stack");
    }
}
