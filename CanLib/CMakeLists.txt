cmake_minimum_required(VERSION 3.16)

project(CanLib LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core SerialBus)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core SerialBus)

add_library(CanLib SHARED
  CanLib_global.h
  caninterface.cpp
  caninterface.h
  customlogger.h customlogger.cpp
)



target_link_libraries(CanLib
  PRIVATE Qt${QT_VERSION_MAJOR}::Core
  PRIVATE Qt${QT_VERSION_MAJOR}::SerialBus
)

target_include_directories(CanLib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)


target_compile_definitions(CanLib PRIVATE CANLIB_LIBRARY)
