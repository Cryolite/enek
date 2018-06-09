cmake_minimum_required(VERSION 3.11.2)

find_package(Boost
  REQUIRED
  COMPONENTS stacktrace_backtrace exception
)

add_library(boost_header INTERFACE)
target_link_libraries(boost_header
  INTERFACE Boost::boost)

add_library(boost_stacktrace INTERFACE)
target_link_libraries(boost_stacktrace
  INTERFACE Boost::stacktrace_backtrace)
if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")
  if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    target_compile_definitions(boost_stacktrace
      INTERFACE BOOST_STACKTRACE_LINK
      INTERFACE BOOST_STACKTRACE_USE_BACKTRACE)
    target_link_libraries(boost_stacktrace
      INTERFACE backtrace
      INTERFACE dl
      INTERFACE thread
      INTERFACE "-rdynamic")
  else ()
    message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID}: An unsupported compiler.")
  endif ()
else ()
  message(FATAL_ERROR "${CMAKE_HOST_SYSTEM_NAME}: An unsupported system.")
endif ()

add_library(boost_exception INTERFACE)
target_link_libraries(boost_exception
  INTERFACE Boost::exception)
