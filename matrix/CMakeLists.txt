add_library(matrix_lib STATIC
        src/matrix.cpp
        src/matrix_proxy.cpp
        src/access.cpp
        src/arithmetics.cpp
        src/helpers.cpp
        src/compare.cpp
        src/ios.cpp
        src/matrix_methods.cpp
)

target_include_directories(matrix_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../big_float/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../vector/inc
)

target_link_libraries(matrix_lib PRIVATE Boost::boost big_float_lib vector_lib)