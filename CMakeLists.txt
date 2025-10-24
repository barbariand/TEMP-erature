set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.16)
project(Temp-erature-HAL)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(libdeps/lvgl/src)
include_directories(libdeps/TFT_eSPI)

file(GLOB_RECURSE LVGL_SOURCES
    "libdeps/lvgl/src/*.c"
)
add_library(lvgl_lib STATIC ${LVGL_SOURCES})

target_include_directories(lvgl_lib PUBLIC
    "libdeps/lvgl/src"
    "libdeps/lvgl"
    "${CMAKE_CURRENT_SOURCE_DIR}/hal"
)

target_compile_definitions(lvgl_lib PUBLIC
    LV_CONF_PATH="lv_conf.h"
    SDL_BUILD
    LV_USE_SDL
)

add_library(hal STATIC
    hal/SDLDisplay.cpp
)
target_include_directories(hal PUBLIC hal hal/LilyGo ${SDL_IMAGE_INCLUDE_DIRS})

target_compile_definitions(hal PUBLIC
    LV_CONF_PATH="lv_conf.h"
    SDL_BUILD
    LV_USE_SDL
)

find_package(SDL2 REQUIRED)
find_package(SDL_image REQUIRED)

target_link_libraries(hal PUBLIC
    SDL2::SDL2
    ${SDL_IMAGE_LIBRARIES}
    lvgl_lib
)

add_executable(main src/main.cpp)

target_include_directories(main PUBLIC
    ./libdeps/lvgl/ # <-- FIXED TYPO HERE (was 'gl')
)

target_link_libraries(main PRIVATE hal)

add_custom_target(run COMMAND ${EXECUTABLE_OUTPUT_PATH}/build/main DEPENDS main)
