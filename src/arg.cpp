#include "arg.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

#include <libxdgbasedir/libxdgbasedir.h>

using std::string;
using std::vector;
using std::string_view;

void print_usage(const char* name)
{
    std::cout << "usage: " << name << " [arguments]\n"
              "example: " << name << " --chrootsteam /path/to/chroot/script /path/to/steamapp /path/to/other/steamapp\n"
              "\n"
              "arguments:\n"
              "  -h                                          this\n"
              "  --chrootsteam chrootscript [steamapppaths]  generate chrootstream\n"
              "  -e/--exit                                   exit the program\n";
}

string_view get_last_parameter(const string& str)
{
    auto const pos1 = str.find_last_of('"');
    if(pos1 == str.npos || pos1 == 0)
        return {};
    auto const pos2 = str.find_last_of('"', pos1 - 1);
    if(pos2 == str.npos)
        return {};
    return {str.begin() + pos2 + 1, str.begin() + pos1};
}

void chrootsteam(const string& script, const vector<string>& steamapps)
{
    string outputfile = xdg::config::home() + "/gamelauncher/conf/chrootsteam.ini";
    std::ofstream fout(outputfile);
    for(const string& steamappdir : steamapps)
        for(auto const& dir_entry : std::filesystem::directory_iterator(steamappdir))
        {
            if(dir_entry.path().native().ends_with(".acf"))
            {
                string appid;
                string name;
                {
                    std::ifstream f(dir_entry.path());
                    string line;
                    while((appid.empty() || name.empty()) && getline(f, line))
                    {
                        if(line.find("appid") != line.npos)
                            appid = get_last_parameter(line);
                        else if(line.find("name") != line.npos)
                            name = get_last_parameter(line);
                    }
                }
                if(!(appid == "1070560" ||
                        appid == "1391110" ||
                        appid == "1628350" ||
                        appid == "1887720" ||
                        appid == "2348590" ||
                        appid == "1493710" ||
                        appid == "2805730" ||
                        appid == "228980"))
                {
                    string lowercase = name;
                    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), [](unsigned char c)
                    {
                        return c == ' ' || c == '\'' || c == ':' || c == ',' || c == '!'  || c == '/' ? '_' : std::tolower(c);
                    });
                    fout << '[' << lowercase << "_chrootsteam]\n"
                         "name=" << name << "\n"
                         "icon=" << lowercase << "\n"
                         "launch=[\n"
                         "  /bin/bash\n"
                         "  -c\n"
                         "  " << script << appid << "\"\n"
                         "]\n"
                         "files=" << dir_entry.path().native() << '\n';
                }
            }
        }
}
int parse_arg(int argc, char** argv)
{
    using namespace std::string_literals;
    bool posarguments = true;
    bool exit = false;
    for(int i = 1; i < argc; ++i)
    {
        if(posarguments && argv[i][0] == '-')
        {
            if(argv[i][1] == '-')
            {
                if(argv[i][2] == 0)
                {
                    posarguments = false;
                }
                else if(argv[i] == "--exit"s)
                {
                    exit = true;
                }
                else if(argv[i] == "--chrootsteam"s)
                {
                    ++i;
                    if(i < argc)
                    {
                        string script(argv[i]);
                        vector<string> steamapps;
                        int j = i + 1;
                        while(j < argc && (!(argv[j][0] == '-' && argv[j][1] == '-')))
                        {
                            steamapps.push_back(argv[j]);
                            ++j;
                        }
                        chrootsteam(script, steamapps);
                        i = j - 1;
                    }
                }
                else
                {
                    print_usage(argv[0]);
                    std::cout << argv[i] << " not implemented\n";
                    return 2;
                }
            }
            else if(argv[i][1] == 0)
            {
                print_usage(argv[0]);
                std::cout << "- not implemented\n";
                return 2;
            }
            else
            {
                int j = 1;
                while(argv[i][j] != 0)
                {
                    if(argv[i][j] == 'h')
                    {
                        print_usage(argv[0]);
                        return 1;
                    }
                    else if(argv[i][j] == 'e')
                        exit = true;
                    else
                    {
                        print_usage(argv[0]);
                        std::cout << "wrong parameter '" << argv[i][j] << "'\n";
                        return 2;
                    }
                    ++j;
                }
            }
        }
        else
        {
            print_usage(argv[0]);
            std::cout << "wrong parameter\n";
            return 2;
        }
    }
    return exit ? 1 : 0;
}
