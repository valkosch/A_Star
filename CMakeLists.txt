cmake_minimum_required(VERSION 3.26)
project(Astar C)

set(CMAKE_C_STANDARD 11)

add_executable(Astar main.c
        debugmalloc.h
        graphElements.c
        linkedlist.c
        pathfinder.c
        beolvasas.c
        beolvasas.h
)
