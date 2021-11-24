cmake_minimum_required(VERSION 3.16)

project(layout_contiguous LANGUAGES CXX)

include(CTest)

find_package(mdspan CONFIG REQUIRED)

add_library(layout_contiguous INTERFACE)
target_include_directories(layout_contiguous INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/layout_contiguous>)
target_include_directories(layout_contiguous SYSTEM INTERFACE
  $<INSTALL_INTERFACE:include>)
target_link_libraries(layout_contiguous INTERFACE std::mdspan)
target_compile_features(layout_contiguous INTERFACE cxx_std_17)
add_library(LayoutContiguous::layout_contiguous ALIAS layout_contiguous)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

install(
  TARGETS layout_contiguous
  EXPORT LayoutContiguousTargets)

install(
  EXPORT LayoutContiguousTargets
  FILE LayoutContiguousTargets.cmake
  NAMESPACE LayoutContiguous::
  DESTINATION lib/cmake/LayoutContiguous)

install(
  DIRECTORY src/layout_contiguous
  DESTINATION include/layout_contiguous)

include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/LayoutContiguousConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/LayoutContiguousConfig.cmake
  INSTALL_DESTINATION lib/cmake/LayoutContiguous)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/LayoutContiguousConfig.cmake
  DESTINATION lib/cmake/LayoutContiguous)
