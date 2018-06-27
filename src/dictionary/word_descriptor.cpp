#include <enek/dictionary/word_descriptor.hpp>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <enek/dictionary/fundamental.hpp>
#include <enek/util/throw.hpp>
#include <enek/util/assert.hpp>
#include <enek/util/abort.hpp>
#include <functional>
#include <memory>
#include <utility>
#include <stdexcept>
#include <cstddef>


namespace Enek::Dictionary{

WordDescriptor::WordDescriptor() noexcept
  : index_(Enek::Dictionary::getSingularWordIndex())
  , validation_pointer_()
{}

WordDescriptor::WordDescriptor(
  Enek::Dictionary::WordIndex index,
  std::shared_ptr<void const> const &validation_pointer) noexcept
  : index_(index)
  , validation_pointer_(validation_pointer)
{
  if (index < Enek::Dictionary::getMaxNumWordsInDictionary()) {
    if (validation_pointer.use_count() > 0) {
      if (validation_pointer != nullptr) {
        return;
      }
      ENEK_ABORT << "`validation_pointer' is the null pointer value.";
    }
    ENEK_ABORT << "`validation_pointer' is empty.";
  }
  ENEK_ABORT << "The number of words in a dictionary exceeds the largest "
                "possible value.";
}

WordDescriptor::WordDescriptor(WordDescriptor const &rhs) noexcept
  : index_(rhs.index_)
  , validation_pointer_(rhs.validation_pointer_)
{
  if (rhs.isInvalidated()) {
    ENEK_ABORT << "Attempt to copy from an invalidated word descriptor.";
  }
}

WordDescriptor::WordDescriptor(WordDescriptor &&rhs) noexcept
  : WordDescriptor()
{
  if (rhs.isInvalidated()) {
    ENEK_ABORT << "Attempt to move from an invalidated word descriptor.";
  }
  this->swap(rhs);
}

void WordDescriptor::swap(WordDescriptor &rhs) noexcept
{
  using std::swap;
  swap(index_,              rhs.index_);
  swap(validation_pointer_, rhs.validation_pointer_);
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

bool WordDescriptor::isIndexSingular_() const noexcept
{
  return index_ == Enek::Dictionary::getSingularWordIndex();
}

bool WordDescriptor::isValidationPointerEmpty_() const noexcept
{
  return !validation_pointer_.owner_before(std::weak_ptr<void const>())
    && !std::weak_ptr<void const>().owner_before(validation_pointer_);
}

bool WordDescriptor::isSingular() const noexcept
{
  ENEK_ASSERT(this->isIndexSingular_() == this->isValidationPointerEmpty_());
  return this->isIndexSingular_();
}

bool WordDescriptor::isInvalidated() const noexcept
{
  ENEK_ASSERT(this->isIndexSingular_() == this->isValidationPointerEmpty_());
  return !this->isIndexSingular_() && validation_pointer_.expired();
}

std::shared_ptr<void const>
WordDescriptor::getValidationPointer() const noexcept
{
  if (auto p = validation_pointer_.lock()) {
    return p;
  }
  if (this->isSingular()) {
    ENEK_ABORT << "Attempt to get the validation pointer of a singular word "
                  "descriptor.";
  }
  ENEK_ASSERT(this->isInvalidated());
  ENEK_ABORT << "Attempt to get the validation pointer of an invalidated word "
                "descriptor.";
}

Enek::Dictionary::WordIndex WordDescriptor::getIndex() const noexcept
{
  if (!validation_pointer_.expired()) {
    ENEK_ASSERT(index_ < Enek::Dictionary::getMaxNumWordsInDictionary());
    return index_;
  }
  if (this->isSingular()) {
    ENEK_ABORT << "Attempt to get the index of a singular word descriptor.";
  }
  ENEK_ASSERT(this->isInvalidated());
  ENEK_ABORT << "Attempt to get the index of an invalidated word descriptor.";
}

void WordDescriptor::assertComparableTo_(
  WordDescriptor const &rhs) const noexcept
{
  if (auto const p = validation_pointer_.lock()) {
    if (auto const q = rhs.validation_pointer_.lock()) {
      if (p == q) {
        return;
      }
      ENEK_ABORT << "Attempt to compare word descriptors from different "
                    "dictionaries.";
    }
    if (rhs.isSingular()) {
      ENEK_ABORT << "Attempt to compare a valid word descriptor to a "
                    "singular word descriptor.";
    }
    ENEK_ASSERT(rhs.isInvalidated());
    ENEK_ABORT << "Attempt to compare a valid word descriptor to an "
                  "invalidated word descriptor.";
  }
  if (this->isSingular()) {
    if (!rhs.validation_pointer_.expired()) {
      ENEK_ABORT << "Attempt to compare a singular word descriptor to a valid "
                    "word descriptor.";
    }
    if (rhs.isSingular()) {
      return;
    }
    ENEK_ASSERT(rhs.isInvalidated());
    ENEK_ABORT << "Attempt to compare a singular word descriptor to an "
                  "invalidated word descriptor.";
  }
  ENEK_ASSERT(this->isInvalidated());
  if (!rhs.validation_pointer_.expired()) {
    ENEK_ABORT << "Attempt to compare an invalidated word descriptor to a "
                  "valid word descriptor.";
  }
  if (rhs.isSingular()) {
    ENEK_ABORT << "Attempt to compare an invalidated word descriptor to a "
                  "singular word descriptor.";
  }
  ENEK_ASSERT(rhs.isInvalidated());
  ENEK_ABORT << "Attempt to compare invalidated word descriptors.";
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

} // namespace Enek::Dictionary

namespace std{

std::size_t hash<Enek::Dictionary::WordDescriptor>::operator()(
  Enek::Dictionary::WordDescriptor const &k) const
{
  if (k.isSingular()) {
    ENEK_THROW<std::invalid_argument>(
      "`std::hash<Enek::Dictionary::WordDescriptor>::operator()' is called on "
      "a singular word descriptor.");
  }
  if (k.isInvalidated()) {
    ENEK_THROW<std::invalid_argument>(
      "`std::hash<Enek::Dictionary::WordDescriptor>::operator()' is called on "
      "an invalidated word descriptor.");
  }
  Enek::Dictionary::WordIndex const index = k.getIndex();
  std::hash<Enek::Dictionary::WordIndex> const hasher;
  return hasher(index);
}

} // namespace std

#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
