#include <enek/dictionary/dictionary.hpp>
#include <enek/dictionary/word_descriptor.hpp>
#include <enek/dictionary/fundamental.hpp>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <enek/util/abort.hpp>
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <enek/util/assert.hpp>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <memory>
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <utility>


namespace Enek::Dictionary{

#if defined(ENEK_ENABLE_ASSERT_PARANOID)

Enek::Dictionary::WordDescriptor Dictionary::getWordDescriptor_(
  Enek::Dictionary::WordIndex index,
  std::shared_ptr<Enek::Dictionary::WordIndex> const &p) noexcept
{
  return Enek::Dictionary::WordDescriptor(index, p);
}

#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

#if defined(ENEK_ENABLE_ASSERT_PARANOID)
Dictionary::WordIterator::WordIterator(
  Enek::Dictionary::WordIndex index,
  std::shared_ptr<Enek::Dictionary::WordIndex> const &p) noexcept
  : index_(index)
  , p_(p)
{
  if (p.use_count() > 0) {
    if (p != nullptr) {
      if (*p <= Enek::Dictionary::getMaxNumWordsInDictionary()) {
        if (index < *p) {
          return;
        }
        ENEK_ABORT << "The number of words in a dictionary exceeds the "
                      "largest possible value.";
      }
      ENEK_ABORT << "`index' is out-of-range.";
    }
    ENEK_ABORT << "`p' is the null pointer value.";
  }
  ENEK_ABORT << "`p' is empty.";
}
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
Dictionary::WordIterator::WordIterator(
  Enek::Dictionary::WordIndex index) noexcept
  : index_(index)
{
  ENEK_ASSERT(index_ <= Enek::Dictionary::getMaxNumWordsInDictionary());
}
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

#if defined(ENEK_ENABLE_ASSERT_PARANOID)
bool Dictionary::WordIterator::isIndexSingular_() const noexcept
{
  return index_ == Enek::Dictionary::getSingularWordIndex();
}

bool Dictionary::WordIterator::isWeakPointerEmpty_() const noexcept
{
  return !p_.owner_before(std::weak_ptr<int>())
    && !std::weak_ptr<int>().owner_before(p_);
}

bool Dictionary::WordIterator::isSingular_() const noexcept
{
  ENEK_ASSERT(this->isIndexSingular_() == this->isWeakPointerEmpty_());
  return this->isIndexSingular_();
}

bool Dictionary::WordIterator::isInvalidated_() const noexcept
{
  ENEK_ASSERT(this->isIndexSingular_() == this->isWeakPointerEmpty_());
  return !this->isIndexSingular_() && p_.expired();
}
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

Dictionary::WordIterator::WordIterator() noexcept
  : index_(Enek::Dictionary::getSingularWordIndex())
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  , p_()
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
{}

Dictionary::WordIterator::WordIterator(WordIterator const &rhs) noexcept
  : index_(rhs.index_)
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  , p_(rhs.p_)
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (rhs.isInvalidated_()) {
    ENEK_ABORT << "Attempt to copy from an invalidated word iterator.";
  }
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

Dictionary::WordIterator::WordIterator(WordIterator &&rhs) noexcept
  : WordIterator()
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (rhs.isInvalidated_()) {
    ENEK_ABORT << "Attempt to move from an invalidated word iterator.";
  }
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
  this->swap(rhs);
}

