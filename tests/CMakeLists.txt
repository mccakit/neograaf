find_package(GTest REQUIRED)
find_package(fmt REQUIRED)

add_library(graaf_utils_fixtures STATIC)
target_sources(graaf_utils_fixtures PUBLIC FILE_SET CXX_MODULES FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/fixtures.cpp)
target_link_libraries(graaf_utils_fixtures GTest::GTest graaf)

file(GLOB_RECURSE test_srcs CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*_test.cpp)

include(GoogleTest)
foreach(test_src ${test_srcs})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name}
        fmt::fmt-module
        GTest::GTest
        GTest::Main
        graaf
        graaf_utils_fixtures)
    gtest_discover_tests(${test_name})
endforeach()
gtest_discover_tests(graph_test)  # ← registers tests with CTest
