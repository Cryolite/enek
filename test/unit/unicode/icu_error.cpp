#include <enek/unicode/icu_error.hpp>
#include <gtest/gtest.h>
#include <unicode/utypes.h>
#include <string>
#include <system_error>


TEST(UnicodeIcuErrorTest, testSingleton)
{
  using Enek::Unicode::getICUErrorCategory;
  EXPECT_EQ(&getICUErrorCategory(), &getICUErrorCategory());
}

TEST(UnicodeIcuErrorTest, testName)
{
  EXPECT_STREQ("icu", Enek::Unicode::getICUErrorCategory().name());
}

TEST(UnicodeIcuErrorTest, testMessage)
{
  std::string message;
  message = Enek::Unicode::getICUErrorCategory().message(
    static_cast<int>(UErrorCode::U_BUFFER_OVERFLOW_ERROR));
  EXPECT_STREQ("ICU: U_BUFFER_OVERFLOW_ERROR (UErrorCode = 15)",
               message.c_str());
}
