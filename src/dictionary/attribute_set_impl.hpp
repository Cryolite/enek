#if !defined(ENEK_SRC_DICTIONARY_ATTRIBUTE_SET_IMPL_HPP_INCLUDE_GUARD)
#define ENEK_SRC_DICTIONARY_ATTRIBUTE_SET_IMPL_HPP_INCLUDE_GUARD

#include <enek/dictionary/attribute_descriptor.hpp>
#include <enek/dictionary/fundamental.hpp>
#include <enek/util/string_key.hpp>
#include <unordered_map>
#include <vector>
#include <string_view>
#include <string>
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
#include <memory>
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)


namespace Enek::Dictionary::Detail{

class AttributeSetImpl
{
private:
  using StringToIndexMap_
    = std::unordered_map<Enek::StringKey, Enek::Dictionary::AttributeIndex>;
  using IndexToStringMap_ = std::vector<std::string_view>;

public:
  AttributeSetImpl();

  AttributeSetImpl(AttributeSetImpl const &) = delete;

  AttributeSetImpl &operator=(AttributeSetImpl const &) = delete;

  Enek::Dictionary::AttributeDescriptor
  add(std::string_view const &string_view);

  Enek::Dictionary::AttributeDescriptor add(std::string &&string);

  Enek::Dictionary::AttributeIndex getSize() const noexcept;

  Enek::Dictionary::AttributeDescriptor
  find(std::string_view const &string) const noexcept;

  Enek::Dictionary::AttributeIndex getIndex(
    Enek::Dictionary::AttributeDescriptor const &attribute) const noexcept;

  std::string_view getStringView(
    Enek::Dictionary::AttributeDescriptor const &attribute) const noexcept;

private:
  StringToIndexMap_ string_to_index_map_;
  IndexToStringMap_ index_to_string_map_;
#if defined(ENEK_ENABLE_ASSERT_PARANOID)
  std::shared_ptr<void const> validation_pointer_;
#endif // defined(ENEK_ENABLE_ASSERT_PARANOID)
}; // class AttributeSetImpl

} // namespace Enek::Dictionary::Detail

#endif // !defined(ENEK_SRC_DICTIONARY_ATTRIBUTE_SET_IMPL_HPP_INCLUDE_GUARD)
