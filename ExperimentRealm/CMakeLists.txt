cmake_minimum_required(VERSION 3.9)
project(ExperimentRealm)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB IMGUI_HEADERS imgui/*.h)
file(GLOB IMGUI_SOURCES imgui/*.cpp)

add_executable(ExperimentRealm main.cpp Shader.cpp Shader.h ${IMGUI_HEADERS} ${IMGUI_SOURCES} EntityLibrary.cpp EntityLibrary.h InputHandler.cpp InputHandler.h InputEvent.h KeySym.h InputEventFromSDL2.h OpenGL.h BezierCurve.cpp BezierCurve.h RenderComponent.h UVSphere.cpp UVSphere.h RayVisualize.cpp RayVisualize.h EditorMaster.h PrimitiveRenderer.cpp PrimitiveRenderer.h InteractionSystem.cpp InteractionSystem.h EntityManager.cpp EntityManager.h Gizmo.cpp Gizmo.h)

target_link_libraries(ExperimentRealm PRIVATE Foundation)
target_link_libraries(ExperimentRealm PRIVATE Math)
target_link_libraries(ExperimentRealm PRIVATE SceneMgr)
target_link_libraries(ExperimentRealm PRIVATE Resource)

target_include_directories(ExperimentRealm PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(ExperimentRealm PRIVATE ${SDL2_LIBRARIES})

target_include_directories(ExperimentRealm PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(ExperimentRealm PRIVATE ${OPENGL_LIBRARIES})

target_link_libraries(ExperimentRealm PRIVATE nanovg)
