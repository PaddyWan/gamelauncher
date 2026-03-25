#include "GUI.hpp"
#include "arg.hpp"

#include <forms.h>

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
    int nargc = 1;
    char* nargv[] = {argv[0], nullptr};
    fl_initialize(&nargc, nargv, "gamelauncher", 0, 0);
    GUI gui;
    while(!gui.isDone())
    {
	gui.run();
    }
    return 0;
}
