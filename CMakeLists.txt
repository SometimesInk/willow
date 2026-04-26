cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  willow
  VERSION 0.0.0
  LANGUAGES C)

find_package(camellia REQUIRED)

add_library(willow)

target_link_libraries(willow PUBLIC camellia::camellia)

target_sources(
  willow
  PRIVATE src/willow/sys/env.c src/willow/conf/config.c src/willow/io/io.c
          src/willow/conf/parser.c
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         include
         FILES
         include/willow/conf/config.h
         include/willow/conf/parser.h
         include/willow/io/io.h
         include/willow/sys/env.h)

target_include_directories(
  willow
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(willow PUBLIC c_std_11)

add_library(willow::willow ALIAS willow)

# Docs with Doxygen
option(BUILD_DOCUMENTATION "Build API documentation with Doxygen" OFF)

if(BUILD_DOCUMENTATION)
  find_package(Doxygen)

  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is required to build documentation")
  endif()

  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

  set(DOXYGEN_CONFIG_FILE ${DOXYGEN_OUT})

  doxygen_add_docs(
    docs ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src
    COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif()

# Testing
enable_testing()

add_subdirectory(tests)
