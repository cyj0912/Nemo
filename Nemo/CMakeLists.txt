cmake_minimum_required(VERSION 3.9)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB IMGUI_HEADERS imgui/*.h)
file(GLOB IMGUI_SOURCES imgui/*.cpp)

file(GLOB MAIN_SOURCES *.cpp)
file(GLOB MAIN_HEADERS *.h)

add_executable(Nemo
	${IMGUI_HEADERS} ${IMGUI_SOURCES} ${MAIN_SOURCES} ${MAIN_HEADERS}
)

target_compile_options(Nemo ${DEFAULT_COMPILE_OPTIONS})

target_link_libraries(Nemo PRIVATE Foundation)
target_link_libraries(Nemo PRIVATE Math)
target_link_libraries(Nemo PRIVATE SceneMgr)
target_link_libraries(Nemo PRIVATE Resource)

target_include_directories(Nemo PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Nemo PRIVATE ${SDL2_LIBRARIES})
if(WIN32)
    target_link_libraries(Nemo PRIVATE SDL2::SDL2)
endif()

target_include_directories(Nemo PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(Nemo PRIVATE ${OPENGL_LIBRARIES})

if(WIN32)
    target_link_libraries(Nemo PRIVATE glad)
endif()
target_link_libraries(Nemo PRIVATE nanovg)
