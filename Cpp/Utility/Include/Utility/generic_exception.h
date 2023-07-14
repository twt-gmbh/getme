/*
Basic generic exception with source location information and what string.

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
