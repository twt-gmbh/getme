/*
Algorithm class for GETMe sequential smoothing according to Section 6.1.3 of

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
// the GETMe book.
#pragma once

#include "Mesh/polygonal_mesh.h"
#include "Smoothing/getme_sequential_config.h"
#include "Smoothing/smoothing_result.h"
#include "polygon_quality_min_heap.h"

namespace Smoothing {
class GetmeSequential final {
public:
  GetmeSequential(const Mesh::PolygonalMesh& mesh,
                  const GetmeSequentialConfig& config);

  SmoothingResult getResult() const;

private:
  void checkInputData() const;
  void initHelperData();
  void applySmoothing();

  struct LocalQualityResult final {
    bool areAllElementsValid = false;
    double transformedElementMeanRatioNumber = -1.0;
    std::vector<std::pair<std::size_t, double>>
        neighborElementIndexAndMeanRatioNumber;
  };

  void transformPolygonAndSetTemporaryNodes(
      const Mathematics::Polygon& polygon);
  LocalQualityResult assessLocalQuality(
      const std::size_t transformedPolygonIndex) const;
  void copyNodes(const std::size_t polygonIndex,
                 const std::vector<Mathematics::Vector2D>& sourceNodes,
                 std::vector<Mathematics::Vector2D>& targetNodes) const;

  // Input data and result in the case of mesh.
  Mesh::PolygonalMesh mesh;
  const GetmeSequentialConfig& config;

  // Helper data.
  PolygonQualityMinHeap minHeap;
  std::vector<bool> isNodeFixed;
  std::vector<Mathematics::Vector2D> temporaryNodes;

  // Result data.
  double smoothingTimeInSeconds = 0.0;
  std::size_t iterationsApplied = 0;
};
}  // namespace Smoothing
