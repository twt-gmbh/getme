// Functions for command line reporting of smoothing results.
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
