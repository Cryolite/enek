# As of 2018/06/24 (marisa 0.2.6), marisa-trie's default CFLAGS and CXXFLAGS
# are `-g -O2`, and LDFLAGS is the empty string.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(ENEK_EXT_MARISA_TRIE_CFLAGS -g)
  set(ENEK_EXT_MARISA_TRIE_CXXFLAGS -g)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
  set(ENEK_EXT_MARISA_TRIE_CFLAGS "-g -O2")
  set(ENEK_EXT_MARISA_TRIE_CXXFLAGS "-g -O2")
else()
  message(FATAL_ERROR "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} is not supported.")
endif()

if (ENEK_ENABLE_ASAN)
  set(ENEK_EXT_MARISA_TRIE_CFLAGS "${ENEK_EXT_MARISA_TRIE_CFLAGS} -fsanitize=address")
  set(ENEK_EXT_MARISA_TRIE_CXXFLAGS "${ENEK_EXT_MARISA_TRIE_CXXFLAGS} -fsanitize=address")
  set(ENEK_EXT_MARISA_TRIE_LDFLAGS "${ENEK_EXT_MARISA_TRIE_LDFLAGS} -fsanitize=address")
endif()

if (ENEK_ENABLE_UBSAN)
  set(ENEK_EXT_MARISA_TRIE_CFLAGS "${ENEK_EXT_MARISA_TRIE_CFLAGS} -fsanitize=undefined")
  set(ENEK_EXT_MARISA_TRIE_CXXFLAGS "${ENEK_EXT_MARISA_TRIE_CXXFLAGS} -fsanitize=undefined")
  set(ENEK_EXT_MARISA_TRIE_LDFLAGS "${ENEK_EXT_MARISA_TRIE_LDFLAGS} -fsanitize=undefined")
endif()

if (ENEK_ENABLE_TSAN)
  set(ENEK_EXT_MARISA_TRIE_CFLAGS "${ENEK_EXT_MARISA_TRIE_CFLAGS} -fsanitize=thread")
  set(ENEK_EXT_MARISA_TRIE_CXXFLAGS "${ENEK_EXT_MARISA_TRIE_CXXFLAGS} -fsanitize=thread")
  set(ENEK_EXT_MARISA_TRIE_LDFLAGS "${ENEK_EXT_MARISA_TRIE_LDFLAGS} -fsanitize=thread")
endif()

if (ENEK_ENABLE_LIBSTDCXX_DEBUG_MODE)
  set(ENEK_EXT_MARISA_TRIE_CXXFLAGS "${ENEK_EXT_MARISA_TRIE_CXXFLAGS} -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC")
endif()

include(ExternalProject)

# `marisa-trie` assumes in-source build. Otherwise its build fails.
ExternalProject_Add(marisa-trie-install
  GIT_REPOSITORY https://github.com/s-yata/marisa-trie.git
  CONFIGURE_COMMAND autoreconf -i && env "CFLAGS=${ENEK_EXT_MARISA_TRIE_CFLAGS}" "CXXFLAGS=${ENEK_EXT_MARISA_TRIE_CXXFLAGS}" "LDFLAGS=${ENEK_EXT_MARISA_TRIE_LDFLAGS}" ./configure --prefix=<INSTALL_DIR>
  BUILD_COMMAND type nproc && make -j $(nproc) || make
  BUILD_IN_SOURCE ON)

ExternalProject_Get_Property(marisa-trie-install INSTALL_DIR)

add_library(marisa-trie INTERFACE)
target_include_directories(marisa-trie INTERFACE "${INSTALL_DIR}/include")
if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
  target_link_libraries(marisa-trie INTERFACE -L "${INSTALL_DIR}/lib" -l marisa)
else()
  message(FATAL_ERROR "CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID} is not supported.")
endif()
add_dependencies(marisa-trie marisa-trie-build)
