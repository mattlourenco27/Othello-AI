//
// Created by Matthew Lourneco on 20/06/2020.
//

#include "Board.h"

//Prints the board along with coordinates of each tile
void printBoard(char board[][26], int n) {
    int i, j;

    printf("  ");
    for(i = 0; i < n; i++) {
        printf("%c", 'a' + i);
    }

    printf("\n");

    for(i = 0; i < n; i++) {
        printf("%c ", 'a' + i);

        for(j = 0; j < n; j++) {
            printf("%c", board[i][j]);
        }

        printf("\n");
    }
}

//Check if specific position is on the board of size n
bool positionInBounds(int n, int row, int col) {
    return row >= 0 && row < n && col >= 0 && col < n;
}

//Check if a piece can be placed in a certain tile and aim in a certain direction
bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol) {
    const char EMPTY = 'U';

    //Check for illegal input
    if(deltaRow == 0 && deltaCol == 0) return false;
    if(board[row][col] != EMPTY) return false;
    if(!positionInBounds(n, row, col)) return false;

    //Check that next tile is the opposite colour
    row += deltaRow;
    col += deltaCol;
    if(!positionInBounds(n, row, col)) return false;
    if(board[row][col] == EMPTY || board[row][col] == colour) return false;

    //Check that the line ends in the same colour
    do {
        row += deltaRow;
        col += deltaCol;
        if(!positionInBounds(n, row, col)) return false;

    } while(board[row][col] != colour && board[row][col] != EMPTY);

    if(board[row][col] == colour) return true;
    return false;
}

//This function evaluates if a move is valid
bool evalMove(char board[][26], int n, int row, int col, char colour) {
    const char EMPTY = 'U';
    int deltaRow, deltaCol;

    //Check for valid position
    if(!positionInBounds(n, row, col)) return false;
    if(board[row][col] != EMPTY) return false;

    //Check that the move is valid in at least one direction
    for(deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for(deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {
                return true;
            }
        }
    }
    return false;
}

//This function checks if a valid move is avialble
bool availableMove(char board[][26], int n, char colour) {
    int i, j;

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(evalMove(board, n, i, j, colour)) {
                return true;
            }
        }
    }
    return false;
}

//This function flips tiles in all valid directions
void flipTiles(char board[][26], int n, int row, int col, int colour) {
    int deltaRow, deltaCol, scanRow, scanCol;

    for(deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for(deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {

                //Moves along direction until opposite colour is found. Flips tiles as it moves
                scanRow = row + deltaRow;
                scanCol = col + deltaCol;
                do {
                    board[scanRow][scanCol] = colour;
                    scanRow += deltaRow;
                    scanCol += deltaCol;
                } while(board[scanRow][scanCol] != colour);
            }
        }
    }

    //Place new tile
    board[row][col] = colour;
}

//This function calculates the number of tiles that can be flipped from a position
int tileScore(char board[][26], int n, int row, int col, int colour) {
    int deltaRow, deltaCol, scanRow, scanCol, score = 0;

    for(deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for(deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {

                //Moves along direction until opposite colour is found
                scanRow = row + deltaRow;
                scanCol = col + deltaCol;
                do {
                    score++;
                    scanRow += deltaRow;
                    scanCol += deltaCol;
                } while(board[scanRow][scanCol] != colour);
            }
        }
    }
    return score;
}

//Prints the board along with coordinates of each tile
void printBoard(char board[][26], int n) {
    int i, j;

    printf("  ");
    for(i = 0; i < n; i++) {
        printf("%c", 'a' + i);
    }

    printf("\n");

    for(i = 0; i < n; i++) {
        printf("%c ", 'a' + i);

        for(j = 0; j < n; j++) {
            printf("%c", board[i][j]);
        }

        printf("\n");
    }
}

//Check if specific position is on the board of size n
bool positionInBounds(int n, int row, int col) {
    return row >= 0 && row < n && col >= 0 && col < n;
}

