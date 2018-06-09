add_library(parsing
  integer_literal.cpp
  floating_literal.cpp
  boolean_literal.cpp
  string_literal.cpp
  ast.cpp
  parse.cpp)
target_include_directories(parsing
  PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(parsing
  PUBLIC  boost_header
  PUBLIC  util
  PRIVATE unicode
  PUBLIC  thread)
