cmake_minimum_required(VERSION 3.16)
project(FileTransferSimulation VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(PROJECT_SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/File.cpp
    src/Customer.cpp
    src/Directory.cpp
    src/Simulation.cpp
)

add_executable(FileTransferSimulation ${PROJECT_SOURCES})

target_link_libraries(FileTransferSimulation PRIVATE 
    Qt5::Core
    Qt5::Gui 
    Qt5::Widgets
)
