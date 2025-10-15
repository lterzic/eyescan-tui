cmake_minimum_required (VERSION 3.22)

project(eyescan-tui
  LANGUAGES CXX
  VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
  GIT_TAG        v5.0.0
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ftxui)
# ------------------------------------------------------------------------------

add_executable(eyescan-tui
  src/main.cpp
  src/display.cpp
  src/sweep.cpp
)
target_include_directories(eyescan-tui PRIVATE src)

target_link_libraries(eyescan-tui
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
