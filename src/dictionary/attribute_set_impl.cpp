#include "attribute_set_impl.hpp"
#include <enek/dictionary/attribute_descriptor.hpp>
#include <enek/dictionary/fundamental.hpp>
#include <enek/util/string_key.hpp>
#include <enek/util/exception_guard.hpp>
#include <enek/util/assert.hpp>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <enek/util/abort.hpp>
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <string_view>
#include <string>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <memory>
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <tuple>
#include <utility>


namespace Enek::Dictionary::Detail{

AttributeSetImpl::AttributeSetImpl()
  : string_to_index_map_()
  , index_to_string_map_()
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  , validation_pointer_(std::make_shared<int>(0))
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
{}

Enek::Dictionary::AttributeDescriptor
AttributeSetImpl::add(std::string_view const &string_view)
{
  Enek::StringKey const key_to_find(string_view);
  auto iter = string_to_index_map_.find(key_to_find);
  if (iter == string_to_index_map_.end()) {
    Enek::StringKey key_to_insert{ std::string(string_view) };
    Enek::Dictionary::AttributeIndex const index = string_to_index_map_.size();
    bool result = false;
    std::tie(iter, result)
      = string_to_index_map_.emplace(std::move(key_to_insert), index);
    ENEK_EXCEPTION_GUARD{
      string_to_index_map_.erase(iter);
    };
    ENEK_ASSERT(result);
    index_to_string_map_.push_back(iter->first.getStringView());
  }
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  return { iter->second, validation_pointer_ };
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  return iter->second;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

Enek::Dictionary::AttributeDescriptor
AttributeSetImpl::add(std::string &&string)
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
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  return { iter->second, validation_pointer_ };
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  return iter->second;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

Enek::Dictionary::AttributeIndex AttributeSetImpl::getSize() const noexcept
{
  ENEK_ASSERT(string_to_index_map_.size() == index_to_string_map_.size());
  return string_to_index_map_.size();
}

Enek::Dictionary::AttributeDescriptor
AttributeSetImpl::find(std::string_view const &string) const noexcept
{
  Enek::StringKey const key(string);
  auto const found = string_to_index_map_.find(key);
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  return found != string_to_index_map_.cend()
    ? Enek::Dictionary::AttributeDescriptor(found->second, validation_pointer_)
    : Enek::Dictionary::AttributeDescriptor();
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  return found != string_to_index_map_.cend()
    ? found->second : Enek::Dictionary::getSingularAttributeIndex();
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

Enek::Dictionary::AttributeIndex AttributeSetImpl::getIndex(
  Enek::Dictionary::AttributeDescriptor const &attribute) const noexcept
{
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  if (auto const p = attribute.getValidationPointer()) {
    if (p == validation_pointer_) {
      ENEK_ASSERT(attribute.getIndex() < this->getSize());
      return attribute.getIndex();
    }
    ENEK_ABORT << "Attempt to get the index of an attribute descriptor from a "
                  "different attribute set.";
  }
  if (attribute.isSingular()) {
    ENEK_ABORT << "Attempt to get the index of a singular attribute "
                  "descriptor.";
  }
  ENEK_ASSERT(attribute.isInvalidated());
  ENEK_ABORT << "Attempt to get the index of an invalidated attribute "
                "descriptor.";
#else // defined(ENEK_ENABLE_ASSERT_PARANOID)
  ENEK_ASSERT(attribute < this->getSize());
  return attribute;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}

std::string_view AttributeSetImpl::getStringView(
  Enek::Dictionary::AttributeDescriptor const &attribute) const noexcept
{
  Enek::Dictionary::AttributeIndex const index = this->getIndex(attribute);
  ENEK_ASSERT(index < index_to_string_map_.size());
  return index_to_string_map_[index];
}

} // namespace Enek::Dictionary::Detail
