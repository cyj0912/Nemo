cmake_minimum_required(VERSION 3.1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake ${CMAKE_SOURCE_DIR}/CMake/Modules)

project(Nemo)

set(CMAKE_SKIP_INSTALL_RULES TRUE)
set(BINARY_DIR ${CMAKE_BINARY_DIR}/Binary)

include(ModuleHelper)

add_subdirectory(ThirdParty)
dump_all_3rdparty_module()

include_directories(.)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})
message(STATUS "------Targets------")
option(BUILD_SHARED_LIBS "Build shared libs" ON)
if(WIN32 AND ${BUILD_SHARED_LIBS})
    #set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

include(CompileOptions)
include_directories(SharedHeaders)
add_subdirectory(Foundation)
add_subdirectory(Math)
add_subdirectory(Resource)
add_subdirectory(Nemo)
add_subdirectory(SceneMgr)
add_subdirectory(ExperimentRealm)
