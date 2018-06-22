#include <enek/dictionary/dictionary.hpp>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <enek/util/throw.hpp>
#include <enek/util/abort.hpp>
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <enek/util/assert.hpp>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <functional>
#include <memory>
#include <stdexcept>
#include <cstddef>
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <utility>


namespace Enek{

#if defined(ENEK_ENABLE_ASSERT_PARANOID)

WordDescriptor::WordDescriptor() noexcept
  : index_(Enek::getInvalidWordIndex())
  , p_()
{}

WordDescriptor::WordDescriptor(
  Enek::WordIndex index,
  std::shared_ptr<Enek::WordIndex> const &p) noexcept
  : index_(index)
  , p_(p)
{
  if (p.use_count() > 0) {
    if (p != nullptr) {
      if (*p <= Enek::getMaxNumWordsInDictionary()) {
        if (index < *p) {
          return;
        }
        ENEK_ABORT << "`index' is out-of-range.";
      }
      ENEK_ABORT << "The number of words in a dictionary exceeds the largest "
                    "possible value.";
    }
    ENEK_ABORT << "`p' is the null pointer value.";
  }
  ENEK_ABORT << "`p' is empty.";
}

bool WordDescriptor::isWeakPointerEmpty_() const noexcept
{
  return !p_.owner_before(std::weak_ptr<int>())
    && !std::weak_ptr<int>().owner_before(p_);
}

bool WordDescriptor::isSingular_() const noexcept
{
  ENEK_ASSERT(
    (index_ == Enek::getInvalidWordIndex()) == this->isWeakPointerEmpty_());
  return index_ == Enek::getInvalidWordIndex();
}

bool WordDescriptor::isInvalidated_() const noexcept
{
  ENEK_ASSERT(
    (index_ == Enek::getInvalidWordIndex()) == this->isWeakPointerEmpty_());
  return index_ != Enek::getInvalidWordIndex() && p_.expired();
}

WordDescriptor::WordDescriptor(WordDescriptor const &rhs) noexcept
  : index_(rhs.index_)
  , p_(rhs.p_)
{
  if (rhs.isInvalidated_()) {
    ENEK_ABORT << "Attempt to copy from an invalidated word descriptor.";
  }
}

WordDescriptor::WordDescriptor(WordDescriptor &&rhs) noexcept
  : WordDescriptor()
{
  if (rhs.isInvalidated_()) {
    ENEK_ABORT << "Attempt to move from an invalidated word descriptor.";
  }
  this->swap(rhs);
}

void WordDescriptor::swap(WordDescriptor &rhs) noexcept
{
  using std::swap;
  swap(index_, rhs.index_);
  swap(p_,     rhs.p_);
}

WordDescriptor &WordDescriptor::operator=(WordDescriptor const &rhs) noexcept
{
  WordDescriptor(rhs).swap(*this);
  return *this;
}

WordDescriptor &WordDescriptor::operator=(WordDescriptor &&rhs) noexcept
{
  WordDescriptor(std::move(rhs)).swap(*this);
  return *this;
}

void WordDescriptor::assertComparableTo_(
  WordDescriptor const &rhs) const noexcept
{
  if (auto const p = p_.lock()) {
    if (auto const q = rhs.p_.lock()) {
      if (p == q) {
        ENEK_ASSERT(index_ <= *p);
        ENEK_ASSERT(rhs.index_ <= *q);
        return;
      }
      ENEK_ABORT << "Attempt to compare word descriptors from different "
                    "dictionaries.";
    }
    if (rhs.isSingular_()) {
      ENEK_ABORT << "Attempt to compare a valid word descriptor to a "
                    "singular word descriptor.";
    }
    ENEK_ASSERT(rhs.isInvalidated_());
    ENEK_ABORT << "Attempt to compare a valid word descriptor to an "
                  "invalidated word descriptor.";
  }
  if (this->isSingular_()) {
    if (!rhs.p_.expired()) {
      ENEK_ABORT << "Attempt to compare a singular word descriptor to a valid "
                    "word descriptor.";
    }
    if (rhs.isSingular_()) {
      return;
    }
    ENEK_ASSERT(rhs.isInvalidated_());
    ENEK_ABORT << "Attempt to compare a singular word descriptor to an "
                  "invalidated word descriptor.";
  }
  ENEK_ASSERT(this->isInvalidated_());
  if (!rhs.p_.expired()) {
    ENEK_ABORT << "Attempt to compare an invalidated word descriptor to a "
                  "valid word descriptor.";
  }
  if (rhs.isSingular_()) {
    ENEK_ABORT << "Attempt to compare an invalidated word descriptor to a "
                  "singular word descriptor.";
  }
  ENEK_ASSERT(rhs.isInvalidated_());
  ENEK_ABORT << "Attempt to compare invalidated word descriptores.";
}

bool WordDescriptor::operator==(WordDescriptor const &rhs) const noexcept
{
  this->assertComparableTo_(rhs);
  return index_ == rhs.index_;
}

bool WordDescriptor::operator!=(WordDescriptor const &rhs) const noexcept
{
  this->assertComparableTo_(rhs);
  return index_ != rhs.index_;
}

bool WordDescriptor::operator<(WordDescriptor const &rhs) const noexcept
{
  this->assertComparableTo_(rhs);
  return index_ < rhs.index_;
}

bool WordDescriptor::operator>(WordDescriptor const &rhs) const noexcept
{
  this->assertComparableTo_(rhs);
  return index_ > rhs.index_;
}

bool WordDescriptor::operator<=(WordDescriptor const &rhs) const noexcept
{
  this->assertComparableTo_(rhs);
  return index_ <= rhs.index_;
}

bool WordDescriptor::operator>=(WordDescriptor const &rhs) const noexcept
{
  this->assertComparableTo_(rhs);
  return index_ >= rhs.index_;
}

void swap(WordDescriptor &lhs, WordDescriptor &rhs) noexcept
{
  lhs.swap(rhs);
}

} // namespace Enek