//Check if a piece can be placed in a certain tile and aim in a certain direction
bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol) {
    const char EMPTY = 'U';

    //Check for illegal input
    if(deltaRow == 0 && deltaCol == 0) return false;
    if(board[row][col] != EMPTY) return false;
    if(!positionInBounds(n, row, col)) return false;

    //Check that next tile is the opposite colour
    row += deltaRow;
    col += deltaCol;
    if(!positionInBounds(n, row, col)) return false;
    if(board[row][col] == EMPTY || board[row][col] == colour) return false;

    //Check that the line ends in the same colour
    do {
        row += deltaRow;
        col += deltaCol;
        if(!positionInBounds(n, row, col)) return false;

    } while(board[row][col] != colour && board[row][col] != EMPTY);

    if(board[row][col] == colour) return true;
    return false;
}

//This function evaluates if a move is valid
bool evalMove(char board[][26], int n, int row, int col, char colour) {
    const char EMPTY = 'U';
    int deltaRow, deltaCol;

    //Check for valid position
    if(!positionInBounds(n, row, col)) return false;
    if(board[row][col] != EMPTY) return false;

    //Check that the move is valid in at least one direction
    for(deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for(deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {
                return true;
            }
        }
    }
    return false;
}

//This function checks if a valid move is avialble
bool availableMove(char board[][26], int n, char colour) {
    int i, j;

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(evalMove(board, n, i, j, colour)) {
                return true;
            }
        }
    }
    return false;
}

//This function flips tiles in all valid directions
void flipTiles(char board[][26], int n, int row, int col, int colour) {
    int deltaRow, deltaCol, scanRow, scanCol;

    for(deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for(deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {

                //Moves along direction until opposite colour is found. Flips tiles as it moves
                scanRow = row + deltaRow;
                scanCol = col + deltaCol;
                do {
                    board[scanRow][scanCol] = colour;
                    scanRow += deltaRow;
                    scanCol += deltaCol;
                } while(board[scanRow][scanCol] != colour);
            }
        }
    }

    //Place new tile
    board[row][col] = colour;
}

//This function calculates the number of tiles that can be flipped from a position
int tileScore(char board[][26], int n, int row, int col, int colour) {
    int deltaRow, deltaCol, scanRow, scanCol, score = 0;

    for(deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for(deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {

                //Moves along direction until opposite colour is found
                scanRow = row + deltaRow;
                scanCol = col + deltaCol;
                do {
                    score++;
                    scanRow += deltaRow;
                    scanCol += deltaCol;
                } while(board[scanRow][scanCol] != colour);
            }
        }
    }
    return score;
}

//This function copies the game board
char **cpBoard(char board[][26], int n) {
    char **newBoard = (char **)malloc(n*sizeof(char *));
    int i, j;

    for(i = 0; i < n; i++) {
        newBoard[i] = (char *)malloc(n*sizeof(char));

        for(j = 0; j < n; j++) {
            newBoard[i][j] = board[i][j];
        }
    }

    return newBoard;
}

//This function frees the board and copies back the data
void freeBoard(char **lastBoard, char board[][26], int n) {
    int i, j;

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            board[i][j] = lastBoard[i][j];
        }
        free(lastBoard[i]);
    }
    free(lastBoard);
}

//This function checks if there is a move available in the corners
bool cornersAvailable(char board[][26], int n, char colour) {
    return evalMove(board, n, 0, 0, colour) || evalMove(board, n, 0, n-1, colour) ||
           evalMove(board, n, n-1, 0, colour) || evalMove(board, n, n-1, n-1, colour);
}

//This function checks if the position is beside a corner
bool danger(char board[][26], int n, int row, int col) {
    return (row == 0 && col == 1) || (row == 1 && col == 0) ||
           (row == 1 && col == 1) || (row == 0 && col == n-2) ||
           (row == 1 && col == n-2) || (row == 1 && col == n-1) ||
           (row == n-2 && col == 0) || (row == n-2 && col == 1) ||
           (row == n-2 && col == n-2) || (row == n-2 && col == n-1) ||
           (row == n-1 && col == 1) || (row == n-1 && col == n-2);
}

