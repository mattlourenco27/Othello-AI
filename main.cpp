//
// Created by Matthew Lourneco on 20/06/2020.
//

#include "ai.h"
#include "board.h"
#include "boardAnalysis.h"

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
    std::cout << "Board size: " << n << std::endl << "Player: " << player << std::endl << "cpu: " << cpu << std::endl;

    /*

    while(availableMove(board, n, userColour) || availableMove(board, n, cpuColour)) {
        if(cpuTurn) {
            //First check if the computer has a valid move
            if(availableMove(board, n, cpuColour)) {
                bestX = 0;
                bestY = 0;

                struct rusage usage; // a structure to hold "resource usage" (including time)
                struct timeval start; // will hold the start and end times
                getrusage(RUSAGE_SELF, &usage);
                start = usage.ru_utime;
                double timeStart = start.tv_sec + start.tv_usec / 1000000.0; // in seconds

                // PLACE THE CODE YOU WISH TO TIME HERE

                findBestMove(board, n, cpuColour, userColour, &bestX, &bestY, timeStart);

                //Flip tiles for cpu
                printf("Computer places %c at %c%c.\n", cpuColour, bestX + 'a', bestY + 'a');
                flipTiles(board, n, bestX, bestY, cpuColour);
                printBoard(board, n);
            } else {
                printf("%c player has no valid move.\n", cpuColour);
            }
        } else {
            //First check if the user has a valid move
            if(availableMove(board, n, userColour)) {

                //Get user move
                printf("Enter move for colour %c (RowCol): ", userColour);
                scanf("%c", &newLine); //Catch stray newline
                scanf("%c%c", &c1, &c2);

                //Convert to x and y co-ordinates
                i = c1 - 'a';
                j = c2 - 'a';

                //Find if the move is valid. Forfiet game if it is not valid
                if(evalMove(board, n, i, j, userColour)) {
                    //Flip tiles for user
                    flipTiles(board, n, i, j, userColour);
                    printBoard(board, n);
                } else {
                    printf("Invalid move.\n");
                    break;
                }
            } else {
                printf("%c player has no valid move.\n", userColour);
            }
        }

        //Flip turn to other player
        cpuTurn = !cpuTurn;
    }

    //Count tiles of each player
    int B = countColour(board, n, 'B');
    int W = countColour(board, n, 'W');

    if(B > W) {
        printf("B player wins.\n");
    } else if(W > B){
        printf("W player wins.\n");
    } else {
        printf("It's a tie.\n");
    }
    return 0;
    */
    return EXIT_SUCCESS;
}
