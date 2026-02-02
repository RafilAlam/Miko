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

# add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/llamacpp)

add_library(MikoLib STATIC ${CMAKE_SOURCE_DIR}/src/core.cpp)
target_include_directories(MikoLib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(MikoLib PRIVATE llama)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/cli.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
  MikoLib
  ftxui::component
  ftxui::dom
  ftxui::screen
)
