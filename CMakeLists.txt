cmake_minimum_required(VERSION 3.21)
project(MyOpenGLApp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Fetch GLFW ---
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.9
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3  # or a newer stable tag if you want
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.4
    GIT_SHALLOW TRUE
)

# Disable extras we don't need
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw glm assimp)

# --- OpenGL ---
find_package(OpenGL REQUIRED)

# --- Your executable ---
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    src/*.c
)

add_library(imgui_static STATIC 
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_static PUBLIC
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui
)

target_link_libraries(imgui_static PRIVATE
    glfw
    OpenGL::GL
)
target_compile_definitions(imgui_static PRIVATE GLFW_INCLUDE_NONE)


add_executable(${PROJECT_NAME} ${SOURCES})

# Include your own headers
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/imgui
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    assimp
    imgui_static
    glfw
    glm::glm
    OpenGL::GL
)

# Windows-specific: prevent console window if you want
# set_target_properties(MyOpenGLApp PROPERTIES WIN32_EXECUTABLE TRUE)
