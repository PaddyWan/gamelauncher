#ifndef GUI_HPP
#define GUI_HPP

#include "config.hpp"

class GUI
{
    public:
        GUI();
        void quit();
        bool isDone();
        void minimize();
        void run();
    private:
        void init();
        void clickedgame(std::size_t i);
        void highlightgame(std::size_t i);
        vector<entry> entryarray;
        ImTextureID banner_texture = 0;
        bool done = false;
        bool isopen = true;
        std::size_t selected = static_cast<std::size_t>(-1);
        std::size_t highlight = static_cast<std::size_t>(-1);
        int hasclicked = 0;
};

#endif//GUI_HPP
