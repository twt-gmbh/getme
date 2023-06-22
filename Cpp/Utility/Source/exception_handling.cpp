// Utility functions for exception handling and consistency checks.
#include "Utility/exception_handling.h"

#include "Utility/generic_exception.h"

#include <iostream>

[[noreturn]] void Utility::throwException(
    const std::string& what,
    const std::source_location& location) {
  std::cerr << "Exception [" << location.file_name() << ", "
            << location.function_name() << ", line " << location.line()
            << "]: " << what << "\n";
  throw GenericException(what, location);
}

void Utility::throwExceptionIfFalse(bool conditionResult,
                                    const std::string& what,
                                    const std::source_location& location) {
  if (!conditionResult) {
    throwException(what, location);
  }
}

void Utility::throwExceptionIfTrue(bool conditionResult,
                                   const std::string& what,
                                   const std::source_location& location) {
  if (conditionResult) {
    throwException(what, location);
  }
}
