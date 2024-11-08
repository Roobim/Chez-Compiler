cmake_minimum_required(VERSION 3.25)
project(lexer_subprogram C)

set(CMAKE_C_STANDARD 11)

add_executable(lexer_subprogram main.c lexer.c lexer.h token.h)
