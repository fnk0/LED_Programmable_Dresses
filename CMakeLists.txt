cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME Spring2016_Dresses)
project(${PROJECT_NAME})

set(${CMAKE_PROJECT_NAME}_SKETCH Spring2016_Dresses.ino)
generate_arduino_firmware(${CMAKE_PROJECT_NAME})
