#include "execute.hpp"

#include <cstring>
#include <unistd.h>

//todo this makes it linux only, look into _spawnvp
void execute(const vector<string>& arg)
{
    if(fork() == 0)
    {
        char** argument_list = new char*[arg.size() + 1];
        for(std::size_t i = 0; i < arg.size(); ++i)
        {
            argument_list[i] = new char[arg[i].size() + 1];
            strcpy(argument_list[i], arg[i].c_str());
        }
        argument_list[arg.size()] = nullptr;
        execvp(argument_list[0], argument_list);
        // for(std::size_t i = 0; i < arg.size(); ++i)
        //     delete[] argument_list[i];
        // delete[] argument_list;
        exit(1); // in case execvp goes wrong
    }
}
