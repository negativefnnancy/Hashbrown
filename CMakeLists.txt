cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 90)
add_compile_options(-Wall -Wextra -pedantic -Wno-unused-parameter)

project(Hashbrown VERSION 0.0.1)

# sound module

add_library(sound STATIC
    src/sound/modulator.c
    src/sound/operator.c
    src/sound/oscillator.c
    src/sound/noise.c
    src/sound/sine.c
    src/sound/soundchip.c
    src/sound/synth.c
)

target_include_directories(sound PUBLIC include)
target_link_libraries(sound m)

# tests

add_executable(test_synth tests/synth.c)
target_link_libraries(test_synth PUBLIC sound sndfile)
