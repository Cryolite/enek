#include <enek/feature_template/type.hpp>
#include <gtest/gtest.h>
#include <sstream>


using Type = Enek::FeatureTemplate::Type;

TEST(FeatureTemplateTypeTest, testOStream)
{
  {
    std::ostringstream oss;
    oss << Type::unknown;
    EXPECT_STREQ("unknown", oss.str().c_str());
  }
  {
    std::ostringstream oss;
    oss << Type::integer;
    EXPECT_STREQ("integer", oss.str().c_str());
  }
  {
    std::ostringstream oss;
    oss << Type::floating;
    EXPECT_STREQ("floating", oss.str().c_str());
  }
  {
    std::ostringstream oss;
    oss << Type::boolean;
    EXPECT_STREQ("boolean", oss.str().c_str());
  }
  {
    std::ostringstream oss;
    oss << Type::string;
    EXPECT_STREQ("string", oss.str().c_str());
  }
}
