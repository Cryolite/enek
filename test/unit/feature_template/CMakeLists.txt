add_subdirectory(parsing)

add_executable(test_unit_feature_template
  type.cpp)
target_link_libraries(test_unit_feature_template
  PRIVATE feature_template
  PRIVATE gtest_main
  PRIVATE thread)
gtest_discover_tests(test_unit_feature_template
  EXTRA_ARGS --gtest_catch_exceptions=0)
