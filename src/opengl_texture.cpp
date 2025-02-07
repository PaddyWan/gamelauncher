#include "texture.hpp"

#if defined SDL2_FRONTEND
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#elif defined GLFW_FRONTEND
#include <GLFW/glfw3.h>
#else
#error "this shouldn't happen"
#endif

//from imgui wiki https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
bool LoadTextureFromMemory(unsigned char* image_data, ImTextureID* out_texture, int image_width, int image_height)
{
    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    *out_texture = image_texture;

    return true;
}
//able to reload a texture with other data
bool ReloadTextureFromMemory(unsigned char* image_data, ImTextureID* out_texture, int image_width, int image_height)
{
    // Create a OpenGL texture identifier
    GLuint image_texture = *out_texture;
    glBindTexture(GL_TEXTURE_2D, image_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    //*out_texture = image_texture;

    return true;
}
