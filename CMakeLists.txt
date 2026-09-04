cmake_minimum_required(VERSION 3.10)
project(DungeonCrawler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)
find_package(nlohmann_json REQUIRED)

set(SOURCES
    src/main.cpp
    src/Tile.cpp
    src/Map.cpp
    src/entities/Player.cpp
    src/entities/Enemy.cpp
)

add_executable(DungeonCrawler ${SOURCES})

target_include_directories(DungeonCrawler PRIVATE src)

add_custom_command(TARGET DungeonCrawler POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)

target_link_libraries(DungeonCrawler raylib nlohmann_json::nlohmann_json)
if(APPLE)
    target_link_libraries(DungeonCrawler "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()