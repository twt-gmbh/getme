/*
Algorithms for planar polygonal meshes.

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

#include <filesystem>
#include <vector>

namespace Mathematics {
class Polygon;
class Vector2D;
}  // namespace Mathematics

namespace Mesh {
class PolygonalMesh;
class MeshQuality;

void writeMeshFile(const PolygonalMesh& mesh,
                   const std::filesystem::path& outfilePath,
                   const bool includeMeanRatioQuality = true);

PolygonalMesh readMeshFile(const std::filesystem::path& infilePath);

// Assign mean ratio numbers of given polygons to given vector.
void computeMeanRatioQualityNumberOfPolygons(
    const std::vector<Mathematics::Polygon>& polygons,
    const std::vector<Mathematics::Vector2D>& nodes,
    std::vector<double>& meanRatioQualityNumbers);

// Compute mean ratio numbers of given polygons.
std::vector<double> computeMeanRatioQualityNumberOfPolygons(
    const std::vector<Mathematics::Polygon>& polygons,
    const std::vector<Mathematics::Vector2D>& nodes);

// Check if the given two meshes are equal with nodes being equal up to the
// given tolerance.
bool areEqual(const PolygonalMesh& first,
              const PolygonalMesh& second,
              const double nodesEqualTolerance = 0.0);

// Generate a distorted mesh by local random node movement of non fixed nodes.
// The maximal node movement distance is controlled by the maxDistortionRadius
// parameter. Distortion usually does not preserve mesh validity.
PolygonalMesh distortNodesLocally(const PolygonalMesh& mesh,
                                  const double maxDistortionRadius);
}  // namespace Mesh
