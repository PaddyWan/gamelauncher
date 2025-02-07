#ifndef IMGUI_ADDITIONS_HPP
#define IMGUI_ADDITIONS_HPP

//https://github.com/ocornut/imgui/issues/1901
#include <imgui_internal.h>
namespace ImGui
{
    bool Spinner(const char* label, float radius, int thickness, const ImU32& color);
}


#endif//IMGUI_ADDITIONS_HPP
