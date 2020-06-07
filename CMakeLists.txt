cmake_minimum_required(VERSION 3.10)
project(glTF_viewer)

# add build property
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wno-unused-parameter -Wno-implicit-fallthrough")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DDEBUG)
ENDIF ()

MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# dep
# find_package(OpenGL REQUIRED)

add_subdirectory(libs/glm ${CMAKE_BINARY_DIR}/glm)
add_subdirectory(libs/glfw ${CMAKE_BINARY_DIR}/glfw)

# for apple
include_directories(/usr/local/include/)
link_directories(/usr/local/lib/)
find_package(OpenGL REQUIRED)

# add local sub module
include_directories ("${PROJECT_SOURCE_DIR}/source/render/include")
add_subdirectory (source/render)

# add main executable program
include_directories ("${PROJECT_SOURCE_DIR}/source/main/include")
aux_source_directory(source/main/src MAIN_FILES)

set(SOURCE_FILES ${MAIN_FILES})
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# link sub cmakelists
target_link_libraries (${PROJECT_NAME} RENDER)