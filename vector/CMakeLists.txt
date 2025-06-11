add_library(vector_lib STATIC
        src/vector.cpp
        src/helpers.cpp
        src/arithmetic.cpp
        src/products_3D.cpp
        src/products_7D.cpp
        src/ios.cpp
        src/access.cpp
        src/compare.cpp
        src/action_for_vector.cpp
)

target_include_directories(vector_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../big_float/inc
)

target_link_libraries(vector_lib PRIVATE Boost::boost big_float_lib)