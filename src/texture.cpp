#include "texture.hpp"

#include <filesystem>

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"


bool LoadTextureFromMemory(unsigned char* image_data, ImTextureID* out_texture, int image_width, int image_height);
bool ReloadTextureFromMemory(unsigned char* image_data, ImTextureID* out_texture, int image_width, int image_height);

FILE* LoadImageFile(const std::string& file_name)
{
    if(std::filesystem::exists(file_name))
        return fopen(file_name.c_str(), "rb");
    else if(std::filesystem::exists(file_name + ".png"))
        return fopen((file_name + ".png").c_str(), "rb");
    else if(std::filesystem::exists(file_name + ".jpg"))
        return fopen((file_name + ".jpg").c_str(), "rb");
    else if(std::filesystem::exists(file_name + ".jpeg"))
        return fopen((file_name + ".jpeg").c_str(), "rb");
    else if(std::filesystem::exists(file_name + ".tga"))
        return fopen((file_name + ".tga").c_str(), "rb");
    else if(std::filesystem::exists(file_name + ".bmp"))
        return fopen((file_name + ".bmp").c_str(), "rb");
    else if(std::filesystem::exists(file_name + ".pbm"))
        return fopen((file_name + ".pbm").c_str(), "rb");
    else if(std::filesystem::exists(file_name + ".pgm"))
        return fopen((file_name + ".pgm").c_str(), "rb");
    else if(std::filesystem::exists(file_name + ".ppm"))
        return fopen((file_name + ".ppm").c_str(), "rb");
    return nullptr;
}


bool LoadTextureFromFile(const std::string& file_name, ImTextureID* out_texture, int out_width, int out_height)
{
    FILE* f = LoadImageFile(file_name);
    if(f == nullptr)
        return false;

    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load_from_file(f, &image_width, &image_height, nullptr, 4);
    fclose(f);
    if(image_data == nullptr)
        return false;

    unsigned char* image_resized_data = stbir_resize_uint8_linear(image_data, image_width, image_height, 0, nullptr, out_width, out_height, 0, STBIR_RGBA);
    stbi_image_free(image_data);

    bool ret = LoadTextureFromMemory(image_resized_data, out_texture, out_width, out_height);
    free(image_resized_data);
    return ret;
}


bool ReloadTextureFromFile(const std::string& file_name, ImTextureID* out_texture, int out_width, int out_height)
{
    FILE* f = LoadImageFile(file_name);
    if(f == nullptr)
        return false;

    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load_from_file(f, &image_width, &image_height, nullptr, 4);
    fclose(f);
    if(image_data == nullptr)
        return false;

    unsigned char* image_resized_data = stbir_resize_uint8_linear(image_data, image_width, image_height, 0, nullptr, out_width, out_height, 0, STBIR_RGBA);
    stbi_image_free(image_data);

    bool ret = ReloadTextureFromMemory(image_resized_data, out_texture, out_width, out_height);
    free(image_resized_data);
    return ret;
}
