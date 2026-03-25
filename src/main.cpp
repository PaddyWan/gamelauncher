#include "GUI.hpp"
#include "arg.hpp"

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
    GUI gui;
    while(!gui.isDone())
    {
	gui.run();
    }
    return 0;
}
