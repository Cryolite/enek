add_executable(test_unit_feature_template_parsing
  common.cpp
  text_position_iterator.cpp
  integer_literal.cpp
  feature_template.cpp)
target_include_directories(test_unit_feature_template_parsing
  PRIVATE .)
target_link_libraries(test_unit_feature_template_parsing
  PRIVATE feature_template
  PRIVATE unicode
  PRIVATE gtest_main
  PRIVATE stdc++fs
  PRIVATE thread)
gtest_discover_tests(test_unit_feature_template_parsing
  EXTRA_ARGS --gtest_catch_exceptions=0)
