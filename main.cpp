//
// Created by Matthew Lourneco on 20/06/2020.
//

#include "ai.h"
#include "board.h"
#include "boardAnalysis.h"
#include <limits.h>

int main(int argc, char **argv)
{
    int n = 0;
    Board *b = nullptr;
    char player, cpu;
    bool cpuTurn;

    // Get board size
    do {
        std::cout << "Enter the board dimension: ";
        std::cin >> n;
        if(std::cin.fail()) std::cout << "Please enter an integer" << std::endl;
        else {
            try {
                // Initialize the board
                b = new Board(n);
            } catch(bad_size& e) {
                std::cout << e.what() << std::endl << "Size must be even and between 8 and 26" << std::endl;
            } catch(std::exception& e) {
                std::cout << e.what() << std::endl;
                return EXIT_FAILURE;
            }
        }

        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    } while(b == nullptr);

    // Get player colour
    do {
        std::cout << "Choose your colour (B/W): ";
        std::cin >> player;
        player = toupper(player);
        if(player == P1) {
            cpu = P2;
            cpuTurn = false;
            break;
        } else if(player == P2) {
            cpu = P1;
            cpuTurn = true;
            break;
        } else {
            std::cout << "Please type 'B' or 'W'" << std::endl;
        }

        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    } while(true);
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');

    // Initial print
    std::cout << *b;
    // std::cout << "Board size: " << n << std::endl << "Player: " << player << std::endl << "cpu: " << cpu << std::endl;

    // Create the ai
    Ai ai(b, cpu);

    // Game loop
    while(availableMove(b, player) || availableMove(b, cpu)) {
        if(cpuTurn) {
            //First check if the computer has a valid move
            if(availableMove(b, cpu)) {
                std::pair<int, int> best;
                best = ai.findBestMove();

                if(!evalMove(b, best.first, best.second, cpu)) {
                    std::cout << "CPU made an illegal move" << std::endl;
                }

                //Flip tiles for cpu
                std::cout << "Computer places " << cpu << " at " << (char)(best.first + 'a') << (char)(best.second + 'a') << ".\n";
                flipTiles(b, best.first, best.second, cpu);
                std::cout << *b;
            } else {
                std::cout << cpu << " player has no valid move.\n";
            }
        } else {
            //First check if the user has a valid move
            if(availableMove(b, player)) {
                char row, col;
                //Get user move
                do {
                    std::cout << "Enter move for colour " << player << " (RowCol): ";
                    std::cin >> row >> col;

                    // Find if the move is valid
                    if(evalMove(b, row - 'a', col - 'a', player)) {
                        //Flip tiles for user
                        flipTiles(b, row - 'a', col - 'a', player);
                        std::cout << *b;
                        break;
                    } else {
                        printf("Invalid move.\n");
                    }
                } while(true);
            } else {
                std::cout << player << " player has no valid move.\n";
            }
        }

        //Flip turn to other player
        cpuTurn = !cpuTurn;
    }

    //Count tiles of each player
    int B = countColour(b, P1);
    int W = countColour(b, P2);

    if(B > W) {
        std::cout << "B player wins.\n";
    } else if(W > B){
        std::cout << "W player wins.\n";
    } else {
        std::cout << "It's a tie.\n";
    }
    std::cout << "B: " << B << " W: " << W << std::endl;

    std::cin >> player;
    delete b;
    return EXIT_SUCCESS;
}
