cmake_minimum_required(VERSION 3.1)

project(Nemo LANGUAGES CXX)

file(GLOB MAIN_SOURCES *.cpp *.ui ECS/*.cpp Physics/*.cpp Render/*.cpp)
file(GLOB MAIN_HEADERS *.h ECS/*.h Physics/*.h Render/*.h)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${PROJECT_NAME} ${MAIN_SOURCES} ${MAIN_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_include_directories(${PROJECT_NAME} PRIVATE ./ECS)
target_include_directories(${PROJECT_NAME} PRIVATE ./Physics)
target_include_directories(${PROJECT_NAME} PRIVATE ./Render)
target_compile_options(${PROJECT_NAME} ${DEFAULT_COMPILE_OPTIONS})
target_link_libraries(${PROJECT_NAME} PRIVATE Foundation)
target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE SceneMgr)
target_link_libraries(${PROJECT_NAME} PRIVATE Resource)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)
find_package(yaml-cpp QUIET)
if(NOT ${yaml-cpp_FOUND})
    # Try to find this on our own
    find_path(yaml-cpp_INCLUDE_DIR NAMES yaml-cpp/yaml.h
              PATHS /usr/include /usr/local/include /sw/include /opt/local/include)
    find_library(yaml-cpp_LIBRARY NAMES yaml-cpp
                 PATHS /usr/lib64 /usr/lib /usr/local/lib64 /usr/local/lib /sw/lib /opt/local/lib)
    if(yaml-cpp_INCLUDE_DIR AND yaml-cpp_LIBRARY)
        add_library(yaml-cpp UNKNOWN IMPORTED)
        set_target_properties(yaml-cpp PROPERTIES IMPORTED_IMPLIB ${yaml-cpp_LIBRARY}
                IMPORTED_LOCATION ${yaml-cpp_LIBRARY}
                INTERFACE_INCLUDE_DIRECTORIES ${yaml-cpp_INCLUDE_DIR})
    else()
        message(FATAL_ERROR "yaml-cpp not found")
    endif()
endif()
find_package(TBB COMPONENTS tbbmalloc tbbmalloc_proxy)

target_link_libraries(${PROJECT_NAME} PRIVATE ThirdParty::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets)

target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${BULLET_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${BULLET_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${YAML_CPP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${TBB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${TBB_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${TBB_DEFINITIONS})
