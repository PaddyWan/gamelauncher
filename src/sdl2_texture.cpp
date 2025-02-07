#include "texture.hpp"

#include <SDL.h>


SDL_Renderer* renderer;

//from imgui wiki https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples

bool LoadTextureFromMemory(unsigned char* image_data, ImTextureID* out_texture, int image_width, int image_height)
{
    int channels = 4;

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)image_data, image_width, image_height, channels * 8, channels* image_width, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    if(surface == nullptr)
    {
        fprintf(stderr, "Failed to create SDL surface: %s\n", SDL_GetError());
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == nullptr)
        fprintf(stderr, "Failed to create SDL texture: %s\n", SDL_GetError());

    *out_texture = (ImTextureID)texture;

    SDL_FreeSurface(surface);

    return true;
}
//from imgui wiki https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
bool ReloadTextureFromMemory(unsigned char* image_data, ImTextureID* out_texture, int image_width, int image_height)
{
    SDL_DestroyTexture((SDL_Texture*)(&out_texture));;
    int channels = 4;

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)image_data, image_width, image_height, channels * 8, channels* image_width, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    if(surface == nullptr)
    {
        fprintf(stderr, "Failed to create SDL surface: %s\n", SDL_GetError());
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == nullptr)
        fprintf(stderr, "Failed to create SDL texture: %s\n", SDL_GetError());

    *out_texture = (ImTextureID)texture;

    SDL_FreeSurface(surface);

    return true;
}
