//
// Created by Matthew Lourenco on 03/07/2020.
//

#include "ai.h"
#include "boardAnalysis.h"
#include <cairomm/context.h>
#include "gameLogic.h"

/* Constructor */
// finish construction of the window and connect signals
GameLogic::GameLogic(const Glib::RefPtr<Gtk::Builder> &refBuilder) {
    pWindow = nullptr;
    b_size = 8;

    // set up the game board for the first time
    b = new Board(b_size);

    // set up ghost chips
    ghostChips = std::vector<plegal>(b_size * b_size);
    fillGhosts();

    // when the app launches the player is P1 by default
    player = P1;
    cpu = P2;
    cpuTurn = false;

    // get the window
    refBuilder->get_widget("app_window", pWindow);
    if(pWindow) {
        // connect the quit signal
        Gtk::Button* pButton = nullptr;
        refBuilder->get_widget("quit_button", pButton);
        if(pButton) pButton->signal_clicked().connect( sigc::mem_fun(*this, &GameLogic::on_quit_clicked));

        // draw the game board
        Gtk::DrawingArea* pArea = nullptr;
        refBuilder->get_widget("game_board", pArea);
        if(pArea) pArea->signal_draw().connect(sigc::bind (sigc::mem_fun (*this, &GameLogic::draw), pArea));

    }
}

/* Destructor */
GameLogic::~GameLogic() {
    delete pWindow;
    delete b;
}

// return a pointer to the window
Gtk::Window* GameLogic::window() {
    return pWindow;
}

// fills the ghost chips by determining legal moves for each player
void GameLogic::fillGhosts() {
    for(int row = 0; row < b_size; row++) {
        for(int col = 0; col < b_size; col++) {
            ghostChips[row * b_size + col].p1legal = evalMove(b, row, col, P1);
            ghostChips[row * b_size + col].p2legal = evalMove(b, row, col, P2);
        }
    }
}

// start the game
int GameLogic::begin() {

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
    return EXIT_SUCCESS;
}

// drawing area override function
bool GameLogic::draw(const Cairo::RefPtr<Cairo::Context> &cr, Gtk::DrawingArea *pArea)
{
    Gtk::Allocation allocation = pArea->get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    // draw the background
    cr->set_source_rgb(0.0625, 0.539, 0); // Pool table green
    cr->paint();

    // draw separating lines for tiles
    cr->save();
    double col_width = width * (1.0 / b_size), row_height = height * (1.0 / b_size);
    cr->set_source_rgb(0.746, 0.746, 0.746); // silver
    cr->set_line_width(2.0);
    cr->set_line_cap(Cairo::LINE_CAP_BUTT);
    for(int col = 1; col < b_size; col++) { // column separators
        cr->move_to(col * col_width, 0);
        cr->line_to(col * col_width, height);
    }
    for(int row = 1; row < b_size; row++) { // row separators
        cr->move_to(0, row * row_height);
        cr->line_to(width, row * row_height);
    }
    cr->stroke();
    cr->restore();

    // draw the outline
    cr->save();
    cr->set_source_rgb(0.539, 0.379, 0); // Wood brown
    cr->set_line_width(10.0);
    cr->set_line_cap(Cairo::LINE_CAP_BUTT);
    cr->set_line_join(Cairo::LINE_JOIN_MITER);
    cr->move_to(0,0);
    cr->line_to(0, height);
    cr->line_to(width, height);
    cr->line_to(width, 0);
    cr->close_path();
    cr->stroke();
    cr->restore();

    // draw the pieces
    cr->save();
    cr->set_line_width(1.0);
    bool p1turn = !(cpuTurn ^ (cpu == P1));
    for(int row = 0; row < b_size; row++) {
        for(int col = 0; col < b_size; col++) {
            char img = b->at(row, col);
            if(img == P1) {
                cr->set_source_rgb(0, 0, 0);
                cr->arc((col + 0.5) * col_width, (row + 0.5) * row_height, col_width * 0.4, 0, 2 * M_PI);
                cr->fill();
            }
            if(img == P2) {
                cr->set_source_rgb(1.0, 1.0, 1.0);
                cr->arc((col + 0.5) * col_width, (row + 0.5) * row_height, col_width * 0.4, 0, 2 * M_PI);
                cr->fill();
            }
            if(ghostChips[b_size * row + col].p1legal && p1turn) {
                cr->set_source_rgba(0, 0, 0, 0.2);
                cr->arc((col + 0.5) * col_width, (row + 0.5) * row_height, col_width * 0.4, 0, 2 * M_PI);
                cr->fill();
            }
            if(ghostChips[b_size * row + col].p2legal && !p1turn) {
                cr->set_source_rgba(1.0, 1.0, 1.0, 0.2);
                cr->arc((col + 0.5) * col_width, (row + 0.5) * row_height, col_width * 0.4, 0, 2 * M_PI);
                cr->fill();
            }
        }
    }
    cr->restore();

    return true;
}

// delete the window
void GameLogic::on_quit_clicked() {
    if(pWindow) pWindow->hide(); //hide() will cause main::run() to end.
}
