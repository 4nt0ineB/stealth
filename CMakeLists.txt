cmake_minimum_required(VERSION 3.23)
project(stealth C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(include/controller)
include_directories(include/graphics)

add_executable(stealth
        include/controller/Controller.h
        include/graphics/Graphic.h
        include/Character.h
        include/Guard.h
        include/Menu.h
        include/Relic.h
        include/Room.h
        include/Test.h
        src/controller/Controller.c
        src/graphics/Graphic.c
        src/Character.c
        src/Guard.c
        src/Main.c
        src/Menu.c
        src/Relic.c
        src/Room.c
        src/Test.c)
