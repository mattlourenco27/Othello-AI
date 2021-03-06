//
// Created by Matthew Lourenco on 03/07/2020.
//

#ifndef REVERSII_GAMELOGIC_H
#define REVERSII_GAMELOGIC_H

#include "ai.h"
#include "board.h"
#include <gtkmm.h>

// struct to keep track of legal placements of chips
struct plegal {
    bool p1legal, p2legal;

    plegal(): p1legal(false), p2legal(false) {}
};

class GameLogic {
    //timeout between end of player turn and beginning of ai turn
    static const int ai_timeout = 400;

    // app window
    Gtk::Window* pWindow;

    // radio buttons
    Gtk::RadioButton *b_start, *w_start;

    // piece count label
    Gtk::Label* pPieceCount;

    // board size
    int b_size;

    // board object pointer
    Board* b;

    // ai opponent pointer
    Ai* ai;

    // colours representing each player
    char player, cpu;

    // turn tracker
    bool cpuTurn;

    // legal move tracker
    std::vector<plegal> ghostChips;

    // fills the ghost chips by determining legal moves for each player
    void fillGhosts();

    // drawing area override function
    bool draw(const Cairo::RefPtr<Cairo::Context> &cr, Gtk::DrawingArea *pArea);

    // handler for the ai's turn
    bool do_ai_turn(Gtk::DrawingArea *pArea);

    // handler for press of the drawing area
    bool on_drawing_area_pressed(GdkEventButton * event, Gtk::DrawingArea *pArea);

    // restart the game
    void on_restart_clicked(Gtk::DrawingArea *pArea);

    // delete the window
    void on_quit_clicked();

    // update the stats
    void update_stats();

public:
    /* Constructor */
    // finish construction of the window and connect signals
    explicit GameLogic(const Glib::RefPtr<Gtk::Builder> & refBuilder);

    /* Destructor */
    ~GameLogic();

    // return a pointer to the window
    Gtk::Window* window();
};

#endif //REVERSII_GAMELOGIC_H
