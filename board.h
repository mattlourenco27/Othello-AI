//
// Created by Matthew Lourenco on 20/06/2020.
//

#ifndef OTHELLO_BOARD_H
#define OTHELLO_BOARD_H

#include <exception>
#include <iostream>
#include <vector>

#define P1 'B'
#define P2 'W'
#define EMPTY 'U'

struct tile {
    char img;
    bool p1legal, p2legal, certain;

    tile(): img('U'), p1legal(false), p2legal(false), certain(false) {}
    void fill(char colour);
};

// Thrown when board initialization size is odd or less than 8 or greater than 26
class bad_size: public std::exception {
    int size;

public:
    bad_size(int size_): size(size_) {}

    int getSize() {return size;}

    virtual const char* what() const throw() {
        return "board initialization size is invalid";
    }
};

// Thrown when an attempt to access a tile off of the board is made
class out_of_bounds: public std::exception {
    int size;
    char row, col;

public:
    out_of_bounds(int size_, int row_, int col_): size(size_), row(row_ + 'a'), col(col_ + 'a') {}
    out_of_bounds(int size_, char row_, char col_): size(size_), row(row_), col(col_) {}

    int getSize() {return size;}
    char getRow() {return row;}
    char getCol() {return col;}

    virtual const char* what() const throw() {
        return "coordinates are outside of the board";
    }
};

// Class to make access of the 2D board easier
class Board {
    // board that stores the tiles
    std::vector<std::vector<tile>> board;

    // size of one side of the square board, must be even
    unsigned size;

public:
    /* Constructor */
    Board(int _size);

    /* Destructors */
    ~Board();

    /* Accessors */

    // Inputs: row and column
    // Output: tile by reference
    // Getter and setter for a tile on the board
    tile & at(int row, int col);
    tile & at(char row, char col);
    // Getter only variant
    tile at(int row, int col) const;
    tile at(char row, char col) const;

    // getter for size
    unsigned dim() const {return size;}

    /* Methods */

    // Inputs: row and column
    // Output: true if row and col non-negative and less than size
    bool inBounds(int row, int col) const;

    // Prints the board
    void print() const;

    /* Operator overloads */

    // Print board to the terminal
    friend std::ostream& operator<<(std::ostream& os, const Board& b);
};


#endif //OTHELLO_BOARD_H
