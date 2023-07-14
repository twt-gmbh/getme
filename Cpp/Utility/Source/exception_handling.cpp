/*
Utility functions for exception handling and consistency checks.

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
