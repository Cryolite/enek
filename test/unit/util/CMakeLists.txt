if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set_source_files_properties(throw.cpp
    PROPERTIES COMPILE_FLAGS -Wno-terminate)
endif()

add_executable(test_unit_util
  assert.cpp
  throw.cpp)
target_link_libraries(test_unit_util
  PRIVATE util
  PRIVATE gtest_main
  PRIVATE thread)
gtest_discover_tests(test_unit_util
  EXTRA_ARGS --gtest_catch_exceptions=0)
