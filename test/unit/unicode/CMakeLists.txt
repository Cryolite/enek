add_executable(test_unit_unicode
  char.cpp)
target_link_libraries(test_unit_unicode
  PRIVATE unicode
  PRIVATE gtest_main
  PRIVATE thread)
gtest_discover_tests(test_unit_unicode
  EXTRA_ARGS --gtest_catch_exceptions=0)
