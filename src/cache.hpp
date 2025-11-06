#ifndef CACHE_HPP
#define CACHE_HPP

#include <string>
#include <vector>
#include <set>
using std::string;
using std::vector;
using std::set;

void set_prio();
set<string> get_cache_files(const set<string>& files);
void load_cache_files(const set<string>& files);
void unload_cache_files(const set<string>& files);
void load_cache_files_config(const string& filename);

#endif//CACHE_HPP
