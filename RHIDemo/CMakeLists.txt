cmake_minimum_required(VERSION 3.0)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB MAIN_SOURCES *.cpp)
file(GLOB MAIN_HEADERS *.h)

add_executable(RHIDemo ${MAIN_SOURCES} ${MAIN_HEADERS})

target_compile_options(RHIDemo ${DEFAULT_COMPILE_OPTIONS})

target_link_libraries(RHIDemo PRIVATE Foundation)
target_link_libraries(RHIDemo PRIVATE Math)
target_link_libraries(RHIDemo PRIVATE OpenGLRHI)

target_include_directories(RHIDemo PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(RHIDemo PRIVATE ${SDL2_LIBRARIES})
if(WIN32)
    target_link_libraries(RHIDemo PRIVATE SDL2::SDL2)
endif()

target_include_directories(RHIDemo PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(RHIDemo PRIVATE ${OPENGL_LIBRARIES})

if(WIN32)
    target_link_libraries(RHIDemo PRIVATE glad)
endif()
