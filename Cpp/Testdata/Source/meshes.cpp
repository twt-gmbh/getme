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
#include "Testdata/meshes.h"

#include "Mesh/polygonal_mesh.h"

std::vector<Mathematics::Vector2D> Testdata::getMixedSampleMeshNodes() {
  return std::vector<Mathematics::Vector2D>{
      {0.0, 0.0},   // 0
      {5.0, -1.0},  // 1
      {7.0, -2.0},  // 2
      {9.0, 0.0},   // 3
      {9.0, 2.0},   // 4
      {9.0, 5.0},   // 5
      {6.0, 5.0},   // 6
      {3.0, 5.0},   // 7
      {0.0, 3.0},   // 8
      {6.0, 2.0},   // 9
      {3.0, 1.0},   // 10
  };
}

std::vector<Mathematics::Polygon> Testdata::getMixedSampleMeshPolygons() {
  return std::vector<Mathematics::Polygon>{
      Mathematics::Polygon{{0, 1, 10}},       // 0
      Mathematics::Polygon{{1, 9, 10}},       // 1
      Mathematics::Polygon{{1, 2, 3, 4, 9}},  // 2
      Mathematics::Polygon{{4, 5, 6, 9}},     // 3
      Mathematics::Polygon{{9, 6, 10}},       // 4
      Mathematics::Polygon{{6, 7, 8, 10}},    // 5
      Mathematics::Polygon{{0, 10, 8}},       // 6
  };
}

std::unordered_set<std::size_t> Testdata::getMixedSampleMeshFixedNodeIndices() {
  return std::unordered_set<std::size_t>{0, 1, 2, 3, 4, 5, 6, 7, 8};
}

Mesh::PolygonalMesh Testdata::getMixedSampleMesh() {
  return Mesh::PolygonalMesh(getMixedSampleMeshNodes(),
                             getMixedSampleMeshPolygons(),
                             getMixedSampleMeshFixedNodeIndices());
}

Mesh::PolygonalMesh Testdata::getInvalidMixedSampleMesh() {
  auto invalidMesh = getMixedSampleMesh();
  invalidMesh.getMutableNodes().at(9) = {17.0, 2.0};
  return invalidMesh;
}
