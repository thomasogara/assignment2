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
    //the square selected by the user
    int selectedSquare = 0;
    //a temporary pointer to the token created once a valid input is received
    token *temp = NULL;

    //print the board ahead of the first token being placed
    printf("\nThe current board:\n");
    print_board(board); // prints board

    //each player can place 4 tokens
    for(int i=0; i < 4; i++){
        //iterate over all players
        for(int j=0; j <numPlayers; j++){
            //request user input
            printf("Please select a square in the first column %s?:", players[j].name);
            scanf("%d", &selectedSquare); // user selects square in first column

            //check that the input is valid
            /*the selected square cannot:
                -have a stack higher than the shortest stack in column 0
                -be outside of the range 0 - 5 (there are only 6 rows on the board)
                -be one of the player's own tokens
            */
            while(board[selectedSquare][0].numTokens > minNumOfTokens || selectedSquare < -1 || selectedSquare > 6 || (board[selectedSquare][0].stack != NULL && board[selectedSquare][0].stack->col == players[j].col))
            {
                printf("This square is not available because ");
                if(selectedSquare < -1 || selectedSquare > 6){
                    printf("the given row number is outside of the accepted bounds (0 - 5)\n");
                }
                else if(board[selectedSquare][0].numTokens > minNumOfTokens){
                    printf("the stack is too high compared to the other stacks in column 0\n");
                }
                else if((board[selectedSquare][0].stack != NULL && board[selectedSquare][0].stack->col == players[j].col)){
                    printf("the top token on this stack is one of your own tokens\n");
                }
                printf("Please select another square\n?:");
                scanf("%d", &selectedSquare); // takes input from user again
            } // close outer while loop

            //place the token on the board
            temp = (token *)malloc(sizeof(token)); // allocates memory for token
            temp->col = players[j].col;//assign player's color to token
            push(&board[selectedSquare][0].stack, temp);//push token to top of selected stack
            board[selectedSquare][0].numTokens++;//increment number of token on selected square

            // updates the minimum number of tokens
            if(((numPlayers * i) + j + 1)%NUM_ROWS ==0){
                minNumOfTokens++;
            }

            //print the board after every turn to show that the token has been placed
            printf("\nThe current board:\n");
            print_board(board); // prints board
        }
    }



}


/*
 * Place tokens in the first column of the board
 *
 * Input: board - a 6x9 array of squares that represents the board
 *        players - the array of the players
 *        numPlayers - the number of players
 *
 * Output: the index of the player that won the game
 */

