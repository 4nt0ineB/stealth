cmake_minimum_required(VERSION 3.22.3)
project(stealth C)

set(CMAKE_C_STANDARD 90)

include_directories(include)
include_directories(include/controller)
include_directories(include/graphics)

add_executable(stealth
        src/Main.c
        src/controller/Controller.c include/controller/Controller.h
        src/model/Player.c include/model/Player.h
        src/model/Guard.c include/model/Guard.h
        src/model/Relic.c include/model/Relic.h
        src/model/Room.c include/model/Room.h
        src/model/Tile.c include/model/Tile.h
        src/model/Direction.c include/model/Direction.h
        src/core/Settings.c include/core/Settings.h
        src/view/View.c include/view/View.h
        src/core/geometry/Rectangle.c include/core/geometry/Rectangle.h
        src/core/geometry/Position.c include/core/geometry/Position.h
        src/core/Util.c include/core/Util.h
        src/core/Timer.c include/core/Timer.h
        src/model/Skill.c include/model/Skill.h
        src/model/Score.c include/model/Score.h
        src/controller/GameData.c include/controller/GameData.h
        src/controller/Button.c include/controller/Button.h)

target_link_libraries(${PROJECT_NAME} MLV m)
# Macros
add_compile_definitions(RESOURCES_DIR="resources/")
add_compile_definitions(DEFAULT_WIN_W_PERCENT=70)
add_compile_definitions(DEFAULT_WIN_H_PERCENT=70)