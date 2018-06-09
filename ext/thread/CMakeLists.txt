cmake_minimum_required(VERSION 3.11.2)

add_library(thread INTERFACE)
target_compile_options(thread
  INTERFACE "-pthread")
target_link_libraries(thread
  INTERFACE "-pthread")
