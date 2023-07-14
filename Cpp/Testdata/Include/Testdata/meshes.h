/*
Test data functions providing sample meshes.

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

#include "Mathematics/polygon.h"
#include "Mathematics/vector2d.h"

#include <unordered_set>
#include <vector>

namespace Mesh {
class PolygonalMesh;
}

namespace Testdata {
std::vector<Mathematics::Vector2D> getMixedSampleMeshNodes();

std::vector<Mathematics::Polygon> getMixedSampleMeshPolygons();

std::unordered_set<std::size_t> getMixedSampleMeshFixedNodeIndices();

// Get a mixed sample mesh consisting of four triangles,
// two quadrilaterals (one regular), and one pentagon.
Mesh::PolygonalMesh getMixedSampleMesh();

// Generate invalid mesh by shifting node 9 of the mixed sample mesh to the
// right of the valid mesh bounding box, which invalidates elements 2 and 3.
Mesh::PolygonalMesh getInvalidMixedSampleMesh();
}  // namespace Testdata
