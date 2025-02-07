#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <imgui.h>
#include <string>

bool LoadTextureFromFile(const std::string& file_name, ImTextureID* out_texture, int out_width, int out_height);
bool ReloadTextureFromFile(const std::string& file_name, ImTextureID* out_texture, int out_width, int out_height);


#endif//TEXTURE_HPP
