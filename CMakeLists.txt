cmake_minimum_required(VERSION 3.12)

project(coel VERSION 0.0.2)

add_library(coel_window STATIC "src/window/win32.cpp")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(cmake/config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/coel-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/coel
    NO_SET_AND_CHECK_MACRO)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/coel-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/coel-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/coel-config-version.cmake
    DESTINATION
        ${CMAKE_INSTALL_DATADIR}/coel)

install(TARGETS coel_window EXPORT coel-targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT coel-targets
    NAMESPACE coel::
    DESTINATION ${CMAKE_INSTALL_DATADIR}/coel)
