/*
Variants of GETMe smoothing algorithms.

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
struct BasicGetmeSimultaneousConfig;
struct GetmeConfig;
struct GetmeResult;
struct GetmeSequentialConfig;
struct GetmeSimultaneousConfig;
struct SmoothingResult;

// Basic GETMe simultaneous smoothing according to Section 6.1.1
// of the GETMe book.
SmoothingResult basicGetmeSimultaneous(
    Mesh::PolygonalMesh mesh,
    const BasicGetmeSimultaneousConfig& config);

// GETMe simultaneous smoothing according to Section 6.1.2 of the GETMe book.
SmoothingResult getmeSimultaneous(Mesh::PolygonalMesh mesh,
                                  const GetmeSimultaneousConfig& config);

// GETMe sequential smoothing according to Section 6.1.3 of the GETMe book.
SmoothingResult getmeSequential(const Mesh::PolygonalMesh& mesh,
                                const GetmeSequentialConfig& config);

// GETMe sequential smoothing according to Section 6.2.1 of the GETMe book.
GetmeResult getme(const Mesh::PolygonalMesh& mesh, const GetmeConfig& config);
}  // namespace Smoothing
