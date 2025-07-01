# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appscada_client_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appscada_client_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\open62541_lib_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\open62541_lib_autogen.dir\\ParseCache.txt"
  "appscada_client_autogen"
  "open62541_lib_autogen"
  )
endif()
