add_library(graaf STATIC)
file(
    GLOB_RECURSE graaf_srcs
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
target_sources(graaf PUBLIC FILE_SET CXX_MODULES FILES ${graaf_srcs})

install(
    TARGETS graaf
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    CXX_MODULES_BMI DESTINATION ${CMAKE_INSTALL_LIBDIR}/cxx/bmi
)
configure_file(${CMAKE_SOURCE_DIR}/packaging/graaf.pc.in graaf.pc @ONLY)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/graaf.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
