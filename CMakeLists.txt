cmake_minimum_required(VERSION 3.20)

project(Bingo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    Bingo

    main.cpp
    src/board/zobrist.cpp
    src/board/board.cpp
    src/movegen/movegen.cpp
    src/move/move.cpp
    src/bitboard/bitboard.cpp
    src/movegen/movegen.cpp
    src/perft/perft.cpp
    src/movegen/attacks.cpp
)

target_include_directories(
    Bingo
    PRIVATE
    src
)
if(MSVC)
    target_compile_options(
        Bingo PRIVATE
        /W4
    )
else()
    target_compile_options(
        Bingo PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()