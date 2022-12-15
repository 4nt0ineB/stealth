cmake_minimum_required(VERSION 3.22.3)
project(stealth C)

set(CMAKE_C_STANDARD 90)

include_directories(include)
include_directories(include/controller)
include_directories(include/graphics)

add_executable(stealth
        include/controller/Controller.h
        include/model/Player.h
        include/model/Guard.h
        include/view/Menu.h
        include/model/Relic.h
        include/model/Room.h
        include/Test.h
        src/controller/Controller.c
        src/model/Player.c
        src/model/Guard.c
        src/Main.c
        src/view/Menu.c
        src/model/Relic.c
        src/model/Room.c
        src/Test.c
        src/model/Tile.c
        src/model/Direction.c
        include/core/Settings.h
        src/view/View.c
        include/view/View.h
        src/core/geometry/Rectangle.c
        include/core/geometry/Rectangle.h
        src/core/geometry/Position.c
        include/core/geometry/Position.h
        src/core/geometry/Circle.c
        include/core/geometry/Circle.h
        src/core/Util.c
        include/core/Util.h src/core/Settings.c
        src/core/Timer.c
        include/core/Timer.h src/model/Skill.c include/model/Skill.h)

target_link_libraries(${PROJECT_NAME} MLV m)
# Macros
add_compile_definitions(RESOURCES_DIR="resources/")
add_compile_definitions(DEFAULT_WIN_W_PERCENT=70)
add_compile_definitions(DEFAULT_WIN_H_PERCENT=70)