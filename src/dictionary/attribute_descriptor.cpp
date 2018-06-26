#include <enek/dictionary/attribute_descriptor.hpp>
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

AttributeDescriptor::AttributeDescriptor() noexcept
  : index_(Enek::Dictionary::getSingularAttributeIndex())
  , validation_pointer_()
{}

AttributeDescriptor::AttributeDescriptor(
  Enek::Dictionary::AttributeIndex index,
  std::shared_ptr<void const> const &validation_pointer) noexcept
  : index_(index)
  , validation_pointer_(validation_pointer)
{
  if (index < Enek::Dictionary::getMaxNumAttributesInColumn()) {
    if (validation_pointer.use_count() > 0) {
      if (validation_pointer != nullptr) {
        return;
      }
      ENEK_ABORT << "`validation_pointer' is the null pointer value.";
    }
    ENEK_ABORT << "`validation_pointer' is empty.";
  }
  ENEK_ABORT << "The number of attributes in a column exceeds the largest "
                "possible value.";
}

AttributeDescriptor::AttributeDescriptor(
  AttributeDescriptor const &rhs) noexcept
  : index_(rhs.index_)
  , validation_pointer_(rhs.validation_pointer_)
{
  if (rhs.isInvalidated()) {
    ENEK_ABORT << "Attempt to copy from an invalidated attribute descriptor.";
  }
}

AttributeDescriptor::AttributeDescriptor(AttributeDescriptor &&rhs) noexcept
  : AttributeDescriptor()
{
  if (rhs.isInvalidated()) {
    ENEK_ABORT << "Attempt to move from an invalidated attribute descriptor.";
  }
  this->swap(rhs);
}

void AttributeDescriptor::swap(AttributeDescriptor &rhs) noexcept
{
  using std::swap;
  swap(index_,              rhs.index_);
  swap(validation_pointer_, rhs.validation_pointer_);
}

AttributeDescriptor &
AttributeDescriptor::operator=(AttributeDescriptor const &rhs) noexcept
{
  AttributeDescriptor(rhs).swap(*this);
  return *this;
}

AttributeDescriptor &
AttributeDescriptor::operator=(AttributeDescriptor &&rhs) noexcept
{
  AttributeDescriptor(std::move(rhs)).swap(*this);
  return *this;
}

bool AttributeDescriptor::isIndexSingular_() const noexcept
{
  return index_ == Enek::Dictionary::getSingularAttributeIndex();
}

bool AttributeDescriptor::isValidationPointerEmpty_() const noexcept
{
  return !validation_pointer_.owner_before(std::weak_ptr<void const>())
    && !std::weak_ptr<void const>().owner_before(validation_pointer_);
}

bool AttributeDescriptor::isSingular() const noexcept
{
  ENEK_ASSERT(this->isIndexSingular_() == this->isValidationPointerEmpty_());
  return this->isIndexSingular_();
}

bool AttributeDescriptor::isInvalidated() const noexcept
{
  ENEK_ASSERT(this->isIndexSingular_() == this->isValidationPointerEmpty_());
  return !this->isIndexSingular_() && validation_pointer_.expired();
}

std::shared_ptr<void const>
AttributeDescriptor::getValidationPointer() const noexcept
{
  if (auto validation_pointer = validation_pointer_.lock()) {
    return validation_pointer;
  }
  if (this->isSingular()) {
    ENEK_ABORT << "Attempt to get the validation pointer of a singular "
                  "attribute descriptor.";
  }
  ENEK_ASSERT(this->isInvalidated());
  ENEK_ABORT << "Attempt to get the validation pointer of an invalidated "
                "attribute descriptor.";
}

Enek::Dictionary::AttributeIndex AttributeDescriptor::getIndex() const noexcept
{
  if (!validation_pointer_.expired()) {
    ENEK_ASSERT(index_ < Enek::Dictionary::getMaxNumAttributesInColumn());
    return index_;
  }
  if (this->isSingular()) {
    ENEK_ABORT << "Attempt to get the index of a singular attribute "
                  "descriptor.";
  }
  ENEK_ASSERT(this->isInvalidated());
  ENEK_ABORT << "Attempt to get the index of an invalidated attribute "
                "descriptor.";
}

