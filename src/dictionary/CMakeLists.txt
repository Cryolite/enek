add_library(dictionary
  attribute_descriptor.cpp
  attribute_set_impl.cpp
  word_descriptor.cpp
  dictionary.cpp)
target_include_directories(dictionary
  PUBLIC  "${PROJECT_SOURCE_DIR}/include"
  PRIVATE .)
target_link_libraries(dictionary
  PRIVATE util
  PUBLIC  boost_header
  PRIVATE thread)
