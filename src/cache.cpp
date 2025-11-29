#include "cache.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <filesystem>
#include <fstream>
#include <string_view>


bool equal_variable(const unsigned char* begin, const unsigned char* end, unsigned char var)
{
    // using buint = unsigned long long;
    // while(std::distance(begin, end) >= sizeof(buint) && (*reinterpret_cast<const buint*>(begin)) == 0)
    // 	begin+=sizeof(buint);
    // while(std::distance(begin, end) >= 1 && (*begin) == 0)
    // 	++begin;
    // return begin == end;
    return std::basic_string_view<unsigned char>(begin,end).find_first_not_of(var) == std::basic_string_view<unsigned char>::npos;
}
struct FDRAII
{
    FDRAII(const string& str): fd(open(str.c_str(), O_RDONLY))
    {
	if(fd != -1)
	    filesize = lseek(fd, 0, SEEK_END);
    }
    ~FDRAII()
    {
	if(fd != -1)
	    close(fd);
    }
    bool isgood() const { return fd != -1 && filesize != -1; }
    int fd;
    off_t filesize;
};
struct MMapRAII
{
    MMapRAII(const FDRAII& fd): mapped(mmap(nullptr, fd.filesize, PROT_READ, MAP_PRIVATE, fd.fd, 0)), filesize(fd.filesize)
    {
    }
    ~MMapRAII()
    {
	if(isgood())
	    munmap(mapped, filesize);
    }    
    bool isgood() const { return mapped != MAP_FAILED; }
    void* mapped;
    off_t filesize;
};

void add_cache(const string& str)
{
    FDRAII fd(str);
    if(!fd.isgood())
	return;
    posix_fadvise(fd.fd, 0, fd.filesize, POSIX_FADV_WILLNEED);
}
void rem_cache(const string& str)
{
    FDRAII fd(str);
    if(!fd.isgood())
	return;
    posix_fadvise(fd.fd, 0, fd.filesize, POSIX_FADV_DONTNEED);
}
bool is_cached(const string& str, vector<unsigned char>& vec)
{
    FDRAII fd(str);
    if(!fd.isgood())
	return false;
    MMapRAII map(fd);
    if(!map.isgood())
	return false;
    const size_t size = (fd.filesize + getpagesize() - 1) / getpagesize();
    vec.reserve(size);
    int result = mincore(map.mapped, fd.filesize, vec.data());
    if(result == -1)
	return false;
    return !equal_variable(vec.data(), vec.data()+size, 0);
}
bool is_cached(const string& str)
{
    vector<unsigned char> vec(0);
    return is_cached(str, vec);
}
set<string> get_cache_files(const set<string>& files)
{
    set<string> returnlist;
    vector<unsigned char> vec(0);
    for(auto const& file: files)
    {
	std::filesystem::path filepath(file);
	if(std::filesystem::is_directory(filepath))
	{
	    for(auto const& entry : std::filesystem::recursive_directory_iterator(filepath))
	    {
		string str(entry.path().string());
		if(entry.is_regular_file() && is_cached(str, vec))
		    returnlist.insert(str);
	    }
	}
	else if(std::filesystem::is_regular_file(filepath) && is_cached(file, vec))
	    returnlist.insert(file);
    }
    return returnlist;
}
void load_cache_files(const set<string>& files)
{
    for(auto const& file: files)
    {
	std::filesystem::path filepath(file);
	if(std::filesystem::is_directory(filepath))
	{
	    for(auto const& entry : std::filesystem::recursive_directory_iterator(filepath))
	    {
		string str(entry.path().string());
		if(entry.is_regular_file())
		    add_cache(str);
	    }
	}
	else if(std::filesystem::is_regular_file(filepath))
	    add_cache(file);
    }
}
void unload_cache_files(const set<string>& files)
{
    for(auto const& file: files)
    {
	std::filesystem::path filepath(file);
	if(std::filesystem::is_directory(filepath))
	{
	    for(auto const& entry : std::filesystem::recursive_directory_iterator(filepath))
	    {
		string str(entry.path().string());
		if(entry.is_regular_file())
		    rem_cache(str);
	    }
	}
	else if(std::filesystem::is_regular_file(filepath))
	    rem_cache(file);
    }
}
void load_cache_files_config(const string& filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
	return;
    set<string> listoffiles;
    string line;
    while(std::getline(file, line))
	listoffiles.insert(line);
    load_cache_files(listoffiles);
}
