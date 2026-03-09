cmake_minimum_required(VERSION 3.20)
project(Pacman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Recherche de SFML 3 (vcpkg s'occupe du reste)
find_package(SFML 3 COMPONENTS Graphics Window System Audio System CONFIG REQUIRED)

# 2. Sources
file(GLOB SOURCES "src/*.cpp")
add_executable(Pacman ${SOURCES})

target_include_directories(Pacman PRIVATE include)

# 3. Liens avec les cibles SFML::
target_link_libraries(Pacman PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::System)

add_custom_command(TARGET Pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:Pacman>/assets"
)