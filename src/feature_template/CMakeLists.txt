add_subdirectory(parsing)

add_library(feature_template_local
  type.cpp)
target_include_directories(feature_template_local
  PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(feature_template_local
  PRIVATE util
  PUBLIC  thread)

add_library(feature_template INTERFACE)
target_link_libraries(feature_template
  INTERFACE feature_template_parsing
  INTERFACE feature_template_local)
