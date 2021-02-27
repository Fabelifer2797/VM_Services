cmake_minimum_required(VERSION 3.17)
project(ImageServer C)

set(CMAKE_C_STANDARD 11)

add_executable(ImageServer main.c)
target_link_libraries(ImageServer ulfius jansson)
