find_package(benchmark REQUIRED)
find_package(fmt REQUIRED)

file(GLOB_RECURSE perf_srcs CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME}_perf ${perf_srcs})

target_link_libraries(
    ${PROJECT_NAME}_perf
    PRIVATE benchmark::benchmark benchmark::benchmark_main fmt::fmt-module graaf
)
