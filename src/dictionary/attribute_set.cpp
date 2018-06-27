#include <enek/dictionary/attribute_set.hpp>
#include <enek/dictionary/attribute_descriptor.hpp>
#include <enek/dictionary/fundamental.hpp>
#include <enek/util/string_key.hpp>
#include <enek/util/exception_guard.hpp>
#include <enek/util/assert.hpp>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <enek/util/abort.hpp>
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <unordered_map>
#include <vector>
#include <string_view>
#include <string>
#include <memory>
#include <tuple>
#include <utility>


namespace Enek::Dictionary{

namespace Detail{

class AttributeSetImpl
{
private:
  using StringToIndexMap_
    = std::unordered_map<Enek::StringKey, Enek::Dictionary::AttributeIndex>;
  using IndexToStringMap_ = std::vector<std::string_view>;

public:
  AttributeSetImpl()
    : string_to_index_map_()
    , index_to_string_map_()
  {}

  AttributeSetImpl(AttributeSetImpl const &) = delete;

  AttributeSetImpl &operator=(AttributeSetImpl const &) = delete;

  Enek::Dictionary::AttributeIndex getSize() const noexcept
  {
    ENEK_ASSERT(string_to_index_map_.size() == index_to_string_map_.size());
    return string_to_index_map_.size();
  }

  Enek::Dictionary::AttributeIndex
  find(std::string_view const &string) const noexcept
  {
    Enek::StringKey const key(string);
    auto const found = string_to_index_map_.find(key);
    return found != string_to_index_map_.cend()
      ? found->second : Enek::Dictionary::getSingularAttributeIndex();
  }

  std::string_view getStringView(
    Enek::Dictionary::AttributeIndex index) const noexcept
  {
    ENEK_ASSERT(index < index_to_string_map_.size());
    return index_to_string_map_[index];
  }

  Enek::Dictionary::AttributeIndex add(std::string_view const &string_view)
  {
    Enek::StringKey const key_to_find(string_view);
    auto iter = string_to_index_map_.find(key_to_find);
    if (iter == string_to_index_map_.end()) {
      Enek::StringKey key_to_insert{ std::string(string_view) };
      Enek::Dictionary::AttributeIndex const
        index = string_to_index_map_.size();
      bool result = false;
      std::tie(iter, result)
        = string_to_index_map_.emplace(std::move(key_to_insert), index);
      ENEK_EXCEPTION_GUARD{
        string_to_index_map_.erase(iter);
      };
      ENEK_ASSERT(result);
      index_to_string_map_.push_back(iter->first.getStringView());
    }
    return iter->second;
  }

  Enek::Dictionary::AttributeIndex add(std::string &&string)
  {
    Enek::StringKey key(std::move(string));
    ENEK_EXCEPTION_GUARD{
      string = key.moveString();
    };
    Enek::Dictionary::AttributeIndex const
      index_candidate = string_to_index_map_.size();
    StringToIndexMap_::iterator iter;
    bool result = false;
    std::tie(iter, result)
      = string_to_index_map_.emplace(std::move(key), index_candidate);
    if (result) {
      ENEK_EXCEPTION_GUARD{
        key = std::move(iter->first);
        string_to_index_map_.erase(iter);
      };
      index_to_string_map_.push_back(iter->first.getStringView());
    }
    return iter->second;
  }

private:
  StringToIndexMap_ string_to_index_map_;
  IndexToStringMap_ index_to_string_map_;
}; // class AttributeSetImpl

namespace{

AttributeSetImpl &castToImpl(std::shared_ptr<void> const &p_impl) noexcept
{
  ENEK_ASSERT(p_impl != nullptr);
  return *static_cast<AttributeSetImpl *>(p_impl.get());
}

AttributeSetImpl const &
castToConstImpl(std::shared_ptr<void> const &p_impl) noexcept
{
  ENEK_ASSERT(p_impl != nullptr);
  return *static_cast<AttributeSetImpl const *>(p_impl.get());
}

} // namespace *unnamed*

} // namepsace Detail

AttributeSet::AttributeSet() noexcept = default;

AttributeSet::AttributeSet(AttributeSet &&rhs) noexcept = default;

Enek::Dictionary::AttributeIndex AttributeSet::getSize() const noexcept
{
  return p_impl_ != nullptr ? Detail::castToConstImpl(p_impl_).getSize() : 0;
}

Enek::Dictionary::AttributeDescriptor
AttributeSet::find(std::string_view const &string) const noexcept
{
  Enek::Dictionary::AttributeIndex const index
    = p_impl_ != nullptr
        ? Detail::castToConstImpl(p_impl_).find(string)
        : Enek::Dictionary::getSingularAttributeIndex();
  return this->getDescriptor_(index);
}

Enek::Dictionary::AttributeIndex AttributeSet::getIndex(
  Enek::Dictionary::AttributeDescriptor const &a) const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (a.getValidationPointer() == p_impl_) {
    ENEK_ASSERT(a.getIndex() < this->getSize());
    return a.getIndex();
  }
  ENEK_ABORT << "Attempt to get the index of an attribute descriptor from a "
                "different attribute set.";
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  ENEK_ASSERT(p_impl_ != nullptr);
  ENEK_ASSERT(a < this->getSize());
  return a;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

Enek::Dictionary::AttributeDescriptor
AttributeSet::add(std::string_view const &string_view)
{
  if (p_impl_ == nullptr) {
    p_impl_ = std::make_shared<Detail::AttributeSetImpl>();
  }
  Enek::Dictionary::AttributeIndex const index
    = Detail::castToImpl(p_impl_).add(string_view);
  return this->getDescriptor_(index);
}

Enek::Dictionary::AttributeDescriptor AttributeSet::add(std::string &&string)
{
  if (p_impl_ == nullptr) {
    p_impl_ = std::make_shared<Detail::AttributeSetImpl>();
  }
  Enek::Dictionary::AttributeIndex const index
    = Detail::castToImpl(p_impl_).add(std::move(string));
  return this->getDescriptor_(index);
}

Enek::Dictionary::AttributeDescriptor AttributeSet::getDescriptor_(
  Enek::Dictionary::AttributeIndex index) const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  return index != Enek::Dictionary::getSingularAttributeIndex()
    ? Enek::Dictionary::AttributeDescriptor(index, p_impl_)
    : Enek::Dictionary::AttributeDescriptor();
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  ENEK_ASSERT(p_impl_ != nullptr);
  ENEK_ASSERT(index < this->getSize());
  return index;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

} // namespace Enek::Dictionary
