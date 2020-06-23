//
// Created by Matthew Lourenco on 20/06/2020.
//

#ifndef OTHELLO_AI_H
#define OTHELLO_AI_H

#include "board.h"
#include "boardAnalysis.h"
#include <vector>

struct scores {
    int my, op; // current score of ai(my) and player(op)

    scores(): my(0), op(0) {}
};

struct possibleFuture {
    Board *b;
    bool aiTurn;
    scores *tileScore;

    possibleFuture(Board *b_, bool aiTurn_, scores *tileScore_): b(b_), aiTurn(aiTurn_), tileScore(tileScore_) {}
    ~possibleFuture() {delete b;}
};

// AI that is restricted to one second of time to make decisions
class Ai {
    Board *b; // pointer to the current game board
    char role, opRole; // P1 or P2
    std::vector<scores> data; // calculated best scores for each player per space

public:
    /* Constructor */
    Ai(Board *b_, char role_);

    /* Destructor */
    ~Ai();

    /* Methods */
    // Finds the best possible move in one second of time
    std::pair<int, int> findBestMove();
};


#endif //OTHELLO_AI_H
