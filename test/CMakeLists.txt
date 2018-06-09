add_executable(feature_template_test
  feature_template.cpp)
target_link_libraries(feature_template_test
  PRIVATE parsing
  PRIVATE gtest_main
  PRIVATE thread)
add_test(NAME feature_template_test COMMAND feature_template_test)
