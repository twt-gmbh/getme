// Algorithms for planar polygonal meshes.
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
