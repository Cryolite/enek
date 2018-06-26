#include <enek/util/string_key.hpp>
#include <enek/util/abort.hpp>
#include <string_view>
#include <string>
#include <functional>
#include <variant>
#include <utility>
#include <cstddef>


namespace Enek{

StringKey::StringKey() noexcept
  : impl_()
{}

StringKey::StringKey(std::string const &string)
  : impl_(string)
{}

StringKey::StringKey(std::string &&string) noexcept
  : impl_(std::move(string))
{}

StringKey::StringKey(std::string_view const &string_view) noexcept
  : impl_(string_view)
{}

StringKey::StringKey(StringKey const &rhs)
  : impl_(rhs.impl_)
{}

StringKey::StringKey(StringKey &&rhs) noexcept
  : impl_(std::move(rhs.impl_))
{}

void StringKey::swap(StringKey &rhs) noexcept
{
  using std::swap;
  swap(impl_, rhs.impl_);
}

StringKey &StringKey::operator=(StringKey const &rhs)
{
  StringKey(rhs).swap(*this);
  return *this;
}

StringKey &StringKey::operator=(StringKey &&rhs) noexcept
{
  StringKey(std::move(rhs)).swap(*this);
  return *this;
}

namespace{

class MoveStringVisitor
{
public:
  constexpr MoveStringVisitor() = default;

  MoveStringVisitor(MoveStringVisitor const &) = delete;

  MoveStringVisitor &operator=(MoveStringVisitor const &) = delete;

  std::string operator()(std::string &string) const noexcept
  {
    return std::move(string);
  }

  std::string operator()(std::string_view const &) const noexcept
  {
    ENEK_ABORT << "Attempt to move an object of type `std::string' from an "
                  "object of type `Enek::StringKey' that does not hold an "
                  "object of type `std::string'.";
    return {};
  }
}; // class MoveStringVisitor

} // namespace *unnamed*

std::string StringKey::moveString() noexcept
{
  return std::visit(MoveStringVisitor(), impl_);
}

std::string_view StringKey::getStringView() const noexcept
{
  return std::visit(
    [](std::string_view const &sv) noexcept { return sv; }, impl_);
}

bool StringKey::operator==(StringKey const &rhs) const noexcept
{
  return this->getStringView() == rhs.getStringView();
}

bool StringKey::operator!=(StringKey const &rhs) const noexcept
{
  return this->getStringView() != rhs.getStringView();
}

bool StringKey::operator<(StringKey const &rhs) const noexcept
{
  return this->getStringView() < rhs.getStringView();
}

bool StringKey::operator>(StringKey const &rhs) const noexcept
{
  return this->getStringView() > rhs.getStringView();
}

bool StringKey::operator<=(StringKey const &rhs) const noexcept
{
  return this->getStringView() <= rhs.getStringView();
}

bool StringKey::operator>=(StringKey const &rhs) const noexcept
{
  return this->getStringView() >= rhs.getStringView();
}

void swap(StringKey &lhs, StringKey &rhs) noexcept
{
  lhs.swap(rhs);
}

} // namespace Enek

namespace std{

std::size_t hash<Enek::StringKey>::operator()(
  Enek::StringKey const &key) const noexcept
{
  std::string_view const sv = key.getStringView();
  std::hash<std::string_view> const hasher;
  return hasher(sv);
}

} // namespace std
