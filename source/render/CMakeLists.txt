cmake_minimum_required(VERSION 3.10)
project(RENDER)

include_directories(include)
include_directories(include/orca)

aux_source_directory(src SRC_FILES)

set(SOURCES ${SRC_FILES})

add_library(${PROJECT_NAME} ${SOURCES})

# if (UNIX)
# 	target_link_libraries(${PROJECT_NAME} m)
# endif()

target_include_directories(${PROJECT_NAME} PRIVATE include)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} glm)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} glfw)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} assimp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} dl)
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} GLU)
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} GL)
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} X11)
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} Xrandr)
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} Xi)
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} Xxf86vm)
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} Xinerama)
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} Xcursor)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)