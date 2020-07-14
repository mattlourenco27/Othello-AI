//
// Created by Matthew Lourenco on 03/07/2020.
//

#include "boardAnalysis.h"
#include <cairomm/context.h>
#include "gameLogic.h"

/* Constructor */
// finish construction of the window and connect signals
GameLogic::GameLogic(const Glib::RefPtr<Gtk::Builder> &refBuilder):
    pWindow(nullptr),
    b_start(nullptr),
    w_start(nullptr),
    b_size(8),
    b(new Board(b_size)),
    ghostChips(b_size * b_size),
    player(P1), // when the app launches the player is P1 by default
    cpu(P2),
    cpuTurn(false),
    ai(new Ai(b, P2))
{
    // set up ghost chips
    fillGhosts();

    // get the window
    refBuilder->get_widget("app_window", pWindow);
    if(pWindow) {
        // connect the quit signal
        Gtk::Button* pButton = nullptr;
        refBuilder->get_widget("quit_button", pButton);
        if(pButton) pButton->signal_clicked().connect( sigc::mem_fun(*this, &GameLogic::on_quit_clicked));

        // get the radio buttons
        refBuilder->get_widget("radio_black", b_start);
        refBuilder->get_widget("radio_white", w_start);

        // draw the game board
        Gtk::DrawingArea* pArea = nullptr;
        refBuilder->get_widget("game_board", pArea);
        if(pArea) {
            pArea->signal_draw().connect(sigc::bind(sigc::mem_fun(*this, &GameLogic::draw), pArea));
            pArea->add_events(Gdk::BUTTON_PRESS_MASK);
            pArea->signal_button_press_event().connect(sigc::bind(sigc::mem_fun(*this, &GameLogic::on_drawing_area_pressed), pArea));

            // connect the restart signal
            pButton = nullptr;
            refBuilder->get_widget("restart_button", pButton);
            if(pButton) pButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &GameLogic::on_restart_clicked), pArea));
        }
    }
}

/* Destructor */
GameLogic::~GameLogic() {
    delete pWindow;
    delete b_start;
    delete w_start;
    delete b;
    delete ai;
    ghostChips.clear();
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

// handler for the ai's turn
bool GameLogic::do_ai_turn(Gtk::DrawingArea *pArea) {
    if(!cpuTurn) return false;

    // drawing area must be redrawn in all cases forward
    pArea->queue_draw();

    if(!availableMove(b, cpu)) { // cpu has no move, show possible player moves instead
        cpuTurn = false;
        return false;
    }

    // choose best move and flip tiles for cpu
    std::pair<int, int> best = ai->findBestMove();
    flipTiles(b, best.first, best.second, cpu);
    fillGhosts();

    cpuTurn = !availableMove(b, player) && availableMove(b, cpu);
    return cpuTurn; // on return true timeout continues
}

// handler for press of the drawing area
bool GameLogic::on_drawing_area_pressed(GdkEventButton * event, Gtk::DrawingArea *pArea) {
    if(event->type == GDK_BUTTON_PRESS && event->button == 1) {
        Gtk::Allocation allocation = pArea->get_allocation();
        const int width = allocation.get_width();
        const int height = allocation.get_height();
        int col = event->x * b_size / width, row = event->y * b_size / height;
        if(!cpuTurn && evalMove(b, row, col, player)) {
            flipTiles(b, row, col, player);
            fillGhosts();
            cpuTurn = true;
            pArea->queue_draw();

            Glib::signal_timeout().connect(sigc::bind(sigc::mem_fun(*this, &GameLogic::do_ai_turn), pArea), ai_timeout);
        }
    }

    return true;
}

// restart the game
void GameLogic::on_restart_clicked(Gtk::DrawingArea *pArea) {
    if(!b_start->get_active() && !w_start->get_active()) return;

    delete b;
    b = new Board(b_size);
    pArea->queue_draw();

    if(b_start->get_active()) {
        player = P1;
        cpu = P2;
        cpuTurn = false;
    } else {
        player = P2;
        cpu = P1;
        cpuTurn = true;
        Glib::signal_timeout().connect(sigc::bind(sigc::mem_fun(*this, &GameLogic::do_ai_turn), pArea), ai_timeout);
    }

    delete ai;
    ai = new Ai(b, cpu);
    fillGhosts();
}

// delete the window
void GameLogic::on_quit_clicked() {
    if(pWindow) pWindow->hide(); //hide() will cause main::run() to end.
}
