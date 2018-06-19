add_library(feature_template_parsing
  input_type.cpp
  integer_literal.cpp
  floating_literal.cpp
  boolean_literal.cpp
  string_literal.cpp
  ast.cpp
  parse.cpp)
target_include_directories(feature_template_parsing
  PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(feature_template_parsing
  PUBLIC  boost_header
  PUBLIC  util
  PRIVATE unicode
  PUBLIC  thread)
