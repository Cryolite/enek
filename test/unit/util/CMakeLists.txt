add_executable(test_unit_util
  assert.cpp)
target_link_libraries(test_unit_util
  PRIVATE util
  PRIVATE gtest_main
  PRIVATE thread)
gtest_discover_tests(test_unit_util)

