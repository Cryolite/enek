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
  , p_()
  , column_index_(Enek::Dictionary::getSingularColumnIndex())
{}

AttributeDescriptor::AttributeDescriptor(
  Enek::Dictionary::AttributeIndex index,
  std::shared_ptr<Enek::Dictionary::AttributeIndex> const &p,
  Enek::Dictionary::ColumnIndex column_index) noexcept
  : index_(index)
  , p_(p)
  , column_index_(column_index)
{
  if (p.use_count() > 0) {
    if (p != nullptr) {
      if (*p <= Enek::Dictionary::getMaxNumAttributesInColumn()) {
        if (index < *p) {
          if (column_index < Enek::Dictionary::getMaxNumColumns()) {
            return;
          }
          ENEK_ABORT << "`column_index' is out-of-range.";
        }
        ENEK_ABORT << "`index' is out-of-range.";
      }
      ENEK_ABORT << "The number of attributes in a column exceeds the largest "
                    "possible value.";
    }
    ENEK_ABORT << "`p' is the null pointer value.";
  }
  ENEK_ABORT << "`p' is empty.";
}

bool AttributeDescriptor::isAttributeIndexSingular_() const noexcept
{
  return index_ == Enek::Dictionary::getSingularAttributeIndex();
}

bool AttributeDescriptor::isWeakPointerEmpty_() const noexcept
{
  return !p_.owner_before(std::weak_ptr<int>())
    && !std::weak_ptr<int>().owner_before(p_);
}

bool AttributeDescriptor::isColumnIndexSingular_() const noexcept
{
  return column_index_ == Enek::Dictionary::getSingularColumnIndex();
}

bool AttributeDescriptor::isSingular_() const noexcept
{
  ENEK_ASSERT(
    this->isAttributeIndexSingular_() == this->isWeakPointerEmpty_());
  ENEK_ASSERT(
    this->isAttributeIndexSingular_() == this->isColumnIndexSingular_());
  return index_ == Enek::Dictionary::getSingularAttributeIndex();
}

bool AttributeDescriptor::isInvalidated_() const noexcept
{
  ENEK_ASSERT(
    this->isAttributeIndexSingular_() == this->isWeakPointerEmpty_());
  ENEK_ASSERT(
    this->isAttributeIndexSingular_() == this->isColumnIndexSingular_());
  return !this->isAttributeIndexSingular_() && p_.expired();
}

AttributeDescriptor::AttributeDescriptor(
  AttributeDescriptor const &rhs) noexcept
  : index_(rhs.index_)
  , p_(rhs.p_)
  , column_index_(rhs.column_index_)
{
  if (rhs.isInvalidated_()) {
    ENEK_ABORT << "Attempt to copy from an invalidated attribute descriptor.";
  }
}

AttributeDescriptor::AttributeDescriptor(AttributeDescriptor &&rhs) noexcept
  : AttributeDescriptor()
{
  if (rhs.isInvalidated_()) {
    ENEK_ABORT << "Attempt to move from an invalidated attribute descriptor.";
  }
  this->swap(rhs);
}

void AttributeDescriptor::swap(AttributeDescriptor &rhs) noexcept
{
  using std::swap;
  swap(index_,        rhs.index_);
  swap(p_,            rhs.p_);
  swap(column_index_, rhs.column_index_);
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

void AttributeDescriptor::assertComparableTo_(
  AttributeDescriptor const &rhs) const noexcept
{
  if (auto const p = p_.lock()) {
    if (auto const q = rhs.p_.lock()) {
      if (p == q) {
        ENEK_ASSERT(index_ <= *p);
        ENEK_ASSERT(rhs.index_ <= *q);
        if (column_index_ == rhs.column_index_) {
          return;
        }
        ENEK_ABORT << "Attempt to compare attribute descriptors from "
                      "different columns of a dictionary.";
      }
      ENEK_ABORT << "Attempt to compare attribute descriptors from different "
                    "dictionaries.";
    }
    if (rhs.isSingular_()) {
      ENEK_ABORT << "Attempt to compare a valid attribute descriptor to a "
                    "singular attribute descriptor.";
    }
    ENEK_ASSERT(rhs.isInvalidated_());
    ENEK_ABORT << "Attempt to compare a valid attribute descriptor to an "
                  "invalidated attribute descriptor.";
  }
  if (this->isSingular_()) {
    if (!rhs.p_.expired()) {
      ENEK_ABORT << "Attempt to compare a singular attribute descriptor to a "
                    "valid attribute descriptor.";
    }
    if (rhs.isSingular_()) {
      return;
    }
    ENEK_ASSERT(rhs.isInvalidated_());
    ENEK_ABORT << "Attempt to compare a singular attribute descriptor to an "
                  "invalidated attribute descriptor.";
  }
  ENEK_ASSERT(this->isInvalidated_());
  if (!rhs.p_.expired()) {
    ENEK_ABORT << "Attempt to compare an invalidated attribute descriptor to "
                  "a valid attribute descriptor.";
  }
  if (rhs.isSingular_()) {
    ENEK_ABORT << "Attempt to compare an invalidated attribute descriptor to "
                  "a singular attribute descriptor.";
  }
  ENEK_ASSERT(rhs.isInvalidated_());
  ENEK_ABORT << "Attempt to compare invalidated attribute descriptores.";
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
  if (k.isInvalidated_()) {
    ENEK_THROW<std::invalid_argument>(
      "`std::hash<Enek::Dictionary::AttributeDescriptor>::operator()' is "
      "called on an invalidated attribute descriptor.");
  }
  return std::hash<Enek::Dictionary::AttributeIndex>()(k.index_);
}

} // namespace std

#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
