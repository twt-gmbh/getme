/*
Unit tests for utility functions for exception handling and consistency

Copyright (C) 2023  TWT GmbH Science & Innovation.

This file is part of GETMe Polygonal Meshes 2D.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/
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
