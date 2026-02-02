cmake_minimum_required(VERSION 4.2.3)

project(Miko)

include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG main
)
FetchContent_Declare(
  llama
  GIT_REPOSITORY https://github.com/ggerganov/llama.cpp
  GIT_TAG master

)
FetchContent_MakeAvailable(ftxui)
FetchContent_MakeAvailable(llama)

file(GLOB src_cli ${CMAKE_SOURCE_DIR}/src/cli/*.cpp)
file(GLOB src_mikolib ${CMAKE_SOURCE_DIR}/src/mikolib/*.cpp)

add_library(MikoLib STATIC ${src_mikolib})
target_include_directories(MikoLib PUBLIC ${CMAKE_SOURCE_DIR}/include/mikolib)
target_link_libraries(MikoLib PRIVATE llama)

add_executable(${PROJECT_NAME} ${src_cli})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/cli)
target_link_libraries(${PROJECT_NAME} PRIVATE
  MikoLib
  ftxui::component
  ftxui::dom
  ftxui::screen
)
