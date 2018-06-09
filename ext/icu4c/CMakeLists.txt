cmake_minimum_required(VERSION 3.11.2)

find_package(ICU
  REQUIRED
  COMPONENTS data uc)

add_library(icu_uc INTERFACE)
target_link_libraries(icu_uc
  INTERFACE ICU::uc
  INTERFACE thread)
