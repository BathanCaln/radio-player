# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/RadioPlayer_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/RadioPlayer_autogen.dir/ParseCache.txt"
  "RadioPlayer_autogen"
  )
endif()
