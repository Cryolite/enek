add_executable(test_unit_feature_template_parsing
  feature_template.cpp)
target_link_libraries(test_unit_feature_template_parsing
  PRIVATE parsing
  PRIVATE gtest_main
  PRIVATE thread)
gtest_discover_tests(test_unit_feature_template_parsing)
