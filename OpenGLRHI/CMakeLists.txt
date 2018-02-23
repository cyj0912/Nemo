set(MODULE_NAME OpenGLRHI)

file(GLOB PRIVATE_SOURCES Private/*.h Private/*.cpp)
file(GLOB PUBLIC_SOURCES Public/*.h)
source_group(Private FILES ${PRIVATE_SOURCES})
source_group(Public FILES ${PUBLIC_SOURCES})

add_library(${MODULE_NAME} ${PRIVATE_SOURCES} ${PUBLIC_SOURCES})

include(GenerateExportHeader)
target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
generate_export_header(${MODULE_NAME} EXPORT_MACRO_NAME OPENGLRHI_API EXPORT_FILE_NAME OpenGLRHIAPI.h)

target_include_directories(${MODULE_NAME} PRIVATE Private)
target_include_directories(${MODULE_NAME} PUBLIC Public)
target_compile_options(${MODULE_NAME} ${DEFAULT_COMPILE_OPTIONS})
target_compile_definitions(${MODULE_NAME} PUBLIC RHI_STATIC_USE_OPENGL)
target_link_libraries(${MODULE_NAME} PRIVATE Foundation)
target_link_libraries(${MODULE_NAME} PRIVATE Math)
target_link_libraries(${MODULE_NAME} PUBLIC RHI)

find_package(OpenGL REQUIRED)

target_include_directories(${MODULE_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(${MODULE_NAME} PRIVATE ${OPENGL_LIBRARIES})

if(WIN32)
    target_link_libraries(${MODULE_NAME} PRIVATE glad)
endif()
