add_subdirectory(parsing)

add_library(feature_template
  input_type.cpp)
target_include_directories(feature_template
  PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(feature_template
  PUBLIC  feature_template_parsing
  PRIVATE util
  PUBLIC  thread)
