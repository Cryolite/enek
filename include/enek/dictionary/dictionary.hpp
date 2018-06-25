#if !defined(ENEK_DICTIONARY_DICTIONARY_HPP_INCLUDE_GUARD)
#define ENEK_DICTIONARY_DICTIONARY_HPP_INCLUDE_GUARD

#include <enek/dictionary/word_descriptor.hpp>
#include <enek/dictionary/fundamental.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_categories.hpp>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <memory>
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <type_traits>
#include <cstdint>


namespace Enek::Dictionary{

class Dictionary
{
private:
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  static Enek::Dictionary::WordDescriptor getWordDescriptor_(
    Enek::Dictionary::WordIndex index,
    std::shared_ptr<Enek::Dictionary::WordIndex> const &p) noexcept;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

public:
  class WordIterator
    : public boost::iterator_facade<
        WordIterator,
        Enek::Dictionary::WordDescriptor,
        boost::forward_traversal_tag,
        Enek::Dictionary::WordDescriptor,
        std::make_signed_t<Enek::Dictionary::WordIndex>>
  {
  private:
    friend class Enek::Dictionary::Dictionary;

  private:
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
    WordIterator(
      Enek::Dictionary::WordIndex index,
      std::shared_ptr<Enek::Dictionary::WordIndex> const &p) noexcept;
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
    explicit WordIterator(Enek::Dictionary::WordIndex index) noexcept;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

  private:
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
    bool isIndexSingular_() const noexcept;

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
    Enek::Dictionary::WordIndex index_;
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
    std::weak_ptr<Enek::Dictionary::WordIndex> p_;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
  }; // class WordIterator

  friend class WordIterator;

  static_assert(
    std::is_same_v<boost::iterator_value<WordIterator>::type,
                   Enek::Dictionary::WordDescriptor>);
  static_assert(
    std::is_same_v<boost::iterator_reference<WordIterator>::type,
                   Enek::Dictionary::WordDescriptor>);
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

  Enek::Dictionary::WordIndex getNumWords() const noexcept;

  Enek::Dictionary::WordIndex
  getWordIndex(Enek::Dictionary::WordDescriptor const &w) const noexcept;

private:
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  std::shared_ptr<Enek::Dictionary::WordIndex> p_;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}; // class Dictionary

} // namespace Enek::Dictionary

#endif // !defined(ENEK_DICTIONARY_DICTIONARY_HPP_INCLUDE_GUARD)
