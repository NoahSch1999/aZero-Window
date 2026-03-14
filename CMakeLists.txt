cmake_minimum_required(VERSION 3.23)

project(aZeroWindow)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")

option(COMPILE_EXAMPLE "Make it as an exe instead of lib" OFF)

if (COMPILE_EXAMPLE)
    add_executable(aZeroWindow
        "src/aZeroWindow.cpp" 
        "examples/main.cpp"
    )
    target_compile_definitions(aZeroWindow PUBLIC COMPILE_EXAMPLE)
else()
    add_library(aZeroWindow 
        "src/aZeroWindow.cpp")
endif()

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.x
)

FetchContent_MakeAvailable(SDL3)

target_include_directories(aZeroWindow
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(aZeroWindow PUBLIC SDL3::SDL3)