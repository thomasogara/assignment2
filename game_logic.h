//an enumerated type used to indicate whether the use is moving a token up or down
enum DIR {DOWN, UP};

/*
 * Prints a horizontal line of hyphen "-" characters
 * Input: void
 */
void printLine(void);


/*
 * Prints the board
 *
 * Input: the board to be printed.
 */
void print_board(square board[NUM_ROWS][NUM_COLUMNS]);


/*
 * Place tokens in the first column of the board
 *
 * Input: board - a 6x9 array of squares that represents the board
 *        players - the array of the players
 *        numPlayers - the number of players
 */
void place_tokens(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers);



/*
 *  * Manages the logic of the game
 *
 * Input: board - a 6x9 array of squares that represents the board
 *        players - the array of the players
 *        numPlayers - the number of players
 */
int play_game(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers);

/*
*   Allows a token to be moved from one square to another
*
*  Input: board - a a 6x9 array of squares that represents the board
*         srcRow, srcCol - the index of the square from which the token is to be moved
*         destRow, destCol - the index of the square to which the token is to be moved
*/
void moveToken(square board[NUM_ROWS][NUM_COLUMNS], int srcRow, int srcCol, int destRow, int destCol);

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
token* pop(token **stack);


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
void push(token **stack, token *insert);

/*
*   count the number of non-empty squares in a row, excluding the last column
*
*   input: board - a 6x9 array of squares that represents the board
*          row - the row in which the number of non-empty squares are to be counted
*
*   output: the number of non-empty squares in the row, excluding column 8
*/
int nonEmptySquaresInRow(square board[NUM_ROWS][NUM_COLUMNS], int row);

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
int isPassable(square board[NUM_ROWS][NUM_COLUMNS], int row, int col);
