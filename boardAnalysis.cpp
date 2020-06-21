//
// Created by Matthew Lourenco on 20/06/2020.
//

#include "boardAnalysis.h"

// Check if a piece can be placed in a certain tile and aim in a certain direction
bool checkLegalInDirection(const Board *b, int row, int col, char colour, int deltaRow, int deltaCol) {

    //Check for illegal input
    if(deltaRow == 0 && deltaCol == 0) return false;
    if(!b->inBounds(row, col)) return false;
    if(b->at(row, col).img != EMPTY) return false;

    //Check that next tile is the opposite colour
    row += deltaRow;
    col += deltaCol;
    if(!b->inBounds(row, col)) return false;
    if(b->at(row, col).img == EMPTY || b->at(row, col).img == colour) return false;

    //Check that the line ends in the same colour
    do {
        row += deltaRow;
        col += deltaCol;
        if(!b->inBounds(row, col)) return false;

    } while(b->at(row, col).img != colour && b->at(row, col).img != EMPTY);

    return b->at(row, col).img == colour;
}

// This function evaluates if a move is valid
bool evalMove(const Board *b, int row, int col, char colour) {
    //Check for valid position
    if(!b->inBounds(row, col)) return false;

    if(b->at(row, col).certain) {
        if(colour == P1) return b->at(row, col).p1legal;
        else if(colour == P2) return b->at(row, col).p2legal;
        else return false;
    }

    // Uncertain, calculate legality
    if(b->at(row, col).img != EMPTY) return false;

    //Check that the move is valid in at least one direction
    for(int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for(int deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if(checkLegalInDirection(b, row, col, colour, deltaRow, deltaCol)) {
                return true;
            }
        }
    }
    return false;
}

// This function checks if a valid move is available
bool availableMove(const Board *b, char colour) {
    for(int i = 0; i < b->dim(); i++) {
        for(int j = 0; j < b->dim(); j++) {
            if(evalMove(b, i, j, colour)) {
                return true;
            }
        }
    }
    return false;
}

// This function flips tiles in all valid directions
void flipTiles(Board *b, int row, int col, int colour) {
    int scanRow, scanCol;

    for(int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for(int deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if(checkLegalInDirection(b, row, col, colour, deltaRow, deltaCol)) {

                // Moves along direction until opposite colour is found. Flips tiles as it moves
                scanRow = row + deltaRow;
                scanCol = col + deltaCol;
                do {
                    b->at(scanRow, scanCol).fill(colour);
                    scanRow += deltaRow;
                    scanCol += deltaCol;
                } while(b->at(scanRow, scanCol).img != colour);
            }
        }
    }

    //Place new tile
    b->at(scanRow, scanCol).img = colour;
}

// This function calculates the number of tiles that can be flipped from a position
int tileScore(const Board *b, int row, int col, int colour) {
    int score = 0, addition;

    // Check for illegal input
    if(!b->inBounds(row, col)) return 0;
    if(b->at(row, col).img != EMPTY) return 0;

    for(int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for(int deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if(deltaRow == 0 && deltaCol == 0) continue;

            // Check that next tile is the opposite colour
            row += deltaRow;
            col += deltaCol;
            if(!b->inBounds(row, col)) continue;
            if(b->at(row, col).img == EMPTY || b->at(row, col).img == colour) continue;

            addition = 0;

            // Check that the line ends in the same colour
            do {
                addition++;
                row += deltaRow;
                col += deltaCol;
                if(!b->inBounds(row, col)) continue;

            } while(b->at(row, col).img != colour && b->at(row, col).img != EMPTY);

            if(b->at(row, col).img == colour) score += addition;
        }
    }

    return score;
}

// This function checks if there is a move available in the corners
bool cornersAvailable(const Board *b, char colour) {
    return evalMove(b, 0, 0, colour) || evalMove(b, 0, b->dim() - 1, colour) ||
           evalMove(b, b->dim() - 1, 0, colour) || evalMove(b, b->dim() - 1, b->dim() - 1, colour);
}

// This function checks if the position is beside a corner
bool danger(const Board *b, int row, int col) {
    return (row == 0 && col == 1) || (row == 1 && col == 0) ||
           (row == 1 && col == 1) || (row == 0 && col == b->dim() - 2) ||
           (row == 1 && col == b->dim() - 2) || (row == 1 && col == b->dim() - 1) ||
           (row == b->dim() - 2 && col == 0) || (row == b->dim() - 2 && col == 1) ||
           (row == b->dim() - 2 && col == b->dim() - 2) || (row == b->dim() - 2 && col == b->dim() - 1) ||
           (row == b->dim() - 1 && col == 1) || (row == b->dim() - 1 && col == b->dim() - 2);
}

// This function counts the number of tiles of a specific colour
int countColour(const Board *b, char colour) {
    int tiles = 0;

    for(int i = 0; i < b->dim(); i++) {
        for(int j = 0; j < b->dim(); j++) {
            if(b->at(i, j).img == colour) {
                tiles++;
            }
        }
    }

    return tiles;
}

/*

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
*/
