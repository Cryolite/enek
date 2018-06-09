cmake_minimum_required(VERSION 3.11.2)
project(Enek)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC)
  add_definitions(-DENEK_DEBUG)
endif()
add_definitions(-DENEK_GIT_COMMIT_HASH="")

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
  add_compile_options(-fsanitize=address)
  set(CMAKE_EXE_LINKER_FLAGS -fsanitize=address)
endif()

add_subdirectory(ext)
add_subdirectory(src)
add_subdirectory(test)