typedef struct playerScore {
    int cpu;
    int ai;
} Score;

//This function counts the number of tiles of a specific colour
int countColour(char board[][26], int n, char colour) {
    int i, j, tiles = 0;

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(board[i][j] == colour) {
                tiles++;
            }
        }
    }

    return tiles;
}

//This function is a recursive check of every position
void recurCheck(char board[][26], int n, char cpuColour, char aiColour, bool cpuTurn, int depth, Score *score){
    int i, j;

    bool cpuNoMove = true;

    if(depth == 0) {
        return;
    }

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(cpuTurn && evalMove(board, n, i, j, cpuColour)) {
                cpuNoMove = false;

                score->cpu += tileScore(board, n, i, j, aiColour);

                char **lastBoard = cpBoard(board, n);
                flipTiles(board, n, i, j, cpuColour);
                recurCheck(board, n, cpuColour, aiColour, !cpuTurn, depth - 1, score);
                freeBoard(lastBoard, board, n);
            } else if(!cpuTurn && evalMove(board, n, i, j, aiColour)) {
                score->ai += tileScore(board, n, i, j, aiColour);

                char **lastBoard = cpBoard(board, n);
                flipTiles(board, n, i, j, aiColour);
                recurCheck(board, n, cpuColour, aiColour, !cpuTurn, depth - 1, score);
                freeBoard(lastBoard, board, n);
            }
        }
    }

    if(cpuNoMove) {
        recurCheck(board, n, cpuColour, aiColour, !cpuTurn, depth - 1, score);
    }
}

//This function finds the best possible move in one second of time
void findBestMove(char board[][26], int n, char cpuColour, char aiColour, int *x, int *y, double trueTimeStart) {
    int worstScore = 0, i, j;
    int depth = 5;
    Score *score;
    score = (Score *)malloc(sizeof(Score));
    score->ai = 0;
    score->cpu = 0;
    *x = -1;

    bool mustPlayDanger = true;

    //Check time
    struct rusage usage;
    struct timeval end;

    double timeStart = trueTimeStart;
    double remainingTime = 0.9;

    if(cornersAvailable(board, n, cpuColour)) {
        if(evalMove(board, n, 0, 0, cpuColour)) {
            char **lastBoard = cpBoard(board, n);
            flipTiles(board, n, 0, 0, cpuColour);
            recurCheck(board, n, cpuColour, aiColour, false, depth, score);
            freeBoard(lastBoard, board, n);

            if(*x == -1 || worstScore > score->ai) {
                worstScore = score->ai;
                *x = 0;
                *y = 0;
            }
        }
        if(evalMove(board, n, 0, n-1, cpuColour)) {
            char **lastBoard = cpBoard(board, n);
            flipTiles(board, n, 0, n-1, cpuColour);
            recurCheck(board, n, cpuColour, aiColour, false, depth, score);
            freeBoard(lastBoard, board, n);

            if(*x == -1 || worstScore > score->ai) {
                worstScore = score->ai;
                *x = 0;
                *y = n-1;
            }
        }
        if(evalMove(board, n, n-1, 0, cpuColour)) {
            char **lastBoard = cpBoard(board, n);
            flipTiles(board, n, n-1, 0, cpuColour);
            recurCheck(board, n, cpuColour, aiColour, false, depth, score);
            freeBoard(lastBoard, board, n);

            if(*x == -1 || worstScore > score->ai) {
                worstScore = score->ai;
                *x = n-1;
                *y = 0;
            }
        }
        if(evalMove(board, n, n-1, n-1, cpuColour)) {
            char **lastBoard = cpBoard(board, n);
            flipTiles(board, n, n-1, n-1, cpuColour);
            recurCheck(board, n, cpuColour, aiColour, false, depth, score);
            freeBoard(lastBoard, board, n);

            if(*x == -1 || worstScore > score->ai) {
                worstScore = score->ai;
                *x = n-1;
                *y = n-1;
            }
        }
    } else {
        for(i = 0; i < n; i++) {
            for(j = 0; j < n; j++) {
                if(evalMove(board, n, i, j, cpuColour)) {
                    if(!danger(board, n, i, j)) {
                        mustPlayDanger = false;
                        char **lastBoard = cpBoard(board, n);
                        flipTiles(board, n, i, j, cpuColour);
                        recurCheck(board, n, cpuColour, aiColour, false, depth, score);
                        freeBoard(lastBoard, board, n);

                        if(*x == -1 || worstScore > score->ai) {
                            worstScore = score->ai;
                            *x = i;
                            *y = j;
                        }

                        getrusage(RUSAGE_SELF, &usage);
                        end = usage.ru_utime;
                        double timeEnd = end.tv_sec + end.tv_usec / 1000000.0; // in seconds

                        remainingTime -= timeEnd - timeStart;
                        timeStart = timeEnd;

                        if(remainingTime < 0.1) {
                            return;
                        }
                    }
                }
            }
        }

        if(mustPlayDanger) {
            for(i = 0; i < n; i++) {
                for(j = 0; j < n; j++) {
                    if(evalMove(board, n, i, j, cpuColour)) {
                        char **lastBoard = cpBoard(board, n);
                        flipTiles(board, n, i, j, cpuColour);
                        recurCheck(board, n, cpuColour, aiColour, false, depth, score);
                        freeBoard(lastBoard, board, n);

                        if(*x == -1 || worstScore > score->ai) {
                            worstScore = score->ai;
                            *x = i;
                            *y = j;
                        }

                        getrusage(RUSAGE_SELF, &usage);
                        end = usage.ru_utime;
                        double timeEnd = end.tv_sec + end.tv_usec / 1000000.0; // in seconds

                        remainingTime -= timeEnd - timeStart;
                        timeStart = timeEnd;

                        if(remainingTime < 0.1) {
                            return;
                        }
                    }
                }
            }
        }
    }

    free(score);
}

