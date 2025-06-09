cmake_minimum_required(VERSION 3.28)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

set(BOOST_ROOT "C:/msys64/mingw64")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_DEBUG ON)

find_package(Boost 1.81.0 REQUIRED COMPONENTS system filesystem)

message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Boost includes: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")

add_executable(untitled main.cpp
        big_float/inc/bigfloat.hpp
        big_float/src/big_float.cpp
        big_float/src/arithmetic.cpp
        big_float/src/logic.cpp
        big_float/src/helpers.cpp
        big_float/src/ios.cpp
        "big_float/src/ trigonometry.cpp"
        big_float/tests/tests.cpp
        big_float/src/log.cpp)

target_include_directories(untitled PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(untitled PRIVATE Boost::boost Boost::system Boost::filesystem)