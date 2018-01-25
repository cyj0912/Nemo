cmake_minimum_required(VERSION 3.9)
project(ExperimentRealm)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB IMGUI_HEADERS imgui/*.h)
file(GLOB IMGUI_SOURCES imgui/*.cpp)

add_executable(ExperimentRealm main.cpp Shader.cpp Shader.h ${IMGUI_HEADERS} ${IMGUI_SOURCES} EntityLibrary.cpp EntityLibrary.h InputHandler.cpp InputHandler.h)

target_link_libraries(ExperimentRealm PRIVATE Foundation)
target_link_libraries(ExperimentRealm PRIVATE Math)
target_link_libraries(ExperimentRealm PRIVATE SceneMgr)
target_link_libraries(ExperimentRealm PRIVATE Resource)

target_include_directories(ExperimentRealm PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(ExperimentRealm PRIVATE ${SDL2_LIBRARIES})

target_include_directories(ExperimentRealm PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(ExperimentRealm PRIVATE ${OPENGL_LIBRARIES})

target_link_libraries(ExperimentRealm PRIVATE nanovg)
