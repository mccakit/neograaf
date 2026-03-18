add_library(graaf STATIC)
file(GLOB_RECURSE graaf_srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(graaf PUBLIC FILE_SET CXX_MODULES FILES ${graaf_srcs})
