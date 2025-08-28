cmake_minimum_required(VERSION 3.5)

project(CANBus LANGUAGES CXX)

# Общие настройки
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем подпроекты
add_subdirectory(CanLib)
add_subdirectory(CANBusClient)
add_subdirectory(CANBusServer)


