//
// Created by Matthew Lourenco on 03/07/2020.
//

#ifndef REVERSII_GAMELOGIC_H
#define REVERSII_GAMELOGIC_H

#include <gtkmm.h>

namespace glc {
    // app window
    extern Gtk::Window* pWindow;

    // start the game
    int begin();

    // clean up instantiated window
    void clean();
    void draw_background();

    // delete the window
    void on_quit_clicked();

    // finish construction of the window and connect signals
    void set_up(const Glib::RefPtr<Gtk::Builder> & refBuilder);
}

#endif //REVERSII_GAMELOGIC_H
