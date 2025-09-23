find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED sdl2_ttf)

# Include SDL2 headers in console
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Link SDL2 libraries to console
link_directories(${SDL2_LIBRARY_DIRS} ${SDL2_TTF_LIBRARY_DIRS})

# Define source files for console library
set(SOURCES
    lib/bus.c
    lib/cart.c
    lib/cpu_fetch.c
    lib/cpu_proc.c
    lib/cpu_util.c
    lib/cpu.c
    lib/dma.c
    lib/emu.c
    lib/instructions.c
    lib/interrupts.c
    lib/io.c
    lib/lcd.c
    lib/ppu.c
    lib/ram.c
    lib/stack.c
    lib/timer.c
    lib/ui.c
)

# Create the console library
add_library(console_lib ${SOURCES})

# Link SDL2 and SDL2_ttf to console_lib
target_link_libraries(console_lib ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
