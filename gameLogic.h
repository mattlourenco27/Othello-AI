//
// Created by Matthew Lourenco on 03/07/2020.
//

#ifndef REVERSII_GAMELOGIC_H
#define REVERSII_GAMELOGIC_H

#include "board.h"
#include <gtkmm.h>

class GameLogic {
    // board size
    int b_size;

    // board object pointer
    Board* b;

public:
    /* Constructor */
    // finish construction of the window and connect signals
    explicit GameLogic(const Glib::RefPtr<Gtk::Builder> & refBuilder);

    /* Destructor */
    ~GameLogic();

    // app window
    Gtk::Window* pWindow;

    // start the game
    int begin();

    // drawing area override function
    bool draw(const Cairo::RefPtr<Cairo::Context> &cr, Gtk::DrawingArea *pArea);

    // delete the window
    void on_quit_clicked();
};

#endif //REVERSII_GAMELOGIC_H
