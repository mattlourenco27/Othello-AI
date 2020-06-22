//
// Created by Matthew Lourenco on 20/06/2020.
//

#include "ai.h"
#include <queue>

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
    std::queue<possibleFuture *> q;
    Board *tmp;
    possibleFuture *p;

    //Save start time

    // fill legalities
    resetEmptyCertainty(b);
    fillLegalities(b);

    //begin check
    if(cornersAvailable(b, role)) {
        //Try taking all available corners
        if(evalMove(b, 0, 0, role)) {
            // try playing here
            tmp = new Board(*b);
            q.push(new possibleFuture(tmp, false, &data[0]));
            // add flipped tile to score
            data[0].my = flipTiles(tmp, 0, 0, role);

            // Check time passed
        }
        if(evalMove(b, 0, b->dim() - 1, role)) {
            // try playing here
            tmp = new Board(*b);
            q.push(new possibleFuture(tmp, false, &data[b->dim() - 1]));
            // add flipped tile to score
            data[b->dim() - 1].my = flipTiles(tmp, 0, b->dim() - 1, role);

            // Check time passed
        }
        if(evalMove(b, b->dim() - 1, 0, role)) {
            // try playing here
            tmp = new Board(*b);
            q.push(new possibleFuture(tmp, false, &data[(b->dim() - 1) * b->dim()]));
            // add flipped tile to score
            data[(b->dim() - 1) * b->dim()].my = flipTiles(tmp, b->dim() - 1, 0, role);

            // Check time passed
        }
        if(evalMove(b, b->dim() - 1, b->dim() - 1, role)) {
            // try playing here
            tmp = new Board(*b);
            q.push(new possibleFuture(tmp, false, &data[b->dim() * b->dim() - 1]));
            // add flipped tile to score
            data[b->dim() * b->dim() - 1].my = flipTiles(tmp, b->dim() - 1, b->dim() - 1, role);

            // Check time passed
        }
    } else {
        for (int row = 0; row < b->dim(); row++) {
            for (int col = 0; col < b->dim(); col++) {
                // try all available moves
                if(evalMove(b, row, col, role)) {
                    // try playing here
                    tmp = new Board(*b);
                    q.push(new possibleFuture(tmp, false, &data[row*b->dim() + col]));
                    // add flipped tile to score
                    data[row*b->dim() + col].my = flipTiles(tmp, row, col, role);

                    // Check time passed
                }
            }
        }
    }

    while(true) {
        // evaluate each possible future and pursue further futures

        // end the loop once too much time has passed
    }

    // clean possible futures
    while(q.size()) {
        p = q.front();
        q.pop();
        delete p;
    }

    // fine tile with desired score ratio and return its coordinates
}
