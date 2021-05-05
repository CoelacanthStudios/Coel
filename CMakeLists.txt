cmake_minimum_required(VERSION 3.12)

project(coel 
    VERSION 0.0.7
    LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(coel_window "src/window/win32.cpp")
target_compile_features(coel_window PUBLIC cxx_std_20)
target_include_directories(coel_window
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
if(BUILD_SHARED_LIBS AND WIN32)
    target_compile_definitions(coel_window
        PRIVATE "COEL_EXPORT=__declspec(dllexport)"
        INTERFACE "COEL_EXPORT=__declspec(dllimport)")
endif()

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
