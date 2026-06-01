cmake_minimum_required(VERSION 3.15)
project(john-chess C)

set(CMAKE_C_STANDARD 11)


# Compiler-specific flags
if(MSVC)
    # MSVC compiler flags
    set(CMAKE_C_FLAGS_DEBUG "/Zi /Od")
    set(CMAKE_C_FLAGS_RELEASE "/O2")
else()
    # GCC/Clang compiler flags
    set(CMAKE_C_FLAGS_DEBUG "-g -O0")
    set(CMAKE_C_FLAGS_RELEASE "-O3")
endif()

set(CMAKE_BUILD_TYPE Debug)

# Windows-specific settings
if(WIN32)
    # Hide console window for Windows GUI application
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

    # If you want to keep the console for debugging, comment out the line above
endif()

# Include FetchContent module for downloading dependencies
include(FetchContent)

# Try to find raylib, if not found, download and build it
find_package(raylib 5.0 QUIET)

if(NOT raylib_FOUND)
    message(STATUS "raylib not found, downloading and building from source...")

    # Use a specific commit that works with newer CMake
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 5.5
            PATCH_COMMAND sed -i "1s/.*/cmake_minimum_required(VERSION 3.15)/" CMakeLists.txt || true
    )

    # Don't build raylib examples
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(raylib)
endif()

add_executable(john-chess main.c
        moves.c
        types.h
        moves.h
        textures.c
        textures.h
        engine.c
        nmoves.c
        nmoves.h
        engine.h
)
target_link_libraries(john-chess raylib)

# Windows-specific libraries
if(WIN32)
    target_link_libraries(john-chess winmm)
endif()

# Copy assets folder to build directory
add_custom_command(TARGET john-chess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:john-chess>/assets
        COMMENT "Copying assets to build directory"
)
