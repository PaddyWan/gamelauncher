#include "GUI.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>

void GUI::run()
{
}
Fl_Box* label;
Fl_Button* btn1;
Fl_Button* btn2;
Fl_Button* btn3;
void GUI::set_label()
{
    label->label(entryarray[selected].name.c_str());
    btn1->show();
    if(entryarray[selected].directory.empty())
    {
	btn2->hide();
	btn3->hide();
    }
    else
    {
	btn2->show();
	btn3->show();
    }
}
void list_cb(Fl_Widget* w, void* t)
{
    Fl_Hold_Browser* browser = reinterpret_cast<Fl_Hold_Browser*>(w);
    reinterpret_cast<GUI*>(t)->set_select(browser->value()-1);
}
void button1_cb(Fl_Widget*, void* t)
{
    reinterpret_cast<GUI*>(t)->push_play();
}
void button2_cb(Fl_Widget*, void* t)
{
    reinterpret_cast<GUI*>(t)->push_cache();
}
void button3_cb(Fl_Widget*, void* t)
{
    reinterpret_cast<GUI*>(t)->push_uncache();
}

void GUI::init()
{
    Fl_Hold_Browser* list = new Fl_Hold_Browser(10, 10, 180, 700);
    for(std::size_t i = 0; i < entryarray.size(); ++i)
	list->add(entryarray[i].name.c_str());
    list->callback(list_cb, this);
    label = new Fl_Box(200, 10, 470, 40, "Select an item");
    btn1 = new Fl_Button(200, 60, 150, 30, "Play");
    btn1->callback(button1_cb, this);
    btn1->hide();
    btn2 = new Fl_Button(200, 100, 150, 30, "Cache All");
    btn2->callback(button2_cb, this);
    btn2->hide();
    btn3 = new Fl_Button(200, 140, 150, 30, "Uncache");
    btn3->callback(button3_cb, this);
    btn3->hide();
}

void GUI::clickedgame(std::size_t i)
{
}

void GUI::highlightgame(std::size_t i)
{
}
