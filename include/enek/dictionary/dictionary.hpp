#if !defined(ENEK_DICTIONARY_DICTIONARY_HPP_INCLUDE_GUARD)
#define ENEK_DICTIONARY_DICTIONARY_HPP_INCLUDE_GUARD

#include <enek/dictionary/word_descriptor.hpp>
#include <enek/dictionary/attribute_descriptor.hpp>
#include <enek/dictionary/fundamental.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <vector>
#include <string_view>
#include <string>
#include <type_traits>
#include <memory>
#include <cstdint>


namespace Enek::Dictionary{

class Dictionary
{
private:
  Enek::Dictionary::WordDescriptor
  getWordDescriptor(Enek::Dictionary::WordIndex index) const noexcept;

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
      Dictionary const *p_dictionary,
      std::shared_ptr<Enek::Dictionary::WordIndex const> const &p_size) noexcept;
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
    explicit WordIterator(WordIndex index) noexcept;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

  public:
    WordIterator() noexcept;

    WordIterator(WordIterator const &rhs) noexcept;

    WordIterator(WordIterator &&rhs) noexcept;

    void swap(WordIterator &rhs) noexcept;

    WordIterator &operator=(WordIterator const &rhs) noexcept;

    WordIterator &operator=(WordIterator &&rhs) noexcept;

  private:
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
    bool isIndexSingular_() const noexcept;

    bool isDictionaryPointerNull_() const noexcept;

    bool isSizePointerEmpty_() const noexcept;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

  private:
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
    bool isSingular() const noexcept;

    bool isInvalidated() const noexcept;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

  private:
    friend class boost::iterator_core_access;

    reference dereference() const noexcept;

    bool equal(WordIterator const &rhs) const noexcept;

    void increment() noexcept;

  private:
    WordIndex index_;
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
    Dictionary const *p_dictionary_;
    std::weak_ptr<WordIndex const> p_size_;
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
  Dictionary() noexcept;

  Dictionary(Dictionary const &) = delete;

  Dictionary &operator=(Dictionary const &) = delete;

  WordIterator beginWords() const noexcept;

  WordIterator endWords() const noexcept;

  using WordIteratorRange = boost::iterator_range<WordIterator>;

  WordIteratorRange iterateWords() const noexcept;

  Enek::Dictionary::WordIndex getNumWords() const noexcept;

  Enek::Dictionary::WordIndex getWordIndex(
    Enek::Dictionary::WordDescriptor const &w) const noexcept;

  Enek::Dictionary::ColumnIndex getNumWordColumns(
    Enek::Dictionary::WordDescriptor const &w) const noexcept;

  Enek::Dictionary::AttributeDescriptor getWordAttribute(
    Enek::Dictionary::WordDescriptor const &w,
    Enek::Dictionary::ColumnIndex column_index) const noexcept;

  std::string_view getWordAttributeStringView(
    Enek::Dictionary::WordDescriptor const &w,
    Enek::Dictionary::ColumnIndex column_index) const noexcept;

  Enek::Dictionary::WordDescriptor
  addWord(std::vector<std::string> &&attribute_strings);

private:
  std::shared_ptr<void> p_impl_;
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  std::shared_ptr<Enek::Dictionary::WordIndex const> p_size_;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}; // class Dictionary

void swap(Dictionary::WordIterator &lhs, Dictionary::WordIterator &rhs) noexcept;

} // namespace Enek::Dictionary

#endif // !defined(ENEK_DICTIONARY_DICTIONARY_HPP_INCLUDE_GUARD)
