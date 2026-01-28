# CMake configuration for RayTracer project
cmake_minimum_required(VERSION 3.10)

project(RayTracer LANGUAGES CXX)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build Types
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    Debug Release
)

# Default to Debug if no build type is specified (single-config generators)
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: Debug Release."
        FORCE)
endif()

# Source Files
set(SOURCE_ONE_WEEKEND
    InOneWeekend/main.cpp
    InOneWeekend/src/vector3.cpp
    InOneWeekend/src/color.cpp
    InOneWeekend/src/ray.cpp
    InOneWeekend/src/hittable.cpp
    InOneWeekend/src/hittable_list.cpp
    InOneWeekend/src/sphere.cpp
    InOneWeekend/src/interval.cpp
)

# Include Directories
include_directories(include)

# Add Executables
add_executable(RayTracerInOneWeekend ${SOURCE_ONE_WEEKEND})

# Include Directories for Target
target_include_directories(RayTracerInOneWeekend PRIVATE InOneWeekend/include)

# Compile Options
set(COMMON_CXX_FLAGS -fdiagnostics-color=always -pedantic-errors -Wall -Wextra -Werror -Weffc++ -Wconversion -Wsign-conversion)
set(DEBUG_CXX_FLAGS ${COMMON_CXX_FLAGS} -O0 -g -ggdb -DDEBUG -fno-omit-frame-pointer)
set(RELEASE_CXX_FLAGS ${COMMON_CXX_FLAGS} -O3 -DNDEBUG -march=native)

# Target Compile Options
target_compile_options(RayTracerInOneWeekend PRIVATE
    $<$<CONFIG:Release>:${RELEASE_CXX_FLAGS}>
    $<$<CONFIG:Debug>:${DEBUG_CXX_FLAGS}>
)

# Set Target Properties
set_target_properties(RayTracerInOneWeekend PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/$<CONFIG>
)
