#include <enek/feature_template/parsing/text_position_iterator.hpp>
#include <gtest/gtest.h>
#include <boost/range/end.hpp>
#include <boost/range/begin.hpp>
#include <iterator>
#include <string>


TEST(FeatureTemplateParsingTextPositionIteratorTest, testIncrement)
{
  using Enek::FeatureTemplate::Parsing::makeTextPositionIteratorRange;
  {
    std::string str("a\nb");
    auto r = makeTextPositionIteratorRange(str);
    auto first = boost::begin(r);
    auto iter = first;
    auto const last = boost::end(r);
    EXPECT_EQ('a', *iter);
    EXPECT_EQ(0, iter.getLineNumber());
    EXPECT_EQ(0, iter.getColumnNumber());
    EXPECT_TRUE(iter.getLineFirstPosition() == first.getBaseIterator());
    EXPECT_TRUE(iter.getLineLastPosition(last.getBaseIterator()) == std::next(iter).getBaseIterator());
    ++iter;
    EXPECT_EQ('\n', *iter);
    EXPECT_EQ(0, iter.getLineNumber());
    EXPECT_EQ(1, iter.getColumnNumber());
    EXPECT_TRUE(iter.getLineFirstPosition() == first.getBaseIterator());
    EXPECT_TRUE(iter.getLineLastPosition(last.getBaseIterator()) == iter.getBaseIterator());
    ++iter;
    EXPECT_EQ('b', *iter);
    EXPECT_EQ(1, iter.getLineNumber());
    EXPECT_EQ(0, iter.getColumnNumber());
    EXPECT_TRUE(iter.getLineFirstPosition() == iter.getBaseIterator());
    EXPECT_TRUE(iter.getLineLastPosition(last.getBaseIterator()) == last.getBaseIterator());
    first = iter;
    ++iter;
    EXPECT_EQ(1, iter.getLineNumber());
    EXPECT_EQ(1, iter.getColumnNumber());
    EXPECT_TRUE(iter.getLineFirstPosition() == first.getBaseIterator());
    EXPECT_TRUE(iter.getLineLastPosition(last.getBaseIterator()) == last.getBaseIterator());
    EXPECT_TRUE(iter == last);
  }
  {
    std::string str("a\rb");
    auto r = makeTextPositionIteratorRange(str);
    auto first = boost::begin(r);
    auto iter = first;
    auto const last = boost::end(r);
    EXPECT_EQ('a', *iter);
    EXPECT_EQ(0, iter.getLineNumber());
    EXPECT_EQ(0, iter.getColumnNumber());
    EXPECT_TRUE(iter.getLineFirstPosition() == first.getBaseIterator());
    EXPECT_TRUE(iter.getLineLastPosition(last.getBaseIterator()) == std::next(iter).getBaseIterator());
    ++iter;
    EXPECT_EQ('\r', *iter);
    EXPECT_EQ(0, iter.getLineNumber());
    EXPECT_EQ(1, iter.getColumnNumber());
    EXPECT_TRUE(iter.getLineFirstPosition() == first.getBaseIterator());
    EXPECT_TRUE(iter.getLineLastPosition(last.getBaseIterator()) == iter.getBaseIterator());
    ++iter;
    EXPECT_EQ('b', *iter);
    EXPECT_EQ(1, iter.getLineNumber());
    EXPECT_EQ(0, iter.getColumnNumber());
    EXPECT_TRUE(iter.getLineFirstPosition() == iter.getBaseIterator());
    EXPECT_TRUE(iter.getLineLastPosition(last.getBaseIterator()) == last.getBaseIterator());
    first = iter;
    ++iter;
    EXPECT_EQ(1, iter.getLineNumber());
    EXPECT_EQ(1, iter.getColumnNumber());
    EXPECT_TRUE(iter.getLineFirstPosition() == first.getBaseIterator());
    EXPECT_TRUE(iter.getLineLastPosition(last.getBaseIterator()) == last.getBaseIterator());
    EXPECT_TRUE(iter == last);
  }
  {
    std::string str("a\r\nb");
    auto r = makeTextPositionIteratorRange(str);
    auto first = boost::begin(r);
    auto iter = first;
    auto const last = boost::end(r);
    EXPECT_EQ('a', *iter);
    EXPECT_EQ(0, iter.getLineNumber());
    EXPECT_EQ(0, iter.getColumnNumber());
    EXPECT_TRUE(iter.getLineFirstPosition() == first.getBaseIterator());
    EXPECT_TRUE(iter.getLineLastPosition(last.getBaseIterator()) == std::next(iter).getBaseIterator());
    ++iter;
    EXPECT_EQ('\r', *iter);
    EXPECT_EQ(0, iter.getLineNumber());
    EXPECT_EQ(1, iter.getColumnNumber());
    EXPECT_TRUE(iter.getLineFirstPosition() == first.getBaseIterator());
    EXPECT_TRUE(iter.getLineLastPosition(last.getBaseIterator()) == iter.getBaseIterator());
    ++iter;
    EXPECT_EQ('\n', *iter);
    EXPECT_EQ(1, iter.getLineNumber());
    EXPECT_EQ(0, iter.getColumnNumber());
    EXPECT_TRUE(iter.getLineFirstPosition() == iter.getBaseIterator());
    EXPECT_TRUE(iter.getLineLastPosition(last.getBaseIterator()) == last.getBaseIterator());
    first = iter;
    ++iter;
    EXPECT_EQ('b', *iter);
    EXPECT_EQ(1, iter.getLineNumber());
    EXPECT_EQ(0, iter.getColumnNumber());
    EXPECT_TRUE(iter.getLineFirstPosition() == iter.getBaseIterator());
    EXPECT_TRUE(iter.getLineLastPosition(last.getBaseIterator()) == last.getBaseIterator());
    first = iter;
    ++iter;
    EXPECT_EQ(1, iter.getLineNumber());
    EXPECT_EQ(1, iter.getColumnNumber());
    EXPECT_TRUE(iter.getLineFirstPosition() == first.getBaseIterator());
    EXPECT_TRUE(iter.getLineLastPosition(last.getBaseIterator()) == last.getBaseIterator());
    EXPECT_TRUE(iter == last);
  }
}
