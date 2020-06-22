//
// Created by Matthew Lourenco on 20/06/2020.
//

#include "ai.h"

/* Constructor */
Ai::Ai(Board *b_, char role_): b(b_), role(role_) {
    data = std::vector<scores>(b->dim() * b->dim());
    if(role == P1) opRole = P2;
    else opRole = P1;
}

/* Destructor */
Ai::~Ai() {
    data.clear();
}

/* Methods */
// Finds the best possible move in one second of time
std::pair<int, int> Ai::findBestMove() {
    //Save start time

    // fill legalities
    resetEmptyCertainty(b);
    fillLegalities(b);

    //begin check
    if(cornersAvailable(b, role)) {
        //Try taking all available corners
    } else {
        for (int row = 0; row < b->dim(); row++) {
            for (int col = 0; col < b->dim(); col++) {
                // try all available moves
                if(evalMove(b, row, col, role)) {
                    // try playing here
                    // add flipped tile to score

                    // queue to look ahead moves and count scores
                }
            }
        }
    }
}
