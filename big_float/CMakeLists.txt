add_library(big_float_lib STATIC
        src/big_float.cpp
        src/arithmetic.cpp
        src/logic.cpp
        src/helpers.cpp
        src/ios.cpp
        "src/ trigonometry.cpp"
        src/log.cpp
        tests/tests.cpp
)

target_include_directories(big_float_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(big_float_lib PRIVATE Boost::boost)