include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(tests test_layout_contiguous_at_left.cpp test_layout_contiguous_at_right.cpp test_submdspan.cpp)
target_link_libraries(tests PRIVATE layout_contiguous GTest::gtest_main)
gtest_discover_tests(tests)

find_package(OpenMP REQUIRED)
add_library(vectorization test_vectorization.cpp)
target_link_libraries(vectorization PUBLIC layout_contiguous OpenMP::OpenMP_CXX)
target_compile_options(vectorization PUBLIC -Wall -Wextra)
