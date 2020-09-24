set(CMAKE_CXX_STANDARD 17)

project(Tlang)

add_subdirectory(core)

aux_source_directory(. DIR_SRCS)

include_directories(./ ./core)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} core)

