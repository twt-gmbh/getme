// Utility functions for exception handling and consistency checks.
#pragma once

#include <source_location>
#include <string>

namespace Utility {
[[noreturn]] void throwException(
    const std::string& what,
    const std::source_location& location = std::source_location::current());

void throwExceptionIfFalse(
    bool conditionResult,
    const std::string& what,
    const std::source_location& location = std::source_location::current());

void throwExceptionIfTrue(
    bool conditionResult,
    const std::string& what,
    const std::source_location& location = std::source_location::current());
}  // namespace Utility
