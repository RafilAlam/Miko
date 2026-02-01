cmake_minimum_required(VERSION 4.2.3)

project(Miko)

add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/llamacpp)

add_library(MikoLib STATIC ${CMAKE_SOURCE_DIR}/src/core.cpp)
target_include_directories(MikoLib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(MikoLib PRIVATE llama)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/cli.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE MikoLib)
