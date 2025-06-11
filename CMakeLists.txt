cmake_minimum_required(VERSION 3.28)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

# Настройка Boost (общая для всех подпроектов)
set(BOOST_ROOT "C:/msys64/mingw64")
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.81.0 REQUIRED COMPONENTS system filesystem)

# Подключаем подпроекты
add_subdirectory(big_float)
add_subdirectory(vector)
add_subdirectory(matrix)

# Основной исполняемый файл
add_executable(untitled main.cpp)
target_link_libraries(untitled PRIVATE
        Boost::boost Boost::system Boost::filesystem
        big_float_lib vector_lib matrix_lib
)

# Общие include-директории
target_include_directories(untitled PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/big_float/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/vector/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/matrix/inc
)