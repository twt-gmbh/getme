// Test data functions providing sample meshes.
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
