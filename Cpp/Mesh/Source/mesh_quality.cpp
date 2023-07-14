/*
Mesh quality information class.

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
#include "Mesh/mesh_quality.h"

#include "Mesh/polygonal_mesh.h"
#include "Mesh/polygonal_mesh_algorithms.h"

#include <optional>
#include <tuple>

namespace {
std::tuple<double, double> computeQMinAndQMeanTerminateIfInvalid(
    const std::vector<double>& polygonMeanRatioQualityNumbers) {
  double qMin = std::numeric_limits<double>::infinity();
  double sumOfMeanRatioNumbers = 0.0;
  for (const auto meanRatioNumber : polygonMeanRatioQualityNumbers) {
    if (meanRatioNumber <= 0.0) {
      return std::make_tuple(-1.0, -1.0);
    }
    if (meanRatioNumber < qMin) {
      qMin = meanRatioNumber;
    }
    sumOfMeanRatioNumbers += meanRatioNumber;
  }
  return std::make_tuple(
      qMin, sumOfMeanRatioNumbers
                / static_cast<double>(polygonMeanRatioQualityNumbers.size()));
}

// Compute all quality numbers except q_min* if mesh is not given.
// Do not terminate computation preliminary.
std::tuple<double, std::optional<double>, double, std::size_t>
computeQMinQMinStarQMeanAndNumberOfInvalid(
    const std::vector<double>& polygonMeanRatioQualityNumbers,
    const Mesh::PolygonalMesh* mesh = nullptr) {
  double qMin = std::numeric_limits<double>::infinity();
  double qMinStar = std::numeric_limits<double>::infinity();
  std::optional<double> qMinStarOptional;
  double sumOfMeanRatioNumbers = 0.0;
  std::size_t numberOfInvalid = 0;
  for (std::size_t polygonIndex = 0;
       polygonIndex < polygonMeanRatioQualityNumbers.size(); ++polygonIndex) {
    const double meanRatioNumber =
        polygonMeanRatioQualityNumbers.at(polygonIndex);
    if (meanRatioNumber <= 0.0) {
      ++numberOfInvalid;
    }
    sumOfMeanRatioNumbers += meanRatioNumber;
    if (meanRatioNumber < qMin) {
      qMin = meanRatioNumber;
    }
    if (mesh && !mesh->isFixedPolygon(polygonIndex)
        && meanRatioNumber < qMinStar) {
      qMinStar = meanRatioNumber;
    }
  }
  if (numberOfInvalid > 0) {
    return std::make_tuple(-1.0, qMinStarOptional, -1.0, numberOfInvalid);
  }
  if (mesh && qMinStar <= 1.0) {
    qMinStarOptional = qMinStar;
  }
  return std::make_tuple(
      qMin, qMinStarOptional,
      sumOfMeanRatioNumbers
          / static_cast<double>(polygonMeanRatioQualityNumbers.size()),
      std::size_t(0));
}
}  // namespace

namespace Mesh {
MeshQuality::MeshQuality(const PolygonalMesh& mesh) {
  const auto elementMeanRatioNumbers = computeMeanRatioQualityNumberOfPolygons(
      mesh.getPolygons(), mesh.getNodes());
  std::tie(qMin, qMinStar, qMean, numberOfInvalidElements) =
      computeQMinQMinStarQMeanAndNumberOfInvalid(elementMeanRatioNumbers,
                                                 &mesh);
}

MeshQuality::MeshQuality(const std::vector<double>& elementMeanRatioNumbers,
                         const bool determineNumberOfInvalidElements) {
  if (determineNumberOfInvalidElements) {
    std::tie(qMin, qMinStar, qMean, numberOfInvalidElements) =
        computeQMinQMinStarQMeanAndNumberOfInvalid(elementMeanRatioNumbers);
  } else {
    std::tie(qMin, qMean) =
        computeQMinAndQMeanTerminateIfInvalid(elementMeanRatioNumbers);
  }
}
}  // namespace Mesh
