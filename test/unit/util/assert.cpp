#include <enek/util/assert.hpp>
#include <gtest/gtest.h>
#include <ostream>
#include <ios>
#include <csignal>


#if defined(ENEK_ENABLE_ASSERT)

TEST(UtilAssertTest, testWithoutMessage)
{
  static_assert(SIGABRT == 6);
#if defined(ENEK_ENABLE_ASSERT)
  ASSERT_EXIT(ENEK_ASSERT(false);,
              [](int exit_code) { return exit_code == 128 + SIGABRT; },
              R"(assert\.cpp:18: .+: Assertion `false' failed\.
(Git commit hash: |Backtrace:
))");
#else // defined(ENEK_ENABLE_ASSERT)
  ENEK_ASSERT(false);
#endif // defined(ENEK_ENABLE_ASSERT)
}

TEST(UtilAssertTest, testMessage)
{
#if defined(ENEK_ENABLE_ASSERT)
  ASSERT_EXIT(ENEK_ASSERT(false) << "Death message.";,
              [](int exit_code) { return exit_code == 128 + SIGABRT; },
              R"(assert\.cpp:32: .+: Assertion `false' failed\.
Death message\.
(Git commit hash: |Backtrace:
))");
#else // defined(ENEK_ENABLE_ASSERT)
  ENEK_ASSERT(false) << "Death message.";
#endif // defined(ENEK_ENABLE_ASSERT)
}

TEST(UtilAssertTest, testOStreamManipulator)
{
#if defined(ENEK_ENABLE_ASSERT)
  ASSERT_EXIT(ENEK_ASSERT(false) << "First line." << std::endl
              << "Second line.";,
              [](int exit_code) { return exit_code == 128 + SIGABRT; },
              R"(assert\.cpp:48: .+: Assertion `false' failed\.
First line.
Second line.
(Git commit hash: |Backtrace:
))");
#else // defined(ENEK_ENABLE_ASSERT)
  ENEK_ASSERT(false) << "First line." << std::endl << "Second line.";
#endif // defined(ENEK_ENABLE_ASSERT)
}

namespace{

std::ios &getMessage(std::ios &ios)
{
  char str[] = "`std::ios' manipulator.";
  static_assert(sizeof(str) == 24);
  ios.rdbuf()->sputn(str, sizeof(str) - 1);
  return ios;
}

} // namespace *unnamed*

TEST(UtilAssertTest, testIosManipulator)
{
#if defined(ENEK_ENABLE_ASSERT)
  ASSERT_EXIT(ENEK_ASSERT(false) << getMessage;,
              [](int exit_code) { return exit_code == 128 + SIGABRT; },
              R"(assert\.cpp:74: .+: Assertion `false' failed.
`std::ios' manipulator\.
(Git commit hash: |Backtrace:
))");
#else // defined(ENEK_ENABLE_ASSERT)
  ENEK_ASSERT(false) << getMessage;
#endif // defined(ENEK_ENABLE_ASSERT)
}

TEST(UtilAssertTest, testIosBaseManipulator)
{
#if defined(ENEK_ENABLE_ASSERT)
  ASSERT_EXIT(ENEK_ASSERT(false) << std::boolalpha << false;,
              [](int exit_code) { return exit_code == 128 + SIGABRT; },
              R"(assert\.cpp:88: .+: Assertion `false' failed.
false
(Git commit hash: |Backtrace:
))");
#else // defined(ENEK_ENABLE_ASSERT)
  ENEK_ASSERT(false) << std::boolalpha << false;
#endif // defined(ENEK_ENABLE_ASSERT)
}

#endif // defined(ENEK_ENABLE_ASSERT)
