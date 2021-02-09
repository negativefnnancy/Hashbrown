cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 90)
add_compile_options(-Wall -Wextra -pedantic -Wno-unused-parameter)

project(Hashbrown VERSION 0.0.1)



# dependencies

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)



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
    src/music/channel.c
    src/music/player.c
    src/music/configuration.c
    src/music/envelope.c
    src/music/envelope_generator.c
    src/music/module.c
    src/music/pattern.c
    src/music/row.c
)

target_include_directories(music PUBLIC include)



# tracker app

add_executable(hashbrown_tracker
    src/tracker/main.c
    src/tracker/interface.c
    src/tracker/color.c
    src/tracker/theme.c
    src/tracker/region.c
    src/tracker/vector.c
    src/tracker/util.c
    src/tracker/ui/element.c
    src/tracker/ui/box.c
    src/tracker/ui/box_style.c
    src/tracker/ui/dummy.c
)

target_link_libraries(hashbrown_tracker PUBLIC sound music sndfile)

add_custom_command(TARGET hashbrown_tracker PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res/tracker/ $<TARGET_FILE_DIR:hashbrown_tracker>)

target_include_directories(hashbrown_tracker PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2TTF_INCLUDE_DIRS})

target_link_libraries(hashbrown_tracker PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2TTF_LIBRARIES})



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

add_executable(test_music
    src/tests/music.c
    src/tests/audio.c
)

target_link_libraries(test_music PUBLIC sound music sndfile)
