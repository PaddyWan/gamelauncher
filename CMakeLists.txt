cmake_minimum_required(VERSION 3.5)
project(gamelauncher)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
  set(SOURCES
    src/GUI.cpp
    src/execute.cpp
    src/imgui_additions.cpp
    src/texture.cpp
    src/config.cpp
    src/arg.cpp
    src/cache.cpp
  )
set(IMGUI_SOURCES
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
)
set(IMGUI_INCLUDE_DIRS
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
)
set(LIBXDGBASE_SOURCE
  libxdgbasedir/src/libxdgbasedir/libxdgbasedir.cpp
)
set(LIBXDGBASE_INCLUDE_DIRS
  libxdgbasedir/src
)
set(STB_INCLUDE_DIRS
  stb
)

set(GAMELAUNCHER_BACKEND "GLFW_OPENGL3" CACHE STRING "Gamelauncher Backend choosen")
set_property(CACHE GAMELAUNCHER_BACKEND PROPERTY STRINGS "SDL2_SDL2" "SDL2_OPENGL3" "GLFW_OPENGL3")
get_property(GAMELAUNCHER_BACKEND_STRINGS CACHE GAMELAUNCHER_BACKEND PROPERTY STRINGS)
if(NOT GAMELAUNCHER_BACKEND IN_LIST GAMELAUNCHER_BACKEND_STRINGS)
  message(FATAL_ERROR "Wrong value of the parameter 'GAMELAUNCHER_BACKEND'")
endif()
if(GAMELAUNCHER_BACKEND STREQUAL "SDL2_SDL2")
  set(SOURCES
    ${SOURCES}
    src/main_sdl2_sdl2.cpp
    src/sdl2_texture.cpp
  )
  set(IMGUI_SOURCES
    ${IMGUI_SOURCES}
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.cpp
  )
  find_package(SDL2 REQUIRED)
  add_compile_definitions(SDL2_FRONTEND)
  add_compile_definitions(SDL2_BACKEND)  
elseif(GAMELAUNCHER_BACKEND STREQUAL "SDL2_OPENGL3")
  set(SOURCES
    ${SOURCES}
    src/main_sdl2_opengl3.cpp
    src/opengl_texture.cpp
  )
  set(IMGUI_SOURCES
    ${IMGUI_SOURCES}
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  )
  find_package(SDL2 REQUIRED)
  set(OpenGL_GL_PREFERENCE GLVND)
  find_package(OpenGL)
  add_compile_definitions(SDL2_FRONTEND)
  add_compile_definitions(OPENGL_BACKEND)
elseif(GAMELAUNCHER_BACKEND STREQUAL "GLFW_OPENGL3")
  set(SOURCES
    ${SOURCES}
    src/main_glfw_opengl3.cpp
    src/opengl_texture.cpp
  )
  set(IMGUI_SOURCES
    ${IMGUI_SOURCES}
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  )
  set(OpenGL_GL_PREFERENCE GLVND)
  find_package(OpenGL)
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLFW REQUIRED glfw3)
  add_compile_definitions(GLFW_FRONTEND)
  add_compile_definitions(OPENGL_BACKEND)
else()
  message(FATAL_ERROR "Wrong value of the parameter 'GAMELAUNCHER_BACKEND'")
endif()
# find_package(nlohmann_json REQUIRED)

add_executable(gamelauncher ${SOURCES} ${IMGUI_SOURCES} ${LIBXDGBASE_SOURCE})
target_include_directories(gamelauncher PUBLIC ${IMGUI_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${LIBXDGBASE_INCLUDE_DIRS} ${STB_INCLUDE_DIRS})
target_link_libraries(gamelauncher ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${GLFW_LIBRARIES})
