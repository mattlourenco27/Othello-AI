//
// Created by Matthew Lourenco on 20/06/2020.
//

#include "ai.h"
#include <chrono>
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
    auto start = std::chrono::high_resolution_clock::now(), stop = start;
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    const double maxTime = 0.9;

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
        }
        if(evalMove(b, 0, b->dim() - 1, role)) {
            // try playing here
            tmp = new Board(*b);
            q.push(new possibleFuture(tmp, false, &data[b->dim() - 1]));
            // add flipped tile to score
            data[b->dim() - 1].my = flipTiles(tmp, 0, b->dim() - 1, role);
        }
        if(evalMove(b, b->dim() - 1, 0, role)) {
            // try playing here
            tmp = new Board(*b);
            q.push(new possibleFuture(tmp, false, &data[(b->dim() - 1) * b->dim()]));
            // add flipped tile to score
            data[(b->dim() - 1) * b->dim()].my = flipTiles(tmp, b->dim() - 1, 0, role);
        }
        if(evalMove(b, b->dim() - 1, b->dim() - 1, role)) {
            // try playing here
            tmp = new Board(*b);
            q.push(new possibleFuture(tmp, false, &data[b->dim() * b->dim() - 1]));
            // add flipped tile to score
            data[b->dim() * b->dim() - 1].my = flipTiles(tmp, b->dim() - 1, b->dim() - 1, role);
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
                }
            }
        }
    }

    char currPlayer;
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    while(duration.count() < maxTime && !q.empty()) {
        // evaluate each possible future and pursue further futures
        p = q.front();
        q.pop();

        currPlayer = p->aiTurn? role: opRole;
        fillLegalities(p->b);

        if(cornersAvailable(p->b, currPlayer)) {
            //Try taking all available corners
            if(evalMove(p->b, 0, 0, currPlayer)) {
                // try playing here
                tmp = new Board(*p->b);
                q.push(new possibleFuture(tmp, !p->aiTurn, p->tileScore));
                // add flipped tiles to score
                if(p->aiTurn) p->tileScore->my += flipTiles(tmp, 0, 0, currPlayer);
                else p->tileScore->op += flipTiles(tmp, 0, 0, currPlayer);
            }
            if(evalMove(p->b, 0, b->dim() - 1, currPlayer)) {
                // try playing here
                tmp = new Board(*p->b);
                q.push(new possibleFuture(tmp, !p->aiTurn, p->tileScore));
                // add flipped tiles to score
                if(p->aiTurn) p->tileScore->my += flipTiles(tmp, 0, b->dim() - 1, currPlayer);
                else p->tileScore->op += flipTiles(tmp, 0, b->dim() - 1, currPlayer);
            }
            if(evalMove(p->b, b->dim() - 1, 0, currPlayer)) {
                // try playing here
                tmp = new Board(*p->b);
                q.push(new possibleFuture(tmp, !p->aiTurn, p->tileScore));
                // add flipped tiles to score
                if(p->aiTurn) p->tileScore->my += flipTiles(tmp, b->dim() - 1, 0, currPlayer);
                else p->tileScore->op += flipTiles(tmp, b->dim() - 1, 0, currPlayer);
            }
            if(evalMove(p->b, b->dim() - 1, b->dim() - 1, currPlayer)) {
                // try playing here
                tmp = new Board(*p->b);
                q.push(new possibleFuture(tmp, !p->aiTurn, p->tileScore));
                // add flipped tiles to score
                if(p->aiTurn) p->tileScore->my += flipTiles(tmp, b->dim() - 1, b->dim() - 1, currPlayer);
                else p->tileScore->op += flipTiles(tmp, b->dim() - 1, b->dim() - 1, currPlayer);
            }
        } else {
            for (int row = 0; row < b->dim(); row++) {
                for (int col = 0; col < b->dim(); col++) {
                    // try all available moves
                    if(evalMove(p->b, row, col, currPlayer)) {
                        // try playing here
                        tmp = new Board(*p->b);
                        q.push(new possibleFuture(tmp, !p->aiTurn, p->tileScore));
                        // add flipped tiles to score
                        if(p->aiTurn) p->tileScore->my += flipTiles(tmp, row, col, currPlayer);
                        else p->tileScore->op += flipTiles(tmp, row, col, currPlayer);
                    }
                }
            }
        }

        delete p;

        // end the loop once too much time has passed
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    }

    // clean possible futures
    while(!q.empty()) {
        delete q.front();
        q.pop();
    }

    // find tile with desired score ratio and return its coordinates
    int best = -1, diff, i = 0;

    //find the first valid move
    while(best == -1) {
        if(data[i].my != 0) {
            best = i;
            diff = data[best].my - data[best].op;
        }
        i++;
    }

    for(; i < data.size(); i++) {
        if(data[i].my - data[i].op > diff) {
            best = i;
            diff = data[best].my - data[best].op;
        }
    }

    return std::make_pair(best / b->dim(), best % b->dim());
}
