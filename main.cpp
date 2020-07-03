//
// Created by Matthew Lourneco on 20/06/2020.
//

#include "gameLogic.h"
#include <gtkmm.h>
#include <iostream>

int main(int argc, char **argv) {
    // Icon made by Pixelmeetup from www.flaticon.com
    Glib::RefPtr<Gtk::Application> app =
            Gtk::Application::create(argc, argv, "com.github.mattlourenco27.reversi-ai");
    int exit_state = EXIT_FAILURE;

    //Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("../ui/ui.glade");
    } catch(const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    } catch(const Glib::MarkupError& ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    } catch(const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    // finish construction and signal connection
    GameLogic gameL(refBuilder);

    if(gameL.pWindow) {
        // enter application loop
        exit_state = app->run(*gameL.pWindow);
    }

    return exit_state;
}