void AttributeDescriptor::assertComparableTo_(
  AttributeDescriptor const &rhs) const noexcept
{
  if (auto const p = validation_pointer_.lock()) {
    if (auto const q = rhs.validation_pointer_.lock()) {
      if (p == q) {
        return;
      }
      ENEK_ABORT << "Attempt to compare attribute descriptors from different "
                    "attribute sets.";
    }
    if (rhs.isSingular()) {
      ENEK_ABORT << "Attempt to compare a valid attribute descriptor to a "
                    "singular attribute descriptor.";
    }
    ENEK_ASSERT(rhs.isInvalidated());
    ENEK_ABORT << "Attempt to compare a valid attribute descriptor to an "
                  "invalidated attribute descriptor.";
  }
  if (this->isSingular()) {
    if (!rhs.validation_pointer_.expired()) {
      ENEK_ABORT << "Attempt to compare a singular attribute descriptor to a "
                    "valid attribute descriptor.";
    }
    if (rhs.isSingular()) {
      return;
    }
    ENEK_ASSERT(rhs.isInvalidated());
    ENEK_ABORT << "Attempt to compare a singular attribute descriptor to an "
                  "invalidated attribute descriptor.";
  }
  ENEK_ASSERT(this->isInvalidated());
  if (!rhs.validation_pointer_.expired()) {
    ENEK_ABORT << "Attempt to compare an invalidated attribute descriptor to "
                  "a valid attribute descriptor.";
  }
  if (rhs.isSingular()) {
    ENEK_ABORT << "Attempt to compare an invalidated attribute descriptor to "
                  "a singular attribute descriptor.";
  }
  ENEK_ASSERT(rhs.isInvalidated());
  ENEK_ABORT << "Attempt to compare invalidated attribute descriptors.";
}

bool AttributeDescriptor::operator==(
  AttributeDescriptor const &rhs) const noexcept
{
  this->assertComparableTo_(rhs);
  return index_ == rhs.index_;
}

bool AttributeDescriptor::operator!=(
  AttributeDescriptor const &rhs) const noexcept
{
  this->assertComparableTo_(rhs);
  return index_ != rhs.index_;
}

bool AttributeDescriptor::operator<(
  AttributeDescriptor const &rhs) const noexcept
{
  this->assertComparableTo_(rhs);
  return index_ < rhs.index_;
}

bool AttributeDescriptor::operator>(
  AttributeDescriptor const &rhs) const noexcept
{
  this->assertComparableTo_(rhs);
  return index_ > rhs.index_;
}

bool AttributeDescriptor::operator<=(
  AttributeDescriptor const &rhs) const noexcept
{
  this->assertComparableTo_(rhs);
  return index_ <= rhs.index_;
}

bool AttributeDescriptor::operator>=(
  AttributeDescriptor const &rhs) const noexcept
{
  this->assertComparableTo_(rhs);
  return index_ >= rhs.index_;
}

void swap(AttributeDescriptor &lhs, AttributeDescriptor &rhs) noexcept
{
  lhs.swap(rhs);
}

} // namespace Enek::Dictionary

namespace std{

std::size_t hash<Enek::Dictionary::AttributeDescriptor>::operator()(
  Enek::Dictionary::AttributeDescriptor const &k) const
{
  if (k.isSingular()) {
    ENEK_THROW<std::invalid_argument>(
      "`std::hash<Enek::Dictionary::AttributeDescriptor>::operator()' is "
      "called on a singular attribute descriptor.");
  }
  if (k.isInvalidated()) {
    ENEK_THROW<std::invalid_argument>(
      "`std::hash<Enek::Dictionary::AttributeDescriptor>::operator()' is "
      "called on an invalidated attribute descriptor.");
  }
  Enek::Dictionary::AttributeIndex const index = k.getIndex();
  std::hash<Enek::Dictionary::AttributeIndex> const hasher;
  return hasher(index);
}

} // namespace std

#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
