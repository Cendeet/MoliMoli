cmake_minimum_required(VERSION 4.3)
project(MoliMoli)

if(MSVC)
    add_compile_options(/utf-8)
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

include(FetchContent)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
        GIT_TAG v7.0.3
)
FetchContent_MakeAvailable(ftxui)

add_executable(MoliMoli
        src/main.cpp
        src/dataManage.cpp
        src/userInterface.cpp
        src/add.cpp
)

target_include_directories(MoliMoli PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MoliMoli PRIVATE
        nlohmann_json
        ftxui::screen
        ftxui::dom
        ftxui::component
)

get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)

add_custom_command(TARGET MoliMoli POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MINGW_BIN_DIR}/libwinpthread-1.dll"
        "$<TARGET_FILE_DIR:MoliMoli>"
        COMMENT "Copying libwinpthread-1.dll to output directory"
)
