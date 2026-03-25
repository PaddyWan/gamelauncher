#include "GUI.hpp"

#include <thread>

#include <libxdgbasedir/libxdgbasedir.h>

#include "execute.hpp"
#include "texture.hpp"
#include "cache.hpp"

GUI::GUI()
{
    entryarray = parse_config();
    init();
}
void GUI::quit()
{
    done = true;
}
bool GUI::isDone()
{
    return done;
}
void GUI::set_select(std::size_t i)
{
    selected = i;
    set_label();
}
void GUI::push_play()
{
    execute(entryarray[selected].launch);
}
void GUI::push_cache()
{
    std::thread t([this]()
    {
	hasclicked_cache[0] = true;
	load_cache_files(entryarray[selected].directory);
	hasclicked_cache[0] = false;
    });
    t.detach();
}
void GUI::push_uncache()
{
    std::thread t([this]()
    {
	hasclicked_cache[1] = true;
	unload_cache_files(entryarray[selected].directory);
	hasclicked_cache[1] = false;
    });
    t.detach();
}
