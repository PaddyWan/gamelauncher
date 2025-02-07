#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <imgui.h>
using std::string;
using std::vector;

struct entry
{
    string id;// sort by this
    string name;
    string description; // API mobygames.com/game/
    vector<string> launch;
    string iconname;
    ImTextureID icon_texture = 0; // API steamgriddb.com/
};

vector<entry> parse_config();

#endif//CONFIG_HPP
