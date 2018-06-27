#include <enek/dictionary/dictionary.hpp>
#include <enek/dictionary/word_descriptor.hpp>
#include <enek/dictionary/attribute_set.hpp>
#include <enek/dictionary/attribute_descriptor.hpp>
#include <enek/dictionary/fundamental.hpp>
#include <enek/util/throw.hpp>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <enek/util/abort.hpp>
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <enek/util/assert.hpp>
#include <vector>
#include <string_view>
#include <string>
#include <memory>
#include <utility>
#include <stdexcept>


namespace Enek::Dictionary{

namespace Detail{

class DictionaryImpl
{
public:
  DictionaryImpl() noexcept
    : attribute_sets_()
    , words_()
  {}

  DictionaryImpl(DictionaryImpl const &) = delete;

  DictionaryImpl &operator=(DictionaryImpl const &) = delete;

  Enek::Dictionary::WordIndex getNumWords() const noexcept
  {
    return words_.size();
  }

  Enek::Dictionary::ColumnIndex getNumWordColumns(
    Enek::Dictionary::WordIndex word_index) const noexcept
  {
    ENEK_ASSERT(word_index < words_.size());
    return words_[word_index].size();
  }

  Enek::Dictionary::AttributeDescriptor getWordAttribute(
    Enek::Dictionary::WordIndex word_index,
    Enek::Dictionary::ColumnIndex column_index) const noexcept
  {
    ENEK_ASSERT(word_index < words_.size());
    auto const &word = words_[word_index];
    ENEK_ASSERT(column_index < word.size());
    return word[column_index];
  }

  std::string_view getWordAttributeStringView(
    Enek::Dictionary::ColumnIndex column_index,
    Enek::Dictionary::AttributeDescriptor const &a) const noexcept
  {
    ENEK_ASSERT(column_index < attribute_sets_.size());
    auto const &attribute_set = attribute_sets_[column_index];
    return attribute_set.getStringView(a);
  }

  Enek::Dictionary::WordIndex
  addWord(std::vector<std::string> &&attribute_strings)
  {
    if (attribute_strings.size() > Enek::Dictionary::getMaxNumColumns()) {
      ENEK_THROW<std::invalid_argument>(
        "The number of columns for a word exceeds the largest possible "
        "value.");
    }
    if (attribute_sets_.size() < attribute_strings.size()) {
      attribute_sets_.resize(attribute_strings.size());
    }
    std::vector<Enek::Dictionary::AttributeDescriptor> word;
    for (Enek::Dictionary::ColumnIndex i = 0;
         i < attribute_strings.size();
         ++i) {
      auto const &attribute_string = attribute_strings[i];
      auto &attribute_set = attribute_sets_[i];
      auto a = attribute_set.add(std::move(attribute_string));
      word.emplace_back(std::move(a));
    }
    Enek::Dictionary::WordIndex const word_index = words_.size();
    words_.emplace_back(std::move(word));
    return word_index;
  }

private:
  std::vector<Enek::Dictionary::AttributeSet> attribute_sets_;
  std::vector<std::vector<Enek::Dictionary::AttributeDescriptor>> words_;
}; // class DictionaryImpl

namespace{

DictionaryImpl &castToImpl(std::shared_ptr<void> const &p_impl) noexcept
{
  return *static_cast<DictionaryImpl *>(p_impl.get());
}

DictionaryImpl const &castToConstImpl(std::shared_ptr<void> const &p_impl) noexcept
{
  return *static_cast<DictionaryImpl const *>(p_impl.get());
}

} // namespace *unnamed*

} // namespace Detail

Enek::Dictionary::WordDescriptor
Dictionary::getWordDescriptor(Enek::Dictionary::WordIndex index) const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  return Enek::Dictionary::WordDescriptor(index, p_impl_);
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  return index;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

#if defined(ENEK_ENABLE_ASSERT_PARANOID)
Dictionary::WordIterator::WordIterator(
  Enek::Dictionary::WordIndex index,
  Dictionary const *p_dictionary,
  std::shared_ptr<Enek::Dictionary::WordIndex const> const &p_size) noexcept
  : index_(index)
  , p_dictionary_(p_dictionary)
  , p_size_(p_size)
{
  if (p_dictionary != nullptr) {
    if (p_size.use_count() > 0) {
      if (p_size != nullptr) {
        if (index == 0 || index == *p_size) {
          return;
        }
        ENEK_ABORT << "`index' is an invalid value.";
      }
      ENEK_ABORT << "`p_size' is the null pointer value.";
    }
    ENEK_ABORT << "`p_size' is empty.";
  }
  ENEK_ABORT << "`p_dictionary' is the null pointer value.";
}
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
Dictionary::WordIterator::WordIterator(
  Enek::Dictionary::WordIndex index) noexcept
  : index_(index)
{
  ENEK_ASSERT(index_ <= Enek::Dictionary::getMaxNumWordsInDictionary());
}
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

Dictionary::WordIterator::WordIterator() noexcept
  : index_(Enek::Dictionary::getSingularWordIndex())
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  , p_dictionary_()
  , p_size_()
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
{}

Dictionary::WordIterator::WordIterator(WordIterator const &rhs) noexcept
  : index_(rhs.index_)
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  , p_dictionary_(rhs.p_dictionary_)
  , p_size_(rhs.p_size_)
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (rhs.isInvalidated()) {
    ENEK_ABORT << "Attempt to copy from an invalidated word iterator.";
  }
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

Dictionary::WordIterator::WordIterator(WordIterator &&rhs) noexcept
  : WordIterator()
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (rhs.isInvalidated()) {
    ENEK_ABORT << "Attempt to move from an invalidated word iterator.";
  }
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
  this->swap(rhs);
}

