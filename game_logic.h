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
void play_game(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers);

/*
*   Allows a token to be moved from one square to another
*
*  Input: board - a a 6x9 array of squares that represents the board
*         srcRow, srcCol - the index of the square from which the token is to be moved
*         destRow, destCol - the index of the square to which the token is to be moved
*/
int moveToken(square board[NUM_ROWS][NUM_COLUMNS], int srcRow, int srcCol, int destRow, int destCol);
