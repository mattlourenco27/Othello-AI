//
// Created by Matthew Lourenco on 20/06/2020.
//

#include "board.h"

/* Constructor */
Board::Board(int size_): size(size_) {
    if(size_ < 8 || size_ % 2 != 0 || size_ > 26) throw bad_size(size_);

    this->size = size_;

    this->board = std::vector<std::vector<char>>(size_, std::vector<char>(size_, EMPTY));

    this->board[size_ >> 1][size_ >> 1] = P2;
    this->board[(size_ >> 1) - 1][(size_ >> 1) - 1] = P2;
    this->board[size_ >> 1][(size_ >> 1) - 1] = P1;
    this->board[(size_ >> 1) - 1][size_ >> 1] = P1;
}

/* Destructors */
Board::~Board() {
    this->board.clear();
}

/* Accessors */

// Inputs: row and column
// Output: tile by reference
// Getter and setter for a tile on the board
char & Board::at(int row, int col) {
    if(row < 0 || row >= this->size || col < 0 || col >= this->size) throw out_of_bounds(this->size, row, col);
    return this->board[row][col];
}
char Board::at(int row, int col) const {
    if(row < 0 || row >= this->size || col < 0 || col >= this->size) throw out_of_bounds(this->size, row, col);
    return this->board[row][col];
}
char & Board::at(char row, char col) {
    return this->at(tolower(row) - 'a', tolower(col) - 'a');
}
char Board::at(char row, char col) const {
    return this->at(tolower(row) - 'a', tolower(col) - 'a');
}

/* Methods */

// Inputs: row and column
// Output: true if row and col non-negative and less than size
bool Board::inBounds(int row, int col) const {
    return row >= 0 && col >= 0 && row < this->size && col < this->size;
}

// Prints the board
void Board::print() const {
    std::cout << *this;
}

/* Operator overloads */

// Print board to the terminal
std::ostream& operator<<(std::ostream& os, const Board& b) {
    // Set up the header
    os << std::endl << "  ";
    for(char col = 'a'; col < 'a' + b.size; col++) {
        os << col << ' ';
    }
    os << std::endl;

    // Display the left-margin and board
    for(char row = 'a'; row < b.size + 'a'; row++) {
        os << row << ' ';

        for(char col = 'a'; col < 'a' + b.size; col++) {
            os << b.at(row, col) << ' ';
        }

        os << std::endl;
    }

    return os;
}