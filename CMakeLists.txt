cmake_minimum_required (VERSION 2.6)
project (OCR-Project)
# The version number.
set (OCRProject_VERSION_MAJOR 0)
set (OCRProject_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/OCRProjectConfig.h.in"
  "${PROJECT_BINARY_DIR}/OCRProjectConfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(OCRProject recognition_test.cc)