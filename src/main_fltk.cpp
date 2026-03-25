#include "GUI.hpp"
#include "arg.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>

// Main code
int main(int argc, char** argv)
{
    {
        const int r = parse_arg(argc, argv);
        if(r == 1)
            return 0;
        if(r != 0)
            return r;
    }
    Fl_Window* win = new Fl_Window(680, 720, "gamelauncher");
    GUI gui;
    win->end();
    win->show();
    Fl::run();
    return 0;
}
