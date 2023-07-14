/*
Utility functions for mathematics tests.

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

#include <complex>
#include <vector>

namespace Mathematics {
class Polygon;
class Vector2D;
}  // namespace Mathematics

namespace MathematicsTestUtilities {
std::vector<Mathematics::Vector2D> getSampleNodes();

// Get polygon with node indices 0 to numberOfNodes-1.
Mathematics::Polygon getPolygon(const std::size_t numberOfNodes);

std::vector<Mathematics::Vector2D> complexToVector2DNodes(
    const std::vector<std::complex<double>>& complexNodes);

std::vector<std::complex<double>> vector2DToComplexNodes(
    const std::vector<Mathematics::Vector2D>& vector2DNodes);
}  // namespace MathematicsTestUtilities
