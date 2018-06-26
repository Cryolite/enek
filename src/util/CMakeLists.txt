add_library(util
  type_name.cpp
  abort.cpp
  assert.cpp
  exception.cpp
  throw.cpp
  string_key.cpp)
target_include_directories(util
  PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(util
  PUBLIC boost_exception
  PUBLIC boost_stacktrace
  PUBLIC thread)
