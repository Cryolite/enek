#if !defined(ENEK_DICTIONARY_ATTRIBUTE_SET_HPP_INCLUDE_GUARD)
#define ENEK_DICTIONARY_ATTRIBUTE_SET_HPP_INCLUDE_GUARD

#include <enek/dictionary/attribute_descriptor.hpp>
#include <enek/dictionary/fundamental.hpp>
#include <string_view>
#include <string>
#include <memory>


namespace Enek::Dictionary{

class AttributeSet
{
public:
  AttributeSet() noexcept;

  AttributeSet(AttributeSet &&rhs) noexcept;

  AttributeSet &operator=(AttributeSet const &) = delete;

  Enek::Dictionary::AttributeIndex getSize() const noexcept;

  Enek::Dictionary::AttributeDescriptor
  find(std::string_view const &string) const noexcept;

  Enek::Dictionary::AttributeIndex getIndex(
    Enek::Dictionary::AttributeDescriptor const &a) const noexcept;

  std::string_view getStringView(
    Enek::Dictionary::AttributeDescriptor const &a) const noexcept;

  Enek::Dictionary::AttributeDescriptor
  add(std::string_view const &string_view);

  Enek::Dictionary::AttributeDescriptor add(std::string &&string);

private:
  Enek::Dictionary::AttributeDescriptor
  getDescriptor_(Enek::Dictionary::AttributeIndex index) const noexcept;

private:
  std::shared_ptr<void> p_impl_;
}; // class AttributeSet

} // namespace Enek::Dictionary

#endif // !defined(ENEK_DICTIONARY_ATTRIBUTE_SET_HPP_INCLUDE_GUARD)
