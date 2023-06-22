// Base class for a mesh smoothing results not incorporating iterations
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
