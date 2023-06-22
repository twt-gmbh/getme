// Basic generic exception with source location information and what string.
#pragma once

#include <source_location>
#include <string>

namespace Utility {
class GenericException final : public std::exception {
public:
  GenericException(const std::string& what,
                   const std::source_location& location)
    : what(what), location(location) {}

  const std::string& getWhat() const { return what; }

  const std::source_location& getSourceLocation() const { return location; }

private:
  std::string what;
  std::source_location location;
};
}  // namespace Utility
