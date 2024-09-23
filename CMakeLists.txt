cmake_minimum_required(VERSION 3.26)
project(Tetris_SFML)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
else()
    message(FATAL_ERROR "VCPKG_ROOT environment variable is not defined")
endif()

find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)
if(NOT DEFINED ENV{SFML_ROOT})
  message(FATAL_ERROR "is not found")
else()
  set(SFML_DIR "$ENV{SFML_ROOT}/lib/cmake/SFML")
endif()

find_package(nlohmann_json REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_executable(Tetris_SFML
Source/Main.cpp
Source/Controllers/MainGameController.hpp
Source/Controllers/MainGameController.cpp
Source/Utils/ColorPalette.hpp
Source/Views/GameOverView.hpp
Source/Views/GameOverView.cpp
Source/Views/GameWindowView.hpp
Source/Views/GameWindowView.cpp
Source/Utils/LeaderBoard.hpp
Source/Utils/LeaderBoard.cpp
Source/Utils/GameStates.hpp
Source/Utils/Particle.hpp
Source/Utils/Particle.cpp
Source/Utils/Piece.hpp
Source/Utils/Piece.cpp
)

add_executable(Tetris_Tests
Source/Tests/Tests.cpp
Source/Utils/LeaderBoard.hpp
Source/Utils/LeaderBoard.cpp
)

target_link_libraries(Tetris_SFML
sfml-system
sfml-window
sfml-graphics
nlohmann_json::nlohmann_json
)
target_link_libraries (Tetris_Tests
GTest::gtest_main
)

target_compile_definitions(Tetris_SFML PRIVATE RESOURCE_DIR="${CMAKE_SOURCE_DIR}/Resources")