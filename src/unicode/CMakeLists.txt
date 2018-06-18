add_library(unicode
  icu_error.cpp
  char.cpp
  utf8.cpp)
target_include_directories(unicode
  PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(unicode
  PRIVATE util
  PRIVATE icu_uc
  PUBLIC  thread)
