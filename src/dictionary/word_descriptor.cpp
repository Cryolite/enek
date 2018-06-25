#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <enek/dictionary/word_descriptor.hpp>
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
  , p_()
{}

WordDescriptor::WordDescriptor(
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
        ENEK_ABORT << "`index' is out-of-range.";
      }
      ENEK_ABORT << "The number of words in a dictionary exceeds the largest "
                    "possible value.";
    }
    ENEK_ABORT << "`p' is the null pointer value.";
  }
  ENEK_ABORT << "`p' is empty.";
}

bool WordDescriptor::isIndexSingular_() const noexcept
{
  return index_ == Enek::Dictionary::getSingularWordIndex();
}

bool WordDescriptor::isWeakPointerEmpty_() const noexcept
{
  return !p_.owner_before(std::weak_ptr<int>())
    && !std::weak_ptr<int>().owner_before(p_);
}

bool WordDescriptor::isSingular_() const noexcept
{
  ENEK_ASSERT(this->isIndexSingular_() == this->isWeakPointerEmpty_());
  return this->isIndexSingular_();
}

bool WordDescriptor::isInvalidated_() const noexcept
{
  ENEK_ASSERT(this->isIndexSingular_() == this->isWeakPointerEmpty_());
  return !this->isIndexSingular_() && p_.expired();
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

} // namespace Enek::Dictionary

namespace std{

std::size_t hash<Enek::Dictionary::WordDescriptor>::operator()(
  Enek::Dictionary::WordDescriptor const &k) const
{
  if (k.isInvalidated_()) {
    ENEK_THROW<std::invalid_argument>(
      "`std::hash<Enek::WordDescriptor>::operator()' is called on an "
      "invalidated word descriptor.");
  }
  return std::hash<Enek::Dictionary::WordIndex>()(k.index_);
}

} // namespace std

#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