int main(int argc, char **argv)
{
    int n, i, j;
    char board[26][26];
    char cpuColour, userColour, newLine;
    bool cpuTurn = false;

    printf("Enter the board dimension: ");
    scanf("%d", &n);

    //Initialize the board
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            board[i][j] = 'U';
        }
    }

    //Add starting tiles
    board[n/2][n/2] = 'W';
    board[n/2 - 1][n/2 - 1] = 'W';
    board[n/2][n/2 - 1] = 'B';
    board[n/2 - 1][n/2] = 'B';

    printf("Computer plays (B/W): ");
    scanf("%c", &newLine); //Catch stray newline
    scanf("%c", &cpuColour);
    printBoard(board, n);

    //Determine player to move first
    if(cpuColour == 'B') {
        cpuTurn = true;
        userColour = 'W';
    } else {
        cpuTurn = false;
        userColour = 'B';
    }

    int bestX, bestY;
    char c1, c2;

    while(availableMove(board, n, userColour) || availableMove(board, n, cpuColour)) {
        if(cpuTurn) {
            //First check if the computer has a valid move
            if(availableMove(board, n, cpuColour)) {
                bestX = 0;
                bestY = 0;

                struct rusage usage; // a structure to hold "resource usage" (including time)
                struct timeval start; // will hold the start and end times
                getrusage(RUSAGE_SELF, &usage);
                start = usage.ru_utime;
                double timeStart = start.tv_sec + start.tv_usec / 1000000.0; // in seconds

                // PLACE THE CODE YOU WISH TO TIME HERE

                findBestMove(board, n, cpuColour, userColour, &bestX, &bestY, timeStart);

                //Flip tiles for cpu
                printf("Computer places %c at %c%c.\n", cpuColour, bestX + 'a', bestY + 'a');
                flipTiles(board, n, bestX, bestY, cpuColour);
                printBoard(board, n);
            } else {
                printf("%c player has no valid move.\n", cpuColour);
            }
        } else {
            //First check if the user has a valid move
            if(availableMove(board, n, userColour)) {

                //Get user move
                printf("Enter move for colour %c (RowCol): ", userColour);
                scanf("%c", &newLine); //Catch stray newline
                scanf("%c%c", &c1, &c2);

                //Convert to x and y co-ordinates
                i = c1 - 'a';
                j = c2 - 'a';

                //Find if the move is valid. Forfiet game if it is not valid
                if(evalMove(board, n, i, j, userColour)) {
                    //Flip tiles for user
                    flipTiles(board, n, i, j, userColour);
                    printBoard(board, n);
                } else {
                    printf("Invalid move.\n");
                    break;
                }
            } else {
                printf("%c player has no valid move.\n", userColour);
            }
        }

        //Flip turn to other player
        cpuTurn = !cpuTurn;
    }

    //Count tiles of each player
    int B = countColour(board, n, 'B');
    int W = countColour(board, n, 'W');

    if(B > W) {
        printf("B player wins.\n");
    } else if(W > B){
        printf("W player wins.\n");
    } else {
        printf("It's a tie.\n");
    }
    return 0;
}

