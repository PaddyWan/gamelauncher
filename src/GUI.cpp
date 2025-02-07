#include "GUI.hpp"


#include <libxdgbasedir/libxdgbasedir.h>

#include "imgui_additions.hpp"
#include "execute.hpp"
#include "texture.hpp"

constexpr static int ICON_EDGE = 20;
constexpr static int BANNER_HEIGHT = 100;
constexpr static int BANNER_WIDTH = 400;

GUI::GUI()
{
    init();
}
void GUI::quit()
{
    done = true;
}
bool GUI::isDone()
{
    return done;
}
void GUI::minimize()
{
    // todo this will be used for minimize to tray
    isopen = false;
}
void GUI::run()
{
    if(ImGui::Begin("Gamelauncher", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::BeginChild("child1", {200, 0}, 0, ImGuiWindowFlags_NoSavedSettings);
        {
            const std::size_t current_selected = highlight;
            highlight = static_cast<std::size_t>(-1);
            for(std::size_t i = 0; i < entryarray.size(); ++i)
            {
                if(i == current_selected)
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive]);
                ImGui::PushID(i);
                ImGui::BeginChild("child2", {0, ICON_EDGE}, 0, ImGuiWindowFlags_NoSavedSettings);
                {
                    ImGui::Image(entryarray[i].icon_texture, ImVec2(ICON_EDGE, ICON_EDGE));
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::Text(entryarray[i].name.c_str(), 0);
                }
                ImGui::EndChild();
                if(ImGui::IsItemClicked())
                    clickedgame(i);
                if(ImGui::IsItemHovered())
                    highlightgame(i);
                ImGui::PopID();

                if(i == current_selected)
                    ImGui::PopStyleColor();
            }
        }
        ImGui::EndChild();
        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::BeginChild("child3", { 0, 0 }, ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoSavedSettings);
        if(selected < entryarray.size())
        {
            ImGui::Image(banner_texture, ImVec2(BANNER_WIDTH, BANNER_HEIGHT));
            ImGui::Text(entryarray[selected].name.c_str(), 0);
            if(ImGui::Button("Play"))
            {
                execute(entryarray[selected].launch);
                hasclicked = 60;
            }
            if(hasclicked)
            {
                ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
                ImGui::Spinner("##spinner", 8, 4, col);
                --hasclicked;
            }
            ImGui::TextWrapped(entryarray[selected].description.c_str(), 0);
        }
        ImGui::EndChild();
        ImGui::End();
    }
}

void GUI::init()
{
    entryarray = parse_config();
    for(std::size_t i = 0; i < entryarray.size(); ++i)
    {
        string iconpng = xdg::config::home() + "/gamelauncher/img/" + entryarray[i].iconname + "_icon";
        LoadTextureFromFile(iconpng, &(entryarray[i].icon_texture), ICON_EDGE, ICON_EDGE);
    }
    if(entryarray.size() > 0)
    {
        string bannerpng = xdg::config::home() + "/gamelauncher/img/" + entryarray[0].iconname + "_banner";
        LoadTextureFromFile(bannerpng, &banner_texture, 1, 1);
    }
}

void GUI::clickedgame(std::size_t i)
{
    if(i != selected)
    {
        selected = i;
        if(selected < entryarray.size())
        {
            string bannerpng = xdg::config::home() + "/gamelauncher/img/" + entryarray[selected].iconname + "_banner";
            ReloadTextureFromFile(bannerpng, &banner_texture, BANNER_WIDTH, BANNER_HEIGHT);
        }
    }
}

void GUI::highlightgame(std::size_t i)
{
    highlight = i;
}
