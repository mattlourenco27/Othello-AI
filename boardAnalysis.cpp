//
// Created by Matthew Lourenco on 20/06/2020.
//

#include "boardAnalysis.h"

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

// This function checks if the position is beside a corner
bool besideCorner(const Board *b, int row, int col) {
    return (row == 0 && col == 1) || (row == 1 && col == 0) ||
           (row == 1 && col == 1) || (row == 0 && col == b->dim() - 2) ||
           (row == 1 && col == b->dim() - 2) || (row == 1 && col == b->dim() - 1) ||
           (row == b->dim() - 2 && col == 0) || (row == b->dim() - 2 && col == 1) ||
           (row == b->dim() - 2 && col == b->dim() - 2) || (row == b->dim() - 2 && col == b->dim() - 1) ||
           (row == b->dim() - 1 && col == 1) || (row == b->dim() - 1 && col == b->dim() - 2);
}

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

// This function checks if there is a move available in the corners
bool cornersAvailable(const Board *b, char colour) {
    return evalMove(b, 0, 0, colour) || evalMove(b, 0, b->dim() - 1, colour) ||
           evalMove(b, b->dim() - 1, 0, colour) || evalMove(b, b->dim() - 1, b->dim() - 1, colour);
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

// calculates legality for all uncertain positions
void fillLegalities(Board *b) {
    bool p1legal, p2legal;
    for(int row = 0; row < b->dim(); row++) {
        for(int col = 0; col < b->dim(); col++) {
            if(!b->at(row, col).certain) {
                p1legal = false;
                p2legal = false;
                for(int drow = -1; drow <= 1; drow++) {
                    for(int dcol = -1; dcol <= 1; dcol++) {
                        if(!p1legal) p1legal = checkLegalInDirection(b, row, col, P1, drow, dcol);
                        if(!p2legal) p2legal = checkLegalInDirection(b, row, col, P2, drow, dcol);
                    }
                }

                b->at(row, col).certain = true;
                b->at(row, col).p1legal = p1legal;
                b->at(row, col).p2legal = p2legal;
            }
        }
    }
}

// This function flips tiles in all valid directions
void flipTiles(Board *b, int row, int col, char colour) {
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

// resets certainty on empty tiles
void resetEmptyCertainty(Board *b) {
    for(int row = 0; row < b->dim(); row++) {
        for(int col = 0; col < b->dim(); col++) {
            if(b->at(row, col).img == EMPTY) {
                b->at(row, col).certain = false;
            }
        }
    }
}

// This function calculates the number of tiles that can be flipped from a position
int tileScore(const Board *b, int row, int col, char colour) {
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
