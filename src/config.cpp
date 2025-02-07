#include "config.hpp"

#include <algorithm>
#include <set>
#include <fstream>
#include <filesystem>
//#include <nlohmann/json.hpp>
#include <libxdgbasedir/libxdgbasedir.h>

using std::set;
using std::string_view;

constexpr static string blank_string = " \t";
string_view trim_whitespace(string_view line)
{
    line.remove_prefix(std::min(line.find_first_not_of(blank_string), line.size()));
    auto trim_pos = line.find_last_not_of(blank_string);
    if(trim_pos != line.npos)
        line.remove_suffix(line.size() - trim_pos - 1);
    return line;
}

bool is_surrounded_square_brackets(string_view line)
{
    return !line.empty() && line.front() == '[' && line.back() == ']';
}
string_view get_within_square_brackets(string_view line)
{
    return trim_whitespace({line.begin() + 1, line.end() - 1});
}
bool is_only_begin_square_bracket(string_view line)
{
    return line.size() == 1 && line.front() == '[';
}
bool is_only_end_square_bracket(string_view line)
{
    return line.size() == 1 && line.front() == ']';
}
using std::pair;
pair<string_view, string_view> get_key_value(string_view line)
{
    auto key_pos = line.find_first_of('=');
    if(key_pos != line.npos)
        return {trim_whitespace({line.begin(), line.begin() + key_pos}), trim_whitespace({line.begin() + key_pos + 1, line.end()})};
    else
        return {{}, {}};
}

using std::ifstream;

bool getline_trim(ifstream& stream, string& str, string_view& view)
{
    view = string_view();
    str.clear();
    const bool a = static_cast<bool>(std::getline(stream, str));
    if(a)
        view = trim_whitespace(str);
    return a;
}

vector<entry> parse_config()
{
    vector<entry> ret;
    set<string> alreadyhad;
    // using json = nlohmann::json;
    // ifstream f(xdg::config::home() + "/gamelauncher/list.json");
    // json data = json::parse(f);
    // for(std::size_t i = 0; i < data.size(); ++i)
    // {
    // 	bool allfilesfound = true;
    // 	{
    // 	    std::size_t j = 0;
    // 	    while(allfilesfound && j < data[i]["files"].size())
    // 	    {
    // 		allfilesfound = std::filesystem::exists(data[i]["files"][j]);
    // 		++j;
    // 	    }
    // 	}
    // 	if(allfilesfound)
    // 	{
    // 	    vector<string> ls;
    // 	    for(std::size_t j = 0; j < data[i]["launch"].size(); ++j)
    // 		ls.push_back(data[i]["launch"][j]);
    // 	    ret.push_back({data[i]["id"], data[i]["name"], data[i]["description"], std::move(ls), data[i]["icon"]});
    // 	}
    // }
    for(auto const& dir_entry : std::filesystem::directory_iterator(xdg::config::home() + "/gamelauncher/conf"))
    {
        if(dir_entry.path().native().ends_with(".ini"))
        {
            ifstream f(dir_entry.path());
            string line;
            string_view line_view;
            while(getline_trim(f, line, line_view) && !is_surrounded_square_brackets(line_view))
                ;
            while(is_surrounded_square_brackets(line_view))
            {
                entry e;
                e.id = get_within_square_brackets(line_view);
                bool add = true;
                while(getline_trim(f, line, line_view) && !is_surrounded_square_brackets(line_view))
                {
                    auto p = get_key_value(line_view);
                    if(p.first == "name")
                        e.name = p.second;
                    else if(p.first == "icon")
                        e.iconname = p.second;
                    else if(p.first == "description")
                        e.description = p.second;
                    else if(p.first == "launch")
                    {
                        if(is_only_begin_square_bracket(p.second))
                            while(getline_trim(f, line, line_view) && !is_only_end_square_bracket(line_view))
                                e.launch.push_back(string{line_view});
                        else if(is_surrounded_square_brackets(p.second))
                            e.launch.push_back(string{get_within_square_brackets(p.second)});
                        else if(p.second.empty())
                        {
                            getline_trim(f, line, line_view);
                            if(is_only_begin_square_bracket(line_view))
                                while(getline_trim(f, line, line_view) && !is_only_end_square_bracket(line_view))
                                    e.launch.push_back(string{line_view});
                        }
                        else
                            e.launch.push_back(string{p.second});
                    }
                    else if(p.first == "files")
                        add = add && std::filesystem::exists(p.second);
                }
                if(add)
                    if(alreadyhad.insert(e.id).second)//in case we have a steam.ini auto generated and one custom.ini self made
                        ret.push_back(std::move(e));
            }
        }
    }
    std::sort(ret.begin(), ret.end(), [](const entry& lhs, const entry& rhs)
    {
        return lhs.id < rhs.id;
    });
    return ret;
}
