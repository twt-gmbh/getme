// Unit tests for utility functions for exception handling and consistency
// checks.
#include "Utility/exception_handling.h"

#include "Utility/generic_exception.h"

#include "gtest/gtest.h"

#include <source_location>
#include <string>

TEST(ExceptionHandling, throwException) {
  const std::string what{"throwException test"};
  const auto sourceLocation = std::source_location::current();

  try {
    Utility::throwException(what, sourceLocation);
    FAIL();
  } catch (const Utility::GenericException& exception) {
    EXPECT_EQ(what, exception.getWhat());
    EXPECT_EQ(sourceLocation.file_name(),
              exception.getSourceLocation().file_name());
    EXPECT_EQ(sourceLocation.function_name(),
              exception.getSourceLocation().function_name());
    EXPECT_EQ(sourceLocation.line(), exception.getSourceLocation().line());
  } catch (...) {
    FAIL();
  }
}

TEST(ExceptionHandling, throwExceptionIfFalse) {
  const std::string what{"throwExceptionIfFalse test"};

  EXPECT_THROW(Utility::throwExceptionIfFalse(5 < 3, what),
               Utility::GenericException);
  EXPECT_NO_THROW(Utility::throwExceptionIfFalse(3 < 5, what));
}

TEST(ExceptionHandling, throwExceptionIfTrue) {
  const std::string what{"throwExceptionIfTrue test"};

  EXPECT_THROW(Utility::throwExceptionIfTrue(3 < 5, what),
               Utility::GenericException);
  EXPECT_NO_THROW(Utility::throwExceptionIfTrue(5 < 3, what));
}