void Dictionary::WordIterator::swap(WordIterator &rhs) noexcept
{
  using std::swap;
  swap(index_, rhs.index_);
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  swap(p_,     rhs.p_);
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

void swap(Dictionary::WordIterator &lhs,
          Dictionary::WordIterator &rhs) noexcept
{
  lhs.swap(rhs);
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

Dictionary::WordIterator::reference
Dictionary::WordIterator::dereference() const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (auto const p = p_.lock()) {
    if (index_ < *p) {
      return Dictionary::getWordDescriptor_(index_, p);
    }
    ENEK_ABORT << "Attempt to dereference a past-the-end word iterator.";
  }
  if (this->isSingular_()) {
    ENEK_ABORT << "Attempt to dereference a singular word iterator.";
  }
  ENEK_ASSERT(this->isInvalidated_());
  ENEK_ABORT << "Attempt to dereference an invalidated word iterator.";
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  return index_;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

bool Dictionary::WordIterator::equal(WordIterator const &rhs) const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (auto const p = p_.lock()) {
    if (auto const q = rhs.p_.lock()) {
      if (p == q) {
        ENEK_ASSERT(index_ <= *p);
        ENEK_ASSERT(rhs.index_ <= *q);
        return index_ == rhs.index_;
      }
      ENEK_ABORT << "Attempt to compare word iterators from different "
                    "dictionaries.";
    }
    if (rhs.isSingular_()) {
      ENEK_ABORT << "Attempt to compare a valid word iterator to a singular "
                    "word iterator.";
    }
    ENEK_ASSERT(rhs.isInvalidated_());
    ENEK_ABORT << "Attempt to compare a valid word iterator to an invalidated "
                  "iterator.";
  }
  if (this->isSingular_()) {
    if (!rhs.p_.expired()) {
      ENEK_ABORT << "Attempt to compare a singular word iterator to a valid "
                    "word iterator.";
    }
    if (rhs.isSingular_()) {
      return true;
    }
    ENEK_ASSERT(rhs.isInvalidated_());
    ENEK_ABORT << "Attempt to compare a singular word iterator to an "
                  "invalidated word iterator.";
  }
  ENEK_ASSERT(this->isInvalidated_());
  if (!rhs.p_.expired()) {
    ENEK_ABORT << "Attempt to compare an invalidated word iterator to a valid "
                  "word iterator.";
  }
  if (rhs.isSingular_()) {
    ENEK_ABORT << "Attempt to compare an invalidated word iterator to a "
                  "singular word iterator.";
  }
  ENEK_ASSERT(rhs.isInvalidated_());
  ENEK_ABORT << "Attempt to compare invalidated word iterators.";
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  return index_ == rhs.index_;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

void Dictionary::WordIterator::increment() noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (auto const p = p_.lock()) {
    if (index_ < *p) {
      ++index_;
      return;
    }
    ENEK_ABORT << "Attempt to increment a past-the-end word iterator.";
  }
  if (this->isSingular_()) {
    ENEK_ABORT << "Attempt to increment a singular word iterator.";
  }
  ENEK_ASSERT(this->isInvalidated_());
  ENEK_ABORT << "Attempt to increment an invalidated word iterator.";
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  ENEK_ASSERT(index_ < Enek::Dictionary::getMaxNumWordsInDictionary());
  ++index_;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

Dictionary::Dictionary()
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  : p_(std::make_shared<Enek::Dictionary::WordIndex>(0))
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
{}

Dictionary::WordIterator Dictionary::beginWords() const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  return WordIterator(0, p_);
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  return WordIterator(0);
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

Dictionary::WordIterator Dictionary::endWords() const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  return WordIterator(this->getNumWords(), p_);
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
  return 0;
}

Enek::Dictionary::WordIndex Dictionary::getWordIndex(
  Enek::Dictionary::WordDescriptor const &w) const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (auto const p = w.p_.lock()) {
    if (p == p_) {
      return w.index_;
    }
    ENEK_ABORT << "Attempt to get the index of a word descriptor from a "
                  "different dictionary.";
  }
  if (w.isSingular_()) {
    ENEK_ABORT << "Attempt to get the index of a singular word descriptor.";
  }
  ENEK_ASSERT(w.isInvalidated_());
  ENEK_ABORT << "Attempt to get the index of an invalidated word descriptor.";
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  ENEK_ASSERT(w != Enek::Dictionary::getInvalidWordIndex());
  return w;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

} // namespace Enek::Dictionary
