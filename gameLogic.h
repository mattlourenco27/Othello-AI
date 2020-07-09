//
// Created by Matthew Lourenco on 03/07/2020.
//

#ifndef REVERSII_GAMELOGIC_H
#define REVERSII_GAMELOGIC_H

#include "board.h"
#include <gtkmm.h>

struct plegal {
    bool p1legal, p2legal;

    plegal(): p1legal(false), p2legal(false) {}
};

class GameLogic {
    // app window
    Gtk::Window* pWindow;

    // board size
    int b_size;

    // board object pointer
    Board* b;

    // colours representing each player
    char player, cpu;

    // turn tracker
    bool cpuTurn;

    // legal move tracker
    std::vector<plegal> ghostChips;

public:
    /* Constructor */
    // finish construction of the window and connect signals
    explicit GameLogic(const Glib::RefPtr<Gtk::Builder> & refBuilder);

    /* Destructor */
    ~GameLogic();

    // return a pointer to the window
    Gtk::Window* window();

    // fills the ghost chips by determining legal moves for each player
    void fillGhosts();

    // start the game
    int begin();

    // drawing area override function
    bool draw(const Cairo::RefPtr<Cairo::Context> &cr, Gtk::DrawingArea *pArea);

    // delete the window
    void on_quit_clicked();
};

#endif //REVERSII_GAMELOGIC_H
