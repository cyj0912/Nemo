set(MODULE_NAME SceneMgr)

file(GLOB PRIVATE_SOURCES Private/*.h Private/*.cpp)
file(GLOB PUBLIC_SOURCES Public/*.h)
source_group(Private FILES ${PRIVATE_SOURCES})
source_group(Public FILES ${PUBLIC_SOURCES})

add_library(${MODULE_NAME} ${PRIVATE_SOURCES} ${PUBLIC_SOURCES})

include(GenerateExportHeader)
target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
generate_export_header(${MODULE_NAME} EXPORT_MACRO_NAME SCENEMGR_API EXPORT_FILE_NAME SceneMgrAPI.h)

target_include_directories(${MODULE_NAME} PRIVATE Private)
target_include_directories(${MODULE_NAME} PUBLIC Public)
target_compile_options(${MODULE_NAME} ${DEFAULT_COMPILE_OPTIONS})
target_link_libraries(${MODULE_NAME} PRIVATE Foundation)
target_link_libraries(${MODULE_NAME} PRIVATE Math)
