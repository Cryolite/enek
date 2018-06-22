#include <enek/util/abort.hpp>
#include <gtest/gtest.h>
#include <ostream>
#include <ios>
#include <csignal>


TEST(UtilAbortTest, testWithoutMessage)
{
  EXPECT_EXIT(ENEK_ABORT;,
              ::testing::KilledBySignal(SIGABRT),
              R"(abort\.cpp:15: .*UtilAbortTest.*testWithoutMessage.*: 
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
}

TEST(UtilAbortTest, testMessage)
{
  EXPECT_EXIT(ENEK_ABORT << "A dying message.";,
              ::testing::KilledBySignal(SIGABRT),
              R"(abort\.cpp:25: .*UtilAbortTest.*testMessage.*: A dying message\.
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
}

TEST(UtilAbortTest, testOStreamManipulator)
{
  EXPECT_EXIT(ENEK_ABORT << "First line." << std::endl
              << "Second line.";,
              ::testing::KilledBySignal(SIGABRT),
              R"(abort\.cpp:37: .*UtilAbortTest.*testOStreamManipulator.*: First line.
Second line.
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
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

TEST(UtilAbortTest, testIosManipulator)
{
  EXPECT_EXIT(ENEK_ABORT << getMessage;,
              ::testing::KilledBySignal(SIGABRT),
              R"(abort\.cpp:59: .*UtilAbortTest.*testIosManipulator.*: `std::ios' manipulator\.
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
}

TEST(UtilAbortTest, testIosBaseManipulator)
{
  EXPECT_EXIT(ENEK_ABORT << std::boolalpha << false;,
              ::testing::KilledBySignal(SIGABRT),
              R"(abort\.cpp:69: .*UtilAbortTest.*testIosBaseManipulator.*: false
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
}
