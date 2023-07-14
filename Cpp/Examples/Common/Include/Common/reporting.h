/*
Functions for command line reporting of smoothing results.

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
#include <string>

namespace Mesh {
class MeshQuality;
class PolygonalMesh;
}  // namespace Mesh

namespace Smoothing {
class GetmeResult;
class SmoothingResult;
}  // namespace Smoothing

namespace Common {
void printInitialMeshInformation(const Mesh::PolygonalMesh& mesh);

void printSmoothingResult(const Smoothing::SmoothingResult& result);

void printSmoothingResult(const Smoothing::GetmeResult& result);

void writeResultMesh(const Mesh::PolygonalMesh& mesh,
                     const std::filesystem::path& initialMeshPath,
                     const std::string& meshName);
}  // namespace Common