void Dictionary::WordIterator::swap(WordIterator &rhs) noexcept
{
  using std::swap;
  swap(index_,        rhs.index_);
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  swap(p_dictionary_, rhs.p_dictionary_);
  swap(p_size_,       rhs.p_size_);
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

Dictionary::WordIterator &
Dictionary::WordIterator::operator=(WordIterator const &rhs) noexcept
{
  WordIterator(rhs).swap(*this);
  return *this;
}

Dictionary::WordIterator &
Dictionary::WordIterator::operator=(WordIterator &&rhs) noexcept
{
  WordIterator(std::move(rhs)).swap(*this);
  return *this;
}

#if defined(ENEK_ENABLE_ASSERT_PARANOID)

bool Dictionary::WordIterator::isIndexSingular_() const noexcept
{
  return index_ == Enek::Dictionary::getSingularWordIndex();
}

bool Dictionary::WordIterator::isDictionaryPointerNull_() const noexcept
{
  return p_dictionary_ == nullptr;
}

bool Dictionary::WordIterator::isSizePointerEmpty_() const noexcept
{
  using SizePointer = std::weak_ptr<Enek::Dictionary::WordIndex const>;
  return !p_size_.owner_before(SizePointer())
    && !SizePointer().owner_before(p_size_);
}

bool Dictionary::WordIterator::isSingular() const noexcept
{
  ENEK_ASSERT(this->isIndexSingular_() == this->isDictionaryPointerNull_());
  ENEK_ASSERT(this->isIndexSingular_() == this->isSizePointerEmpty_());
  return this->isIndexSingular_();
}

bool Dictionary::WordIterator::isInvalidated() const noexcept
{
  ENEK_ASSERT(this->isIndexSingular_() == this->isDictionaryPointerNull_());
  ENEK_ASSERT(this->isIndexSingular_() == this->isSizePointerEmpty_());
  return !this->isIndexSingular_() && p_size_.expired();
}

#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

Dictionary::WordIterator::reference
Dictionary::WordIterator::dereference() const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (auto const p = p_size_.lock()) {
    if (index_ < *p) {
      ENEK_ASSERT(!this->isDictionaryPointerNull_());
      return p_dictionary_->getWordDescriptor(index_);
    }
    ENEK_ASSERT(index_ == *p);
    ENEK_ABORT << "Attempt to dereference a past-the-end word iterator.";
  }
  if (this->isSingular()) {
    ENEK_ABORT << "Attempt to dereference a singular word iterator.";
  }
  ENEK_ASSERT(this->isInvalidated());
  ENEK_ABORT << "Attempt to dereference an invalidated word iterator.";
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  ENEK_ASSERT(index_ < Enek::Dictionary::getMaxNumWordsInDictionary());
  return index_;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

bool Dictionary::WordIterator::equal(WordIterator const &rhs) const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (auto const p = p_size_.lock()) {
    if (auto const q = rhs.p_size_.lock()) {
      if (p == q) {
        ENEK_ASSERT(index_ <= *p);
        ENEK_ASSERT(rhs.index_ <= *q);
        ENEK_ASSERT(!this->isDictionaryPointerNull_());
        ENEK_ASSERT(p_dictionary_ == rhs.p_dictionary_);
        return index_ == rhs.index_;
      }
      ENEK_ABORT << "Attempt to compare word iterators from different "
                    "dictionaries.";
    }
    if (rhs.isSingular()) {
      ENEK_ABORT << "Attempt to compare a valid word iterator to a singular "
                    "word iterator.";
    }
    ENEK_ASSERT(rhs.isInvalidated());
    ENEK_ABORT << "Attempt to compare a valid word iterator to an invalidated "
                  "iterator.";
  }
  if (this->isSingular()) {
    if (!rhs.p_size_.expired()) {
      ENEK_ABORT << "Attempt to compare a singular word iterator to a valid "
                    "word iterator.";
    }
    if (rhs.isSingular()) {
      return true;
    }
    ENEK_ASSERT(rhs.isInvalidated());
    ENEK_ABORT << "Attempt to compare a singular word iterator to an "
                  "invalidated word iterator.";
  }
  ENEK_ASSERT(this->isInvalidated());
  if (!rhs.p_size_.expired()) {
    ENEK_ABORT << "Attempt to compare an invalidated word iterator to a valid "
                  "word iterator.";
  }
  if (rhs.isSingular()) {
    ENEK_ABORT << "Attempt to compare an invalidated word iterator to a "
                  "singular word iterator.";
  }
  ENEK_ASSERT(rhs.isInvalidated());
  ENEK_ABORT << "Attempt to compare invalidated word iterators.";
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  ENEK_ASSERT(index_ < Enek::Dictionary::getMaxNumWordsInDictionary()
                || index_ == Enek::Dictionary::getSingularWordIndex());
  ENEK_ASSERT(rhs.index_ < Enek::Dictionary::getMaxNumWordsInDictionary()
                || rhs.index_ == Enek::Dictionary::getSingularWordIndex());
  ENEK_ASSERT((index_ == Enek::Dictionary::getSingularWordIndex())
                == (rhs.index_ == Enek::Dictionary::getSingularWordIndex()));
  return index_ == rhs.index_;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

void Dictionary::WordIterator::increment() noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (auto const p = p_size_.lock()) {
    if (index_ < *p) {
      ENEK_ASSERT(!this->isDictionaryPointerNull_());
      ++index_;
      return;
    }
    ENEK_ASSERT(index_ == *p);
    ENEK_ABORT << "Attempt to increment a past-the-end word iterator.";
  }
  if (this->isSingular()) {
    ENEK_ABORT << "Attempt to increment a singular word iterator.";
  }
  ENEK_ASSERT(this->isInvalidated());
  ENEK_ABORT << "Attempt to increment an invalidated word iterator.";
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  ENEK_ASSERT(index_ < Enek::Dictionary::getMaxNumWordsInDictionary());
  ++index_;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

void swap(Dictionary::WordIterator &lhs, Dictionary::WordIterator &rhs) noexcept
{
  lhs.swap(rhs);
}

Dictionary::Dictionary() noexcept
  : p_impl_()
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  , p_size_()
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
{}

Dictionary::WordIterator Dictionary::beginWords() const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  return this->getNumWords() != 0 ? WordIterator(0, this, p_size_) : WordIterator();
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  return WordIterator(0);
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

Dictionary::WordIterator Dictionary::endWords() const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  Enek::Dictionary::WordIndex const num_words = this->getNumWords();
  return num_words != 0 ? WordIterator(num_words, this, p_size_) : WordIterator();
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  return WordIterator(this->getNumWords());
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

Dictionary::WordIteratorRange Dictionary::iterateWords() const noexcept
{
  return WordIteratorRange(this->beginWords(), this->endWords());
}

Enek::Dictionary::WordIndex Dictionary::getNumWords() const noexcept
{
  return p_impl_ != nullptr
    ? Detail::castToConstImpl(p_impl_).getNumWords() : 0;
}

Enek::Dictionary::WordIndex Dictionary::getWordIndex(
  Enek::Dictionary::WordDescriptor const &w) const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (w.getValidationPointer() == p_impl_) {
    ENEK_ASSERT(w.getIndex() < this->getNumWords());
    return w.getIndex();
  }
  ENEK_ABORT << "Attempt to get the index of a word descriptor from a "
                "different dictionary.";
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  ENEK_ASSERT(w < Enek::Dictionary::getMaxNumWordsInDictionary());
  return w;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

Enek::Dictionary::ColumnIndex Dictionary::getNumWordColumns(
  Enek::Dictionary::WordDescriptor const &w) const noexcept
{
  ENEK_ASSERT(p_impl_ != nullptr);
  Enek::Dictionary::WordIndex const word_index = this->getWordIndex(w);
  return Detail::castToConstImpl(p_impl_).getNumWordColumns(word_index);
}

Enek::Dictionary::AttributeDescriptor Dictionary::getWordAttribute(
  Enek::Dictionary::WordDescriptor const &w,
  Enek::Dictionary::ColumnIndex column_index) const noexcept
{
  ENEK_ASSERT(p_impl_ != nullptr);
  Enek::Dictionary::WordIndex const word_index = this->getWordIndex(w);
  return Detail::castToConstImpl(p_impl_)
    .getWordAttribute(word_index, column_index);
}

std::string_view Dictionary::getWordAttributeStringView(
  Enek::Dictionary::WordDescriptor const &w,
  Enek::Dictionary::ColumnIndex column_index) const noexcept
{
  ENEK_ASSERT(p_impl_ != nullptr);
  Enek::Dictionary::AttributeDescriptor const
    a = this->getWordAttribute(w, column_index);
  return Detail::castToConstImpl(p_impl_)
    .getWordAttributeStringView(column_index, a);
}

Enek::Dictionary::WordDescriptor
Dictionary::addWord(std::vector<std::string> &&attribute_strings)
{
  if (p_impl_ == nullptr) {
    p_impl_ = std::make_shared<Detail::DictionaryImpl>();
  }
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  std::shared_ptr<Enek::Dictionary::WordIndex const> p_size_new
    = std::make_shared<Enek::Dictionary::WordIndex const>(this->getNumWords());
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
  Enek::Dictionary::WordIndex const word_index
    = Detail::castToImpl(p_impl_).addWord(std::move(attribute_strings));
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  p_size_.swap(p_size_new);
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
  return this->getWordDescriptor(word_index);
}

} // namespace Enek::Dictionary
