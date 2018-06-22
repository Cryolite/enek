add_library(dictionary
  dictionary.cpp)
target_include_directories(dictionary
  PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(dictionary
  PRIVATE util
  PUBLIC  boost_header
  PRIVATE thread)
