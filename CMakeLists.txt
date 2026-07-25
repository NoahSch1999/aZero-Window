cmake_minimum_required(VERSION 3.23)

project(aZeroWindow)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")

# SDL configuration
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS_INSTALL OFF CACHE BOOL "" FORCE)

set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)

# SDL_image configuration
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_WEBP OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_JXL OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_PNG ON CACHE BOOL "" FORCE)
set(SDLIMAGE_JPG ON CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    SDL3_Image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-3.2.6
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL3_Image)

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

target_include_directories(aZeroWindow
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(aZeroWindow PUBLIC SDL3::SDL3 SDL3_image::SDL3_image)