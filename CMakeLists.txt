set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.16)
project(Temp-erature-HAL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(libdeps/lvgl/src)
include_directories(libdeps/TFT_eSPI)

# --- Create the LVGL Library ---
# Find all the LVGL source files.
file(GLOB_RECURSE LVGL_SOURCES
    "libdeps/lvgl/src/*.c"
)
add_custom_target(run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main DEPENDS main)
# Create a static library target for LVGL from those source files
add_library(lvgl_lib STATIC ${LVGL_SOURCES})

# Tell the new lvgl_lib target where to find its own headers
target_include_directories(lvgl_lib PUBLIC
    "libdeps/lvgl/src"
    "libdeps/lvgl"
    "${CMAKE_CURRENT_SOURCE_DIR}/hal"  # <-- This adds 'hal' to the include path
)

# Tell LVGL exactly where to find your config file
target_compile_definitions(lvgl_lib PUBLIC
    LV_CONF_PATH="lv_conf.h"
    SDL_BUILD
)

# --- Your HAL Library ---
add_library(hal STATIC
    hal/SDLDisplay.cpp
)
target_include_directories(hal PUBLIC hal hal/LilyGo)

# Tell HAL (which also includes lvgl headers) where to find the config
target_compile_definitions(hal PUBLIC
    LV_CONF_PATH="lv_conf.h"
    SDL_BUILD
)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

# Link HAL against SDL AND our new LVGL library
target_link_libraries(hal PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    lvgl_lib  # <-- This is the crucial addition
)

# --- Your Main Executable ---
add_executable(main src/main.cpp)

target_include_directories(main PUBLIC
    ./libdeps/lvgl/ # <-- FIXED TYPO HERE (was 'gl')
)

# Link the HAL to the main executable
# Because hal links to lvgl_lib PUBLICly, main will get it automatically.
target_link_libraries(main PRIVATE hal)