namespace std{

std::size_t
hash<Enek::WordDescriptor>::operator()(Enek::WordDescriptor const &k) const
{
  if (k.isInvalidated_()) {
    ENEK_THROW<std::invalid_argument>(
      "`std::hash<Enek::WordDescriptor>::operator()' is called on an "
      "invalidated word descriptor.");
  }
  return std::hash<Enek::WordIndex>()(k.index_);
}

} // namespace std

namespace Enek{

Enek::WordDescriptor Dictionary::getWordDescriptor_(
  Enek::WordIndex index, std::shared_ptr<Enek::WordIndex> const &p) noexcept
{
  return Enek::WordDescriptor(index, p);
}

#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

#if defined(ENEK_ENABLE_ASSERT_PARANOID)
Dictionary::WordIterator::WordIterator(
  Enek::WordIndex index, std::shared_ptr<Enek::WordIndex> const &p) noexcept
  : index_(index)
  , p_(p)
{
  if (p.use_count() > 0) {
    if (p != nullptr) {
      if (*p <= Enek::getMaxNumWordsInDictionary()) {
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
Dictionary::WordIterator::WordIterator(Enek::WordIndex index) noexcept
  : index_(index)
{
  ENEK_ASSERT(index_ <= Enek::getMaxNumWordsInDictionary());
}
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

#if defined(ENEK_ENABLE_ASSERT_PARANOID)
bool Dictionary::WordIterator::isWeakPointerEmpty_() const noexcept
{
  return !p_.owner_before(std::weak_ptr<int>())
    && !std::weak_ptr<int>().owner_before(p_);
}

bool Dictionary::WordIterator::isSingular_() const noexcept
{
  ENEK_ASSERT(
    (index_ == Enek::getInvalidWordIndex()) == this->isWeakPointerEmpty_());
  return index_ == Enek::getInvalidWordIndex();
}

bool Dictionary::WordIterator::isInvalidated_() const noexcept
{
  ENEK_ASSERT(
    (index_ == Enek::getInvalidWordIndex()) == this->isWeakPointerEmpty_());
  return index_ != Enek::getInvalidWordIndex() && p_.expired();
}
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)

Dictionary::WordIterator::WordIterator() noexcept
  : index_(Enek::getInvalidWordIndex())
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
  ENEK_ASSERT(index_ < Enek::getMaxNumWordsInDictionary());
  ++index_;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

Dictionary::Dictionary()
  : words_()
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  , p_(std::make_shared<Enek::WordIndex>(0))
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

Enek::WordIndex Dictionary::getNumWords() const noexcept
{
  return words_.size();
}

Enek::WordIndex Dictionary::getWordIndex(
  Enek::WordDescriptor const &w) const noexcept
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
  ENEK_ASSERT(w != Enek::getInvalidWordIndex());
  return w;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

} // namespace Enek
