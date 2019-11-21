cmake_minimum_required(VERSION 3.15)
project(FolderMonitoring)

set(CMAKE_CXX_STANDARD 11)

add_executable(FolderMonitoring main.cpp DirectoryMonitor.cpp DirectoryMonitor.hpp DbWriter.cpp DbWriter.hpp)