# CMake file was originally copied from youtuber Erik Yuzwa (https://www.youtube.com/watch?v=_i4wRjcp8eU)

cmake_minimum_required(VERSION 3.6)

project(Breakout CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
set(BREAKOUT_SOURCES 
  ./src/main.cpp
  ./src/Game.cpp
  ./src/Player.cpp
  ./src/Ball.cpp
  ./src/Utils.cpp
  ./src/Blocks.cpp
)

# Header

include(FetchContent)
function(include_dependency libName gitUrl gitTag)
  FetchContent_Declare(${libName}
    GIT_REPOSITORY  ${gitUrl}
    GIT_TAG         ${gitTag}
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
  )
  FetchContent_MakeAvailable(${libName})
  endFunction()

set(LIB1 raylib)
find_package(${LIB1} QUIET)
if (NOT ${LIB1}_FOUND)
  include_dependency(${LIB1} https://github.com/raysan5/raylib.git 5.0)
endif()

set(LIB2 entt)
find_package(${LIB2} QUIET)
if (NOT ${LIB2}_FOUND)
  include_dependency(${LIB2} https://github.com/skypjack/entt.git v3.15.0)
endif()

add_executable(Breakout ${BREAKOUT_SOURCES})

target_include_directories(Breakout PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_include_directories(Breakout PRIVATE ${raylib_INCLUDE_DIRS})

target_link_libraries(Breakout PRIVATE ${LIB1})
target_link_libraries(Breakout PRIVATE EnTT::EnTT)

