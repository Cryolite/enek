add_executable(test_unit_feature_template_parsing
  text_position_iterator.cpp
  feature_template.cpp)
target_link_libraries(test_unit_feature_template_parsing
  PRIVATE parsing
  PRIVATE unicode
  PRIVATE gtest_main
  PRIVATE thread)
gtest_discover_tests(test_unit_feature_template_parsing
  EXTRA_ARGS --gtest_catch_exceptions=0)
