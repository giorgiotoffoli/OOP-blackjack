cmake_minimum_required(VERSION 3.20)
project(Blackjack LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BLACKJACK_ENABLE_TUI "Build the FTXUI terminal interface" ON)
if(BLACKJACK_ENABLE_TUI)
  include(FetchContent)
  FetchContent_Declare(ftxui
    URL https://github.com/ArthurSonzogni/FTXUI/archive/refs/tags/v6.1.9.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  )
  set(FTXUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(FTXUI_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(FTXUI_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(FTXUI_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(ftxui)
endif()

file(GLOB GAME_SOURCES CONFIGURE_DEPENDS src/*.cpp)
if(NOT BLACKJACK_ENABLE_TUI)
  list(FILTER GAME_SOURCES EXCLUDE REGEX "/TuiView\\.cpp$")
endif()
add_executable(game ${GAME_SOURCES})
target_include_directories(game PRIVATE include)
if(BLACKJACK_ENABLE_TUI)
  target_link_libraries(game PRIVATE ftxui::component)
else()
  target_compile_definitions(game PRIVATE BLACKJACK_TEXT_ONLY)
endif()

if(MINGW)
  target_link_options(game PRIVATE -static)
endif()
