/*
Base class for a mesh smoothing results not incorporating iterations

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
// information.
#pragma once

#include "Mesh/mesh_quality.h"
#include "Mesh/polygonal_mesh.h"

#include <string>

namespace Smoothing {
struct SmoothingResultBase {
  explicit SmoothingResultBase(const std::string& algorithmName,
                               const Mesh::PolygonalMesh& mesh,
                               const double smoothingWallClockTimeInSeconds)
    : algorithmName(algorithmName)
    , mesh(mesh)
    , meshQuality(mesh)
    , smoothingWallClockTimeInSeconds(smoothingWallClockTimeInSeconds) {}

  const std::string algorithmName;
  const Mesh::PolygonalMesh mesh;
  const Mesh::MeshQuality meshQuality;
  const double smoothingWallClockTimeInSeconds;
};
}  // namespace Smoothing
