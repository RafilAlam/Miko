cmake_minimum_required(VERSION 4.2.3)

project(Miko)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/cli.cpp)

add_library(MikoLib STATIC ${PROJECT_SOURCE_DIR}/src/core.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE MikoLib)

target_include_directories(MikoLib PUBLIC ${PROJECT_SOURCE_DIR}/include)
