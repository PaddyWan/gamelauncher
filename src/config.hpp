#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <set>
#include <imgui.h>
using std::string;
using std::vector;
using std::set;

struct entry
{
    string id;// sort by this
    string name;
    string description; // API mobygames.com/game/
    vector<string> launch;
    string iconname;
    ImTextureID icon_texture = 0; // API steamgriddb.com/
    set<string> directory;
};

vector<entry> parse_config();

#endif//CONFIG_HPP
