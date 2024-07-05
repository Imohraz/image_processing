# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\image_processing_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\image_processing_autogen.dir\\ParseCache.txt"
  "image_processing_autogen"
  )
endif()
