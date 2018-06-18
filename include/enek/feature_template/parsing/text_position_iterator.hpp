#if !defined(ENEK_FEATURE_TEMPLATE_PARSING_LINE_ITERATOR_HPP_INCLUDE_GUARD)
#define ENEK_FEATURE_TEMPLATE_PARSING_LINE_ITERATOR_HPP_INCLUDE_GUARD

#include <enek/unicode/utf8.hpp>
#include <enek/util/assert.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/end.hpp>
#include <boost/range/begin.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <type_traits>
#include <utility>
#include <limits>
#include <cstddef>


namespace Enek::FeatureTemplate::Parsing{

template<typename Iterator>
class TextPositionIterator
  : public boost::iterator_facade<
      TextPositionIterator<Iterator>,
      typename boost::iterator_value<Iterator>::type,
      boost::forward_traversal_tag,
      typename boost::iterator_reference<Iterator>::type,
      typename boost::iterator_difference<Iterator>::type>
{
private:
  using Facade_ = boost::iterator_facade<
    TextPositionIterator<Iterator>
  , typename boost::iterator_value<Iterator>::type
  , boost::forward_traversal_tag
  , typename boost::iterator_reference<Iterator>::type
  , typename boost::iterator_difference<Iterator>::type>;

  using This_ = TextPositionIterator;

public:
  using BaseIterator = Iterator;
  using typename Facade_::value_type;
  using typename Facade_::reference;

  static boost::iterator_range<TextPositionIterator>
  makeIteratorRange(BaseIterator const &first, BaseIterator const &last)
  {
    return boost::iterator_range<TextPositionIterator>(
      TextPositionIterator(first, std::false_type()),
      TextPositionIterator(last, std::true_type()));
  }

  static boost::iterator_range<TextPositionIterator>
  makeIteratorRange(boost::iterator_range<BaseIterator> const &r)
  {
    return boost::iterator_range<TextPositionIterator>(
      TextPositionIterator(boost::begin(r), std::false_type()),
      TextPositionIterator(boost::end(r), std::true_type()));
  }

private:
  static constexpr std::size_t
    invalid_line_number_ = std::numeric_limits<std::size_t>::max();

  static constexpr value_type invalid_prev_ = 0;
  static_assert(invalid_prev_ != '\r');
  static_assert(invalid_prev_ != '\n');

public:
  TextPositionIterator()
    : base_()
    , line_number_(invalid_line_number_)
    , line_first_()
    , prev_(invalid_prev_)
  {}

private:
  TextPositionIterator(Iterator const &base, std::false_type)
    : base_(base)
    , line_number_(0)
    , line_first_(base)
    , prev_(invalid_prev_)
  {}

  TextPositionIterator(Iterator const &base, std::true_type)
    : base_(base)
    , line_number_(invalid_line_number_)
    , line_first_()
    , prev_(invalid_prev_)
  {}

public:
  TextPositionIterator(TextPositionIterator const &rhs) = default;

  TextPositionIterator(TextPositionIterator &&rhs)
    : base_(std::move(rhs.base_))
    , line_number_(rhs.line_number_)
    , line_first_(std::move(rhs.line_first_))
    , prev_(rhs.prev_)
  {
    rhs.base_ = Iterator();
    rhs.line_number_ = invalid_line_number_;
    rhs.line_first_ = Iterator();
    rhs.prev_ = invalid_prev_;
  }

  void swap(TextPositionIterator &rhs)
  {
    using std::swap;
    swap(this->base_,  rhs.base_);
    swap(line_number_, rhs.line_number_);
    swap(line_first_,  rhs.line_first_);
    swap(prev_,        rhs.prev_);
  }

  friend void swap(TextPositionIterator &lhs, TextPositionIterator &rhs)
  {
    lhs.swap(rhs);
  }

  TextPositionIterator &operator=(TextPositionIterator const &rhs)
  {
    TextPositionIterator(rhs).swap(*this);
    return *this;
  }

  TextPositionIterator &operator=(TextPositionIterator &&rhs)
  {
    TextPositionIterator(std::move(rhs)).swap(*this);
    return *this;
  }

private:
  friend class boost::iterator_core_access;

  reference dereference() const noexcept(noexcept(*This_::base_))
  {
    return *this->base_;
  }

  bool equal(TextPositionIterator const &rhs)
    const noexcept(noexcept(This_::base_ == rhs.base_))
  {
    return this->base_ == rhs.base_;
  }

  void increment()
  {
    bool new_line = false;
    {
      reference ref = *this->base_;
      switch (ref) {
      case '\r':
        new_line = true;
        ++line_number_;
        break;
      case '\n':
        new_line = true;
        if (prev_ != '\r') {
          ++line_number_;
        }
        break;
      default:
        break;
      }
      prev_ = ref;
    }
    ++this->base_;
    if (new_line) {
      line_first_ = this->base_;
    }
  }

public:
  BaseIterator getBaseIterator()
    const noexcept(noexcept(BaseIterator(This_::base_)))
  {
    return this->base_;
  }

  bool hasTextPosition() const noexcept
  {
    return line_number_ != invalid_line_number_;
  }

  std::size_t getLineNumber() const noexcept
  {
    ENEK_ASSERT(this->hasTextPosition());
    return line_number_;
  }

  std::size_t getColumnNumber() const
  {
    ENEK_ASSERT(this->hasTextPosition());
    return Enek::Unicode::UTF8::getWidthOnConsole(line_first_, this->base_);
  }

  BaseIterator getLineFirstPosition() const
  {
    ENEK_ASSERT(this->hasTextPosition());
    return line_first_;
  }

  BaseIterator getLineLastPosition(BaseIterator const &upper_bound) const
  {
    Iterator iter = this->base_;
    if (*this->base_ == '\n' && prev_ == '\r') {
      ++iter;
    }
    for (; iter != upper_bound; ++iter) {
      if ((*iter == '\r') || (*iter == '\n')) {
        return iter;
      }
    }
    return upper_bound;
  }

private:
  Iterator base_;
  std::size_t line_number_;
  Iterator line_first_;
  value_type prev_;
}; // class TextPositionIterator

template<typename ForwardIterator>
auto makeTextPositionIteratorRange(ForwardIterator first, ForwardIterator last)
{
  using Iterator = TextPositionIterator<ForwardIterator>;
  return Iterator::makeIteratorRange(std::move(first), std::move(last));
}

template<typename ForwardRange>
auto makeTextPositionIteratorRange(ForwardRange &r)
{
  return makeTextPositionIteratorRange(boost::begin(r), boost::end(r));
}

} // namespace Enek::FeatureTemplate::Parsing

#endif // !defined(ENEK_FEATURE_TEMPLATE_PARSING_LINE_ITERATOR_HPP_INCLUDE_GUARD)
