//
// Created by Matthew Lourenco on 20/06/2020.
//

#ifndef OTHELLO_BOARD_H
#define OTHELLO_BOARD_H

#define P1 'B'
#define P2 'W'
#define EMPTY ' '

class Board {
    // board that stores the tiles
    std::vector<std::vector<char>> board;

    // size of one side of the square board, must be even
    unsigned size;

public:
    /* Constructors */
    Board(int size);

    /* Destructors */
    ~Board();

    /* Accessors */

    // Inputs: row and column
    // Output: tile by reference
    // Getter and setter for a tile on the board
    char & at(int row, int col);

    /* Methods */

    // Inputs: row and column
    // Output: true if row and col non-negative and less than size
    bool inBounds(int row, int col);

    /* Operator overloads */

    // Print board to the terminal
    friend ostream& operator<<(ostream& os, const Board& b);
};


#endif //OTHELLO_BOARD_H
