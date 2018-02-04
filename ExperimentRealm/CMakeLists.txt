cmake_minimum_required(VERSION 3.9)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB IMGUI_HEADERS imgui/*.h)
file(GLOB IMGUI_SOURCES imgui/*.cpp)

file(GLOB MAIN_SOURCES *.cpp)
file(GLOB MAIN_HEADERS *.h)

add_executable(ExperimentRealm
	${IMGUI_HEADERS} ${IMGUI_SOURCES} ${MAIN_SOURCES} ${MAIN_HEADERS}
)

target_compile_options(ExperimentRealm ${DEFAULT_COMPILE_OPTIONS})

target_link_libraries(ExperimentRealm PRIVATE Foundation)
target_link_libraries(ExperimentRealm PRIVATE Math)
target_link_libraries(ExperimentRealm PRIVATE SceneMgr)
target_link_libraries(ExperimentRealm PRIVATE Resource)

target_include_directories(ExperimentRealm PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(ExperimentRealm PRIVATE ${SDL2_LIBRARIES})
if(WIN32)
    target_link_libraries(ExperimentRealm PRIVATE SDL2::SDL2)
endif()

target_include_directories(ExperimentRealm PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(ExperimentRealm PRIVATE ${OPENGL_LIBRARIES})

if(WIN32)
    target_link_libraries(ExperimentRealm PRIVATE glad)
endif()
target_link_libraries(ExperimentRealm PRIVATE nanovg)