int main(int argc, char **argv) {
    int n, i, j;
    char board[26][26];
    char cpuColour, userColour, newLine;
    bool cpuTurn = false;

    printf("Enter the board dimension: ");
    scanf("%d", &n);

    //Initialize the board
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            board[i][j] = 'U';
        }
    }

    //Add starting tiles
    board[n / 2][n / 2] = 'W';
    board[n / 2 - 1][n / 2 - 1] = 'W';
    board[n / 2][n / 2 - 1] = 'B';
    board[n / 2 - 1][n / 2] = 'B';

    printf("Computer plays (B/W): ");
    scanf("%c", &newLine); //Catch stray newline
    scanf("%c", &cpuColour);
    printBoard(board, n);

    //Determine player to move first
    if (cpuColour == 'B') {
        cpuTurn = true;
        userColour = 'W';
    } else {
        cpuTurn = false;
        userColour = 'B';
    }

    int bestScore, bestX, bestY;
    char c1, c2;

    while (availableMove(board, n, userColour) || availableMove(board, n, cpuColour)) {
        if (cpuTurn) {
            //Initialize best score and co-ordinates
            bestScore = 0;
            bestX = 0;
            bestY = 0;

            //Find best score and co-ordinates
            for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                    if (tileScore(board, n, i, j, cpuColour) > bestScore) {
                        bestScore = tileScore(board, n, i, j, cpuColour);
                        bestX = i;
                        bestY = j;
                    }
                }
            }

            //If Best score flips no tiles then the cpu cannot play
            if (bestScore == 0) {
                printf("%c player has no valid move.\n", cpuColour);
            } else {
                //Flip tiles for cpu
                printf("Computer places %c at %c%c.\n", cpuColour, bestX + 'a', bestY + 'a');
                flipTiles(board, n, bestX, bestY, cpuColour);
                printBoard(board, n);
            }

        } else {
            //First check if the player has a valid move
            if (availableMove(board, n, userColour)) {

                //Get user move
                printf("Enter move for colour %c (RowCol): ", userColour);
                scanf("%c", &newLine); //Catch stray newline
                scanf("%c%c", &c1, &c2);

                //Convert to x and y co-ordinates
                i = c1 - 'a';
                j = c2 - 'a';

                //Find if the move is valid. Forfiet game if it is not valid
                if (evalMove(board, n, i, j, userColour)) {
                    //Flip tiles for user
                    flipTiles(board, n, i, j, userColour);
                    printBoard(board, n);
                } else {
                    printf("Invalid move.\n");
                    break;
                }
            } else {
                printf("%c player has no valid move.\n", userColour);
            }

        }

        //Flip turn to other player
        cpuTurn = !cpuTurn;
    }

    //Count tiles of each player
    int B = 0, W = 0;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (board[i][j] == 'B') {
                B++;
            } else if (board[i][j] == 'W') {
                W++;
            }
        }
    }

    if (B > W) {
        printf("B player wins.\n");
    } else if (W > B) {
        printf("W player wins.\n");
    } else {
        printf("It's a tie.\n");
    }

    return 0;
}