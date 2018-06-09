add_library(unicode
  char.cpp
  append_code_point.cpp)
target_include_directories(unicode
  PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(unicode
  PRIVATE util
  PRIVATE icu_uc
  PUBLIC  thread)
