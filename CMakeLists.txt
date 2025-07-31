cmake_minimum_required(VERSION 3.16)
project(parquet_qt)
add_subdirectory(table)
add_subdirectory(display)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT display)
