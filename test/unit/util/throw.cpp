#include <enek/util/throw.hpp>
#include <gtest/gtest.h>
#include <boost/exception/get_error_info.hpp>
#include <boost/exception/exception.hpp>
#include <boost/stacktrace/stacktrace.hpp>
#include <regex>
#include <stdexcept>
#include <csignal>
#include <exception>


TEST(UtilThrowTest, testWithoutMessage)
{
  std::exception_ptr p;
  try {
    ENEK_THROW<std::runtime_error>("");
  }
  catch (...) {
    p = std::current_exception();
  }
  try {
    std::rethrow_exception(p);
  }
  catch (std::runtime_error const &e) {
    EXPECT_STREQ("", e.what());
    p = std::current_exception();
  }
  try {
    std::rethrow_exception(p);
  }
  catch (boost::exception const &e) {
    {
      char const * const *p = boost::get_error_info<boost::throw_file>(e);
      ASSERT_NE(p, nullptr);
      std::regex r("throw\\.cpp$");
      EXPECT_TRUE(std::regex_search(*p, r));
    }
    {
      char const * const *p = boost::get_error_info<boost::throw_function>(e);
      ASSERT_NE(p, nullptr);
      std::regex r("UtilThrowTest.*testWithoutMessage");
      EXPECT_TRUE(std::regex_search(*p, r));
    }
    {
      int const *p = boost::get_error_info<boost::throw_line>(e);
      ASSERT_NE(p, nullptr);
      EXPECT_EQ(16, *p);
    }
    {
      boost::stacktrace::stacktrace const *p
        = boost::get_error_info<Enek::StackTraceErrorInfo>(e);
      ASSERT_NE(p, nullptr);
      EXPECT_GT(p->size(), 0);
    }
    {
      char const * const *p
        = boost::get_error_info<Enek::GitCommitHashErrorInfo>(e);
      ASSERT_NE(p, nullptr);
      EXPECT_STREQ(*p, ENEK_GIT_COMMIT_HASH);
    }
  }
}

TEST(UtilThrowTest, testMessage)
{
  std::exception_ptr p;
  try {
    ENEK_THROW<std::runtime_error>("An error message.");
  }
  catch (...) {
    p = std::current_exception();
  }
  try {
    std::rethrow_exception(p);
  }
  catch (std::runtime_error const &e) {
    EXPECT_STREQ("An error message.", e.what());
    p = std::current_exception();
  }
  try {
    std::rethrow_exception(p);
  }
  catch (boost::exception const &e) {
    {
      char const * const *p = boost::get_error_info<boost::throw_file>(e);
      ASSERT_NE(p, nullptr);
      std::regex r("throw\\.cpp$");
      EXPECT_TRUE(std::regex_search(*p, r));
    }
    {
      char const * const *p = boost::get_error_info<boost::throw_function>(e);
      ASSERT_NE(p, nullptr);
      std::regex r("UtilThrowTest.*testMessage");
      EXPECT_TRUE(std::regex_search(*p, r));
    }
    {
      int const *p = boost::get_error_info<boost::throw_line>(e);
      ASSERT_NE(p, nullptr);
      EXPECT_EQ(68, *p);
    }
    {
      boost::stacktrace::stacktrace const *p
        = boost::get_error_info<Enek::StackTraceErrorInfo>(e);
      ASSERT_NE(p, nullptr);
      EXPECT_GT(p->size(), 0);
    }
    {
      char const * const *p
        = boost::get_error_info<Enek::GitCommitHashErrorInfo>(e);
      ASSERT_NE(p, nullptr);
      EXPECT_STREQ(*p, ENEK_GIT_COMMIT_HASH);
    }
  }
}

TEST(UtilThrowTest, testTerminateWithoutMessage)
{
  EXPECT_EXIT([]() noexcept { ENEK_THROW<std::runtime_error>(""); }();,
              ::testing::KilledBySignal(SIGABRT),
              R"(^`std::terminate' is called after throwing an instance of `.+'\.
.*throw\.cpp:124: .*UtilThrowTest.*testTerminateWithoutMessage.*: 
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
}

TEST(UtilThrowTest, testTerminate)
{
  EXPECT_EXIT(
    []() noexcept { ENEK_THROW<std::runtime_error>("An error message."); }();,
    ::testing::KilledBySignal(SIGABRT),
    R"(^`std::terminate' is called after throwing an instance of `.+'\.
.*throw\.cpp:136: .*UtilThrowTest.*testTerminate.*: An error message\.
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
}

TEST(UtilThrowTest, testTerminateHandlerWithStdException)
{
  EXPECT_EXIT(
    []() noexcept { throw std::runtime_error("An error message."); }();,
    ::testing::KilledBySignal(SIGABRT),
    R"(^`std::terminate' is called after throwing an instance of `.+'\.
An error message\.
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
}

TEST(UtilThrowTest, testTerminateHandlerWithUnknownException)
{
  EXPECT_EXIT(
    []() noexcept { throw 0; }();,
    ::testing::KilledBySignal(SIGABRT),
    R"(^`std::terminate' is called after throwing an instance of an unknown type\.
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
}

TEST(UtilThrowTest, testDirectCallOfTerminateHandler)
{
  EXPECT_EXIT(
    std::terminate();,
    ::testing::KilledBySignal(SIGABRT),
    R"(^`std::terminate' is called without throwing any exception\.
(Git commit hash: [[:xdigit:]]+
)?Backtrace:
)");
}
