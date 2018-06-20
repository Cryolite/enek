add_library(feature_template_visitor
  xml_dumper.cpp)
target_include_directories(feature_template_visitor
  PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(feature_template_visitor
  PUBLIC  feature_template_parsing
  PRIVATE util
  PRIVATE boost_header
  PUBLIC  thread)
