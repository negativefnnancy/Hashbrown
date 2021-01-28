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



# music module

add_library(music STATIC
    src/music/module.c
)

target_include_directories(music PUBLIC include)



# tests

add_executable(test_synth
    src/tests/synth.c
    src/tests/audio.c
)

target_link_libraries(test_synth PUBLIC sound sndfile)

add_executable(test_soundchip
    src/tests/soundchip.c
    src/tests/audio.c
)

target_link_libraries(test_soundchip PUBLIC sound sndfile)
