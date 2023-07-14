/*
Variants of Laplacian smoothing algorithms.

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

namespace Mesh {
class PolygonalMesh;
}

namespace Smoothing {
struct BasicLaplaceConfig;
struct SmartLaplaceConfig;
struct SmoothingResult;

// Classic Laplacian smoothing by iterative connected node averaging.
// Node updates are computed simultaneously and set afterwards. Thus the order
// of nodes has no impact on the smoothing results.
// Cf. Section 4.2.1 of the GETMe book.
SmoothingResult basicLaplace(Mesh::PolygonalMesh mesh,
                             const BasicLaplaceConfig& config);

// Smart Laplacian smoothing updating nodes only if this increases
// the arithmetic mean of the mean ratio quality numbers of attached elements.
// Node updates are computed simultaneously and set afterwards. Thus the order
// of nodes has no impact on the smoothing results.
// Cf. Section 4.2.1 of the GETMe book.
SmoothingResult smartLaplace(Mesh::PolygonalMesh mesh,
                             const SmartLaplaceConfig& config);
}  // namespace Smoothing
