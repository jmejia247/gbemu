cmake_minimum_required(VERSION 3.10)
project(gbemu C)

# Add console library
include_directories(console/include)
add_subdirectory(console)

# Find SDL2 and SDL2_ttf using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED sdl2_ttf)

# Add include directories from pkg-config
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Add link directories
link_directories(${SDL2_LIBRARY_DIRS} ${SDL2_TTF_LIBRARY_DIRS})

# Add the executable
add_executable(gbemu main.c)

# Link SDL2 and SDL2_ttf
target_link_libraries(gbemu console_lib ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
