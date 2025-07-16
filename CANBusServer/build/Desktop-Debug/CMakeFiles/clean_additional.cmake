# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appCANBus_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appCANBus_autogen.dir/ParseCache.txt"
  "appCANBus_autogen"
  )
endif()
