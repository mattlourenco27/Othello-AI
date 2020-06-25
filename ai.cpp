//
// Created by Matthew Lourenco on 20/06/2020.
//

#include "ai.h"
#include <chrono>
#include <queue>

/* Constructor */
Ai::Ai(Board *b_, char role_): b(b_), aiRole(role_), opRole((role_ == P1)? P2: P1) {
    data = std::vector<scores>(b->dim() * b->dim());
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
    bool cornersAvail = false;

    // clear scores
    for(auto & score : data) {
        score.my = 0;
        score.op = 0;
    }

    //Save start time
    auto start = std::chrono::high_resolution_clock::now(), stop = start;
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    const double maxTime = 1000;

    //begin check

    //Try taking all available corners
    if(evalMove(b, 0, 0, aiRole)) {
        cornersAvail = true;
        // try playing here
        tmp = new Board(*b);
        q.push(new possibleFuture(tmp, false, &data[0]));
        // add flipped tile to score
        data[0].my = flipTiles(tmp, 0, 0, aiRole);
    }
    if(evalMove(b, 0, b->dim() - 1, aiRole)) {
        cornersAvail = true;
        // try playing here
        tmp = new Board(*b);
        q.push(new possibleFuture(tmp, false, &data[b->dim() - 1]));
        // add flipped tile to score
        data[b->dim() - 1].my = flipTiles(tmp, 0, b->dim() - 1, aiRole);
    }
    if(evalMove(b, b->dim() - 1, 0, aiRole)) {
        cornersAvail = true;
        // try playing here
        tmp = new Board(*b);
        q.push(new possibleFuture(tmp, false, &data[(b->dim() - 1) * b->dim()]));
        // add flipped tile to score
        data[(b->dim() - 1) * b->dim()].my = flipTiles(tmp, b->dim() - 1, 0, aiRole);
    }
    if(evalMove(b, b->dim() - 1, b->dim() - 1, aiRole)) {
        cornersAvail = true;
        // try playing here
        tmp = new Board(*b);
        q.push(new possibleFuture(tmp, false, &data[b->dim() * b->dim() - 1]));
        // add flipped tile to score
        data[b->dim() * b->dim() - 1].my = flipTiles(tmp, b->dim() - 1, b->dim() - 1, aiRole);
    }
    if(!cornersAvail) {
        for (int row = 0; row < b->dim(); row++) {
            for (int col = 0; col < b->dim(); col++) {
                // try all available moves
                if(evalMove(b, row, col, aiRole)) {
                    // try playing here
                    tmp = new Board(*b);
                    q.push(new possibleFuture(tmp, false, &data[row*b->dim() + col]));
                    // add flipped tile to score
                    data[row*b->dim() + col].my = flipTiles(tmp, row, col, aiRole);
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

        currPlayer = p->aiTurn? aiRole: opRole;
        cornersAvail = false;

        //Try taking all available corners
        if(evalMove(p->b, 0, 0, currPlayer)) {
            cornersAvail = true;

            // try playing here
            tmp = new Board(*p->b);
            q.push(new possibleFuture(tmp, !p->aiTurn, p->tileScore));

            // add flipped tiles to score
            if(p->aiTurn) p->tileScore->my += flipTiles(tmp, 0, 0, currPlayer);
            else p->tileScore->op += flipTiles(tmp, 0, 0, currPlayer);
        }
        if(evalMove(p->b, 0, b->dim() - 1, currPlayer)) {
            cornersAvail = true;

            // try playing here
            tmp = new Board(*p->b);
            q.push(new possibleFuture(tmp, !p->aiTurn, p->tileScore));

            // add flipped tiles to score
            if(p->aiTurn) p->tileScore->my += flipTiles(tmp, 0, b->dim() - 1, currPlayer);
            else p->tileScore->op += flipTiles(tmp, 0, b->dim() - 1, currPlayer);
        }
        if(evalMove(p->b, b->dim() - 1, 0, currPlayer)) {
            cornersAvail = true;

            // try playing here
            tmp = new Board(*p->b);
            q.push(new possibleFuture(tmp, !p->aiTurn, p->tileScore));

            // add flipped tiles to score
            if(p->aiTurn) p->tileScore->my += flipTiles(tmp, b->dim() - 1, 0, currPlayer);
            else p->tileScore->op += flipTiles(tmp, b->dim() - 1, 0, currPlayer);
        }
        if(evalMove(p->b, b->dim() - 1, b->dim() - 1, currPlayer)) {
            cornersAvail = true;

            // try playing here
            tmp = new Board(*p->b);
            q.push(new possibleFuture(tmp, !p->aiTurn, p->tileScore));

            // add flipped tiles to score
            if(p->aiTurn) p->tileScore->my += flipTiles(tmp, b->dim() - 1, b->dim() - 1, currPlayer);
            else p->tileScore->op += flipTiles(tmp, b->dim() - 1, b->dim() - 1, currPlayer);
        }
        if(!cornersAvail) {
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
        if(data[i].my > 0) {
            best = i;
            diff = data[best].my - data[best].op;
        }
        i++;
        if(i == data.size()) throw no_generation();
    }

    for(; i < data.size(); i++) {
        if(data[i].my - data[i].op > diff) {
            best = i;
            diff = data[best].my - data[best].op;
        }
    }

    // print data
    std::cout << "[";
    for(int d = 0; d < data.size(); d++) {
        std::cout << data[d];
        if(d + 1 != data.size()) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    std::cout << "The best solution was at data " << best << std::endl;
    return std::make_pair(best / b->dim(), best % b->dim());
}