int play_game(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers){
    srand(time(NULL));//seed the random() function

    int userIndex = 0;//int var used to index the current user

    //variables to store a variety of user inputs
    int row = 0;//store user input of row number
    int column = 0;//store user input of column number
    enum DIR direction = 0;//whether the user would like to move a token up/down (0 - up; 1 - down)
    int choice;//used to store the user choice at different stages in the program

    int diceRoll = 0;//used to store the result of the user's dice roll

    //iterate over the players[] array until a player has 3 tokens in the last column
    while(players[(userIndex-1) % numPlayers].numTokensLastCol != 3){
        //print user's name and indicate start of turn
        //note: that all user names are terminated by '\n\0'
        printf("It is your turn to play %s", players[userIndex].name);
        while(getchar()!='\n');//clear stdin buffer
        printf("Please roll the dice by pressing enter\n?:");//request user input
        //wait for the user to hit enter
        while(getchar()!='\n');
        //simulate a dice roll (between 0 and 5, the minimum and maximum row numbers)
        diceRoll = rand()%6;
        //print out the result of the dice roll
        printf("You rolled a %d on the dice\nYou must move a token from row %d\n", diceRoll+1, diceRoll);

        //ensure that the chosen row is not empty
        if(nonEmptySquaresInRow(board, diceRoll) == 0){
            puts("No moves are currently possible from this row because the row is empty");
            //increment the userIndex
            ++userIndex;
            //perform the modulo operation on userIndex to ensure it remains within
            //the bounds of the players[] array (of length numPlayers)
            userIndex %= numPlayers;

            //return to the start of the while loop
            continue;
        }

        /***************************************************************************/
        //the following code handles the option for the user to move a token up/down

        puts("Would you like to move one of your tokens up/down?");//request user input
        puts("(1)Yes\n(0)No");
        printf("?:");
        scanf("%d", &choice);//scan user input

        //verify that input is valid
        while(choice < 0 || choice > 1){
            puts("That is not a valid choice");
            puts("Would you like to move one of your tokens up/down?");//request user input
            puts("(1)Yes\n(0)No");
            printf("?:");
            scanf("%d", &choice);//scan user input
        }

        //if the user would like to move one of their tokens up/down
        if(choice == 1){
            //request user input
            printf("Please input the row and column number of the token you would like to move\n?:");
            //scan in user input
            scanf("%d", &row);//scan in row index
            scanf("%d", &column);//scan in column index

            //ensure that this token belongs to the user and is not in an obstacle square nor an empty square
            //and that this token is not in the final column
            while(column >= 8 || column < 0 || board[row][column].stack == NULL || board[row][column].stack->col != players[userIndex].col || (board[row][column].type == OBSTACLE && !isPassable(board, row, column))){
                puts("That square cannot be selected, please choose another");//error message
                printf("Please input the row and column number of the token you would like to move\n?:");//request user input
                scanf("%d", &row);//scan in user input
                scanf("%d", &column);//scan in user input
            }

            //request user input
            printf("Would you like to move up or down?\n(1)Up\n(0)Down\n?:");
            scanf("%d", &direction);//direction is of type enum DIR {UP, DOWN}
            //esnure 'direction' is within the allowed range of 0 - 1
            while(direction > 1 || direction < 0){
                puts("That is not a recognised input");//error message
                printf("Would you like to move up or down?\n(1)Up\n(0)Down\n?:");
                scanf("%d", &direction);
            }

            //switch statement to handle user input of direction
            switch(direction){
                case DOWN:
                    //edge case handling (can't move down from row 5)
                    while(row == 5){
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

        //if the row is empty after the user has moved one of their tokens up/down
        //then this turn must be skipped as no moves can be made
        if(nonEmptySquaresInRow(board, diceRoll) == 0){
            puts("No moves currently possible from this row because empty");
            //increment the userIndex
            ++userIndex;
            //perform the modulo operation on userIndex to ensure it remains within
            //the bounds of the players[] array (of length numPlayers)
            userIndex %= numPlayers;

            //return to top of while loop
            continue;
        }

        /***************************************************************/
        //the following code handles the mandatory component of the move

        //the user can move a token from any column in the row number that they rolled using the dice
        printf("Please enter the column number of the token you would like to move in row %d\n?:", diceRoll);//request user input
        scanf("%d", &column);//scan in user input
        //the user cannot select an empty square or a square outside of rows 0 - 8
        while(column >= 8 || column < 0 ||  board[diceRoll][column].stack == NULL){
            puts("This column number is invalid, please try again");
            printf("Please enter the column number of the token you would like to move in row %d\n?:", diceRoll);//request user input
            scanf("%d", &column);//scan in user input
        }

        //if there is only one occupied square on the row and that square is an obstacle
        //square which cannot yet be passed then no moves are possible
        if(board[diceRoll][column].type == OBSTACLE && !isPassable(board, diceRoll,column) && nonEmptySquaresInRow(board, diceRoll) == 1){
            puts("No moves currently possible from this row because it is empty");
            puts("except for an obstacle square which cannot be passed");
            //increment the userIndex
            ++userIndex;
            //perform the modulo operation on userIndex to ensure it remains within
            //the bounds of the players[] array (of length numPlayers)
            userIndex %= numPlayers;

            //return to top of loop
            continue;
        }

        //if the user chooses an obstacle square, ensure that it can be passed
        if(board[diceRoll][column].type == OBSTACLE){
            //iterate over all squares behind this obstacle square, if any are non-empty
            //then this square cannot be passed. also confim selected square is not empty
            while(board[diceRoll][column].stack == NULL || (board[diceRoll][column].type == OBSTACLE && !isPassable(board, diceRoll, column))){
                puts("This obstacle square cannot yet be passed");//error message
                printf("Please enter the column number of the token you would like to move in row %d\n?:", diceRoll);//request user input
                scanf("%d", &column);//scan user input
                //check that the user has not selected an empty square
                while(column >= 8 || column < 0 || board[diceRoll][column].stack == NULL){
                    puts("This square cannot be chosen, it is empty or out of bounds, please choose another");
                    printf("Please enter the column number of the token you would like to move in row %d\n?:", diceRoll);//request user input
                    scanf("%d", &column);//scan in user input
                }
            }
        }

        //once all relavant checks have been made, move the chosen token forward
        moveToken(board, diceRoll, column, diceRoll, column+1);

        //if the token moved is now in the final column, increment numTokensLastCol
        if(column + 1 == 8){
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
    //return the index of the winning player
    return userIndex-1;
}

/*
*   Allows a token to be moved from one square to another
*
*  Input: board - a 6x9 array of squares that represents the board
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
*   ***PLEASE NOTE THAT THIS FUNCTION DOES NOT FREE THE MEMORY ASSSOCIATED WITH THE
*   POPPED TOKEN AS TOKENS PERSIST FOR THE LIFE OF THE PROGRAM***
*
*   input:
*   token **stack - a pointer to a pointer to a stack of tokens, which the top
*                   token will be removed from
*
*   output:
*   token* - a pointer to the token that was removed from the stack
*/
token* pop(token **stack){
    //check that the stack is not empty
    if(stack != NULL){
        token *temp;//initialise a temp pointer to a token
        temp = *stack;//set temp to point to the stack
        *stack = (*stack)->next;//de-thread the top node of the stack
        return temp;//return a pointer to what was previously the top node of the stack
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
    //if the node to be inserted is not NULL
    if (insert != NULL){
        insert->next = *stack;//thread the stack onto the bottom of the node to be inserted
        *stack = insert;//point the stack at the current top node (i.e. the node just inserted)
    }
    else {
        puts("Token must exist if it is to be moved onto the stack");//error message
    }
}


/*
*   count the number of non-empty squares in a row, excluding the last column
*
*   input: board - a 6x9 array of squares that represents the board
*          row - the row in which the number of non-empty squares are to be counted
*
*   output: the number of non-empty squares in the row, excluding column 8
*/
int nonEmptySquaresInRow(square board[NUM_ROWS][NUM_COLUMNS], int row){
    int i = 0;//initialise counter variable i
    int sum = 0;//intialise temp int variable to store sum of non-empty squares
    //count all columns except column 8 since tokens cannot be moved from the last column
    for(i = 0; i < NUM_COLUMNS - 1; i++){
        if(board[row][i].stack != NULL){
            sum++;
        }
    }
    return sum;
}

/*
*   check if an obstacle square can be passed
*
*   input: board - a 6x9 array of squares that represents the board
*          row - the row number of the obstacle square in question
*          col - the column number of the obstacle square in question
*
*  output: 0 - obstacle square cannot be passed
*          1 - obstacle square can be passed
*
*/
int isPassable(square board[NUM_ROWS][NUM_COLUMNS], int row, int col){
    int i = 0;
    int j = 0;
    int passable = 1;
    for(i = 0; i < NUM_ROWS; i++){
        //if this square cannot be passed, exit loop
        if(!passable){
            break;
        }
        //iterate over all squares of the rows behind the current column
        for(j = 0; j < col; j++){
            //if the square is non-empty, this obstacles square cannot be passed
            if(board[i][j].stack != NULL){
                passable = 0;
                break;
            }
        }
    }
    return passable;
}
