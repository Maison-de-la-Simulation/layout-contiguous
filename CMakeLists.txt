cmake_minimum_required(VERSION 3.16)

project(layout_contiguous
  VERSION 0.0.0
  LANGUAGES CXX)

include(CTest)

find_package(mdspan CONFIG REQUIRED)

add_library(layout_contiguous INTERFACE)
target_include_directories(layout_contiguous
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/layout_contiguous>)
target_include_directories(layout_contiguous
  SYSTEM INTERFACE $<INSTALL_INTERFACE:include>)
target_link_libraries(layout_contiguous
  INTERFACE std::mdspan)
target_compile_features(layout_contiguous
  INTERFACE cxx_std_17)
add_library(layout_contiguous::layout_contiguous
  ALIAS layout_contiguous)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

install(
  TARGETS layout_contiguous
  EXPORT layout_contiguous-targets)

install(
  EXPORT layout_contiguous-targets
  NAMESPACE layout_contiguous::
  DESTINATION lib/cmake/layout_contiguous)

install(
  DIRECTORY src/layout_contiguous
  DESTINATION include)

include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/layout_contiguous-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/layout_contiguous-config.cmake
  INSTALL_DESTINATION lib/cmake/layout_contiguous)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/layout_contiguous-config-version.cmake
  COMPATIBILITY SameMajorVersion)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/layout_contiguous-config.cmake ${CMAKE_CURRENT_BINARY_DIR}/layout_contiguous-config-version.cmake
  DESTINATION lib/cmake/layout_contiguous)
