/*
Algorithms for planar polygons.

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

#include <vector>

namespace Mathematics {
class Polygon;
class Vector2D;

// Get nodes of a counter clockwise oriented regular polygon with centroid
// (0,0) and radius 1.0.
std::vector<Vector2D> getNodesOfRegularPolygon(const std::size_t numberOfNodes);

// Compute mean ratio quality number for the given polygon and nodes according
// to Equation 2.6 of the GETMe book. Returns a quality number in [0,1] for
// valid polygons and -1 for invalid polygons.
double getMeanRatio(const Polygon& polygon, const std::vector<Vector2D>& nodes);
}  // namespace Mathematics
