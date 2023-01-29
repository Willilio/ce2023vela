cmake_minimum_required(VERSION 3.24)
project(ce2023vela)

set(CMAKE_CXX_STANDARD 17)

add_executable(ce2023vela main.cpp core/repr/Board.cpp core/repr/Board.h core/util/str.cpp core/util/str.h core/repr/MoveGeneration.cpp core/repr/MoveGeneration.h)
