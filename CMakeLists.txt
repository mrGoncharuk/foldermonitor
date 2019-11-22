cmake_minimum_required(VERSION 3.15)
project(foldermonitor)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)


add_executable(foldermonitor main.cpp DirectoryMonitor.cpp DirectoryMonitor.hpp DbWriter.cpp DbWriter.hpp)
target_link_libraries(foldermonitor LINK_PUBLIC sqlite3)
