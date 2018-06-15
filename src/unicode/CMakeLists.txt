add_library(unicode
  char.cpp
  utf8.cpp)
target_include_directories(unicode
  PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(unicode
  PRIVATE util
  PRIVATE icu_uc
  PUBLIC  thread)
