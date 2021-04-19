cmake_minimum_required(VERSION 3.00)

file(GLOB BASIC_SRC ../basic/*.c)
add_executable(prova main.c ${BASIC_SRC})
target_include_directories(prova PRIVATE ../include)