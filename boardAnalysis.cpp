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
    if(b->at(row, col) != EMPTY) return false;

    //Check that next tile is the opposite colour
    row += deltaRow;
    col += deltaCol;
    if(!b->inBounds(row, col)) return false;
    if(b->at(row, col) == EMPTY || b->at(row, col) == colour) return false;

    //Check that the line ends in the same colour
    try {
        do {
            row += deltaRow;
            col += deltaCol;
        } while(b->at(row, col) != colour && b->at(row, col) != EMPTY);
    } catch(out_of_bounds & e) { // line continued off of the board
        return false;
    }

    return b->at(row, col) == colour;
}

// This function counts the number of tiles of a specific colour
int countColour(const Board *b, char colour) {
    int tiles = 0;

    for(int i = 0; i < b->dim(); i++) {
        for(int j = 0; j < b->dim(); j++) {
            if(b->at(i, j) == colour) {
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
    if(b->at(row, col) != EMPTY) return false;

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

// This function flips tiles in all valid directions and returns the number of tiles flipped
int flipTiles(Board *b, int row, int col, char colour) {
    int scanRow, scanCol, score = 0;

    for(int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for(int deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if(checkLegalInDirection(b, row, col, colour, deltaRow, deltaCol)) {

                // Moves along direction until opposite colour is found. Flips tiles as it moves
                scanRow = row + deltaRow;
                scanCol = col + deltaCol;
                do {
                    b->at(scanRow, scanCol) = colour;
                    score++;
                    scanRow += deltaRow;
                    scanCol += deltaCol;
                } while(b->at(scanRow, scanCol) != colour);
            }
        }
    }

    //Place new tile
    b->at(row, col) = colour;
    return score;
}

// This function calculates the number of tiles that can be flipped from a position
int tileScore(const Board *b, int row, int col, char colour) {
    int scanRow, scanCol, score = 0, addition;

    // Check for illegal input
    if(!b->inBounds(row, col)) return 0;
    if(b->at(row, col) != EMPTY) return 0;

    for(int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for(int deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if(deltaRow == 0 && deltaCol == 0) continue;

            // Check that next tile is the opposite colour
            scanRow = row + deltaRow;
            scanCol = col + deltaCol;
            if(!b->inBounds(scanRow, scanCol)) continue;
            if(b->at(scanRow, scanCol) == EMPTY || b->at(scanRow, scanCol) == colour) continue;

            addition = 0;

            // Check that the line ends in the same colour
            try {
                do {
                    addition++;
                    scanRow += deltaRow;
                    scanCol += deltaCol;
                } while (b->at(scanRow, scanCol) != colour && b->at(scanRow, scanCol) != EMPTY);
            } catch (out_of_bounds & e) {
                continue;
            }

            if(b->at(scanRow, scanCol) == colour) score += addition;
        }
    }

    return score;
}
