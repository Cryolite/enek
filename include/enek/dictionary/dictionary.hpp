#if !defined(ENEK_DICTIONARY_DICTIONARY_HPP_INCLUDE_GUARD)
#define ENEK_DICTIONARY_DICTIONARY_HPP_INCLUDE_GUARD

#include <boost/range/iterator_range.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_categories.hpp>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <functional>
#include <memory>
#include <cstddef>
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <vector>
#include <string>
#include <type_traits>
#include <cstdint>
#include <limits>


namespace Enek{

typedef std::uint32_t WordIndex;

constexpr WordIndex getInvalidWordIndex() noexcept
{
  return std::numeric_limits<WordIndex>::max();
}

constexpr WordIndex getMaxNumWordsInDictionary() noexcept
{
  using WordIndexDifference = std::make_signed_t<WordIndex>;
  return std::numeric_limits<WordIndexDifference>::max();
}

#if defined(ENEK_ENABLE_ASSERT_PARANOID)

class Dictionary;

class WordDescriptor
{
private:
  friend class Enek::Dictionary;

  WordDescriptor() noexcept;

  WordDescriptor(Enek::WordIndex index,
                 std::shared_ptr<Enek::WordIndex> const &p) noexcept;

private:
  bool isWeakPointerEmpty_() const noexcept;

  bool isSingular_() const noexcept;

  bool isInvalidated_() const noexcept;

  void assertComparableTo_(WordDescriptor const &rhs) const noexcept;

public:
  WordDescriptor(WordDescriptor const &rhs) noexcept;

  WordDescriptor(WordDescriptor &&rhs) noexcept;

  void swap(WordDescriptor &rhs) noexcept;

  WordDescriptor &operator=(WordDescriptor const &rhs) noexcept;

  WordDescriptor &operator=(WordDescriptor &&rhs) noexcept;

public:
  bool operator==(WordDescriptor const &rhs) const noexcept;

  bool operator!=(WordDescriptor const &rhs) const noexcept;

  bool operator<(WordDescriptor const &rhs) const noexcept;

  bool operator>(WordDescriptor const &rhs) const noexcept;

  bool operator<=(WordDescriptor const &rhs) const noexcept;

  bool operator>=(WordDescriptor const &rhs) const noexcept;

private:
  friend class std::hash<Enek::WordDescriptor>;

private:
  Enek::WordIndex index_;
  std::weak_ptr<Enek::WordIndex> p_;
}; // class WordDescriptor

void swap(WordDescriptor &lhs, WordDescriptor &rhs) noexcept;

} // namespace Enek

namespace std{

template<>
struct hash<Enek::WordDescriptor>
{
  std::size_t operator()(Enek::WordDescriptor const &k) const;
}; // struct hash<Enek::WordDescriptor>

} // namespace std

namespace Enek{

#else // defined(ENEK_ENABLE_ASSERT_PARANOID)

typedef WordIndex WordDescriptor;

#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

class Dictionary
{
private:
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  static Enek::WordDescriptor getWordDescriptor_(
    Enek::WordIndex index, std::shared_ptr<Enek::WordIndex> const &p) noexcept;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

public:
  class WordIterator
    : public boost::iterator_facade<
        WordIterator,
        Enek::WordDescriptor,
        boost::forward_traversal_tag,
        Enek::WordDescriptor,
        std::make_signed_t<Enek::WordIndex>>
  {
  private:
    friend class Enek::Dictionary;

  private:
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
    WordIterator(Enek::WordIndex index,
                 std::shared_ptr<Enek::WordIndex> const &p) noexcept;
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
    explicit WordIterator(Enek::WordIndex index) noexcept;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

  private:
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
    bool isWeakPointerEmpty_() const noexcept;

    bool isSingular_() const noexcept;

    bool isInvalidated_() const noexcept;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

  public:
    WordIterator() noexcept;

    WordIterator(WordIterator const &rhs) noexcept;

    WordIterator(WordIterator &&rhs) noexcept;

    void swap(WordIterator &rhs) noexcept;

    WordIterator &operator=(WordIterator const &rhs) noexcept;

    WordIterator &operator=(WordIterator &&rhs) noexcept;

  private:
    friend class boost::iterator_core_access;

    reference dereference() const noexcept;

    bool equal(WordIterator const &rhs) const noexcept;

    void increment() noexcept;

  private:
    Enek::WordIndex index_;
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
    std::weak_ptr<Enek::WordIndex> p_;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
  }; // class WordIterator

  friend class WordIterator;

  static_assert(
    std::is_same_v<boost::iterator_value<WordIterator>::type,
                   Enek::WordDescriptor>);
  static_assert(
    std::is_same_v<boost::iterator_reference<WordIterator>::type,
                   Enek::WordDescriptor>);
  static_assert(
    std::is_same_v<boost::iterator_difference<WordIterator>::type,
                   std::int32_t>);
  static_assert(
    std::is_convertible_v<boost::iterator_traversal<WordIterator>::type,
                          boost::forward_traversal_tag>);
  static_assert(
    !std::is_convertible_v<boost::iterator_traversal<WordIterator>::type,
                           boost::bidirectional_traversal_tag>);

public:
  Dictionary();

  Dictionary(Dictionary const &) = delete;

  Dictionary &operator=(Dictionary const &) = delete;

  WordIterator beginWords() const noexcept;

  WordIterator endWords() const noexcept;

  using WordIteratorRange = boost::iterator_range<WordIterator>;

  WordIteratorRange iterateWords() const noexcept;

  Enek::WordIndex getNumWords() const noexcept;

  Enek::WordIndex getWordIndex(Enek::WordDescriptor const &w) const noexcept;

private:
  std::vector<std::vector<std::string>> words_;
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  std::shared_ptr<Enek::WordIndex> p_;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}; // class Dictionary

} // namespace Enek

#endif // !defined(ENEK_DICTIONARY_DICTIONARY_HPP_INCLUDE_GUARD)
