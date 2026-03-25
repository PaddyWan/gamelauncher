#include "GUI.hpp"
#include <forms.h>

//FL_FORM* form;
FL_OBJECT* list;
FL_OBJECT* label;
FL_OBJECT* btn1;
FL_OBJECT* btn2;
FL_OBJECT* btn3;
void GUI::run()
{
    FL_OBJECT* activated = fl_do_forms();
    if(activated == list)
	set_select(fl_get_browser(list)-1);
    else if(activated == btn1)
	push_play();
    else if(activated == btn2)
	push_cache();
    else if(activated == btn3)
	push_uncache();
}
void GUI::set_label()
{
    fl_set_object_label(label, entryarray[selected].name.c_str());
    fl_show_object(btn1);
    if(entryarray[selected].directory.empty())
    {
	fl_hide_object(btn2);
	fl_hide_object(btn3);
    }
    else
    {
	fl_show_object(btn2);
	fl_show_object(btn3);
    }
}
void GUI::init()
{
    FL_FORM* form = fl_bgn_form(FL_UP_BOX, 680, 720);
    list = fl_add_browser(FL_HOLD_BROWSER, 10, 10, 180, 700, "Items");
    for(std::size_t i = 0; i < entryarray.size(); ++i)
	fl_add_browser_line(list, entryarray[i].name.c_str());
    label = fl_add_box(FL_DOWN_BOX, 200, 10, 470, 40, "Select an item");
    btn1 = fl_add_button(FL_NORMAL_BUTTON, 200, 60, 150, 30, "Play");
    btn2 = fl_add_button(FL_NORMAL_BUTTON, 200, 100, 150, 30, "Cache All");
    btn3 = fl_add_button(FL_NORMAL_BUTTON, 200, 140, 150, 30, "Uncache");
    fl_hide_object(btn1);
    fl_hide_object(btn2);
    fl_hide_object(btn3);
    fl_end_form();
    fl_show_form(form, FL_PLACE_CENTER, FL_FULLBORDER, "gamelauncher");
}
void GUI::clickedgame(std::size_t i)
{
}
void GUI::highlightgame(std::size_t i)
{
}
