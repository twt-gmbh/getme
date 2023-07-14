/*
Common functions used by different smoothing algorithms.

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
#include "common_algorithms.h"

#include "Mathematics/polygon_algorithms.h"
#include "Mesh/mesh_quality.h"
#include "Mesh/polygonal_mesh.h"
#include "Mesh/polygonal_mesh_algorithms.h"
#include "Utility/exception_handling.h"

#include <set>

void Smoothing::applyEdgeLengthScaling(
    const Mathematics::Polygon& polygon,
    const std::vector<Mathematics::Vector2D>& originalMeshNodes,
    std::vector<Mathematics::Vector2D>& transformedElementNodes) {
  Mathematics::Vector2D commonPolygonCentroid(0.0, 0.0);
  double originalPolygonLength = 0.0;
  double transformedPolygonLength = 0.0;
  const auto& polygonNodeIndices = polygon.getNodeIndices();
  std::size_t previousMeshNodeIndex = polygonNodeIndices.back();
  std::size_t previousNodeIndex = polygonNodeIndices.size() - 1;
  for (std::size_t nodeNumber = 0; nodeNumber < polygonNodeIndices.size();
       ++nodeNumber) {
    const std::size_t meshNodeIndex = polygonNodeIndices.at(nodeNumber);
    commonPolygonCentroid += originalMeshNodes.at(meshNodeIndex);
    originalPolygonLength += (originalMeshNodes.at(meshNodeIndex)
                              - originalMeshNodes.at(previousMeshNodeIndex))
                                 .getLength();
    previousMeshNodeIndex = meshNodeIndex;
    transformedPolygonLength +=
        (transformedElementNodes.at(nodeNumber)
         - transformedElementNodes.at(previousNodeIndex))
            .getLength();
    previousNodeIndex = nodeNumber;
  }
  commonPolygonCentroid /= static_cast<double>(polygonNodeIndices.size());
  const double scalingFactor = originalPolygonLength / transformedPolygonLength;
  const double oneMinusScalingFactor = 1.0 - scalingFactor;
  for (std::size_t nodeNumber = 0; nodeNumber < polygonNodeIndices.size();
       ++nodeNumber) {
    transformedElementNodes.at(nodeNumber) =
        oneMinusScalingFactor * commonPolygonCentroid
        + scalingFactor * transformedElementNodes.at(nodeNumber);
  }
}

// Helper functions for upcoming implementation of algorithm
// iterativelyResetNodesResultingInInvalidElementsSetNewMeshNodesAndUpdateElementQualityNumbers.
namespace {
// Collect all nodes to be reset to original position stored in mesh.
std::unordered_set<std::size_t> getIndicesOfNodesToReset(
    const std::vector<double>& polygonMeanRatioValues,
    const Mesh::PolygonalMesh& mesh) {
  std::unordered_set<std::size_t> indicesOfNodesToReset;
  for (std::size_t polygonIndex = 0; polygonIndex < mesh.getNumberOfPolygons();
       ++polygonIndex) {
    if (polygonMeanRatioValues.at(polygonIndex) <= 0.0) {
      const auto& polygonNodeIndices =
          mesh.getPolygons().at(polygonIndex).getNodeIndices();
      indicesOfNodesToReset.insert(polygonNodeIndices.begin(),
                                   polygonNodeIndices.end());
    }
  }
  return indicesOfNodesToReset;
}

// Reset nodes and collect indices of all affected polygons.
std::unordered_set<std::size_t> resetNodesAndGetAffectedPolygonIndices(
    const std::unordered_set<std::size_t>& indicesOfNodesToReset,
    const Mesh::PolygonalMesh& mesh,
    std::vector<Mathematics::Vector2D>& newNodePositions) {
  std::unordered_set<std::size_t> indicesOfAffectedPolygons;
  for (const auto nodeIndex : indicesOfNodesToReset) {
    newNodePositions.at(nodeIndex) = mesh.getNodes().at(nodeIndex);
    for (const auto attachedPolygonIndex :
         mesh.getAttachedPolygonIndices(nodeIndex)) {
      indicesOfAffectedPolygons.insert(attachedPolygonIndex);
    }
  }
  return indicesOfAffectedPolygons;
}
}  // namespace

Mesh::MeshQuality Smoothing::
    iterativelyResetNodesResultingInInvalidElementsSetNewMeshNodesAndUpdateElementQualityNumbers(
        std::vector<Mathematics::Vector2D>& newNodePositions,
        std::vector<double>& polygonMeanRatioValues,
        Mesh::PolygonalMesh& mesh) {
  Mesh::computeMeanRatioQualityNumberOfPolygons(
      mesh.getPolygons(), newNodePositions, polygonMeanRatioValues);
  while (true) {
    const auto indicesOfNodesToReset =
        getIndicesOfNodesToReset(polygonMeanRatioValues, mesh);
    // Since the previous step mesh was valid, termination is guaranteed, since
    // worst case is reseting all elements.
    if (indicesOfNodesToReset.empty()) {
      break;
    }
    const auto indicesOfAffectedPolygons =
        resetNodesAndGetAffectedPolygonIndices(indicesOfNodesToReset, mesh,
                                               newNodePositions);
    // Recompute mean ratio numbers of affected polygons.
    for (const auto polygonIndex : indicesOfAffectedPolygons) {
      polygonMeanRatioValues.at(polygonIndex) = Mathematics::getMeanRatio(
          mesh.getPolygons().at(polygonIndex), newNodePositions);
    }
  }
  mesh.setNodes(newNodePositions);
  return Mesh::MeshQuality(polygonMeanRatioValues, false);
}

void Smoothing::checkTransformations(
    const std::size_t maxNumberOfPolygonNodes,
    const std::vector<Mathematics::GeneralizedPolygonTransformation>&
        transformations) {
  Utility::throwExceptionIfTrue(
      maxNumberOfPolygonNodes > transformations.size(),
      "Regularizing transformations must be given for all polygon types.");
  // Check only relevant transformations starting from index 3.
  for (std::size_t numberOfPolygonNodes = 3;
       numberOfPolygonNodes <= maxNumberOfPolygonNodes;
       ++numberOfPolygonNodes) {
    const auto numberOfPolygonsAsString = std::to_string(numberOfPolygonNodes);
    Utility::throwExceptionIfFalse(
        transformations.at(numberOfPolygonNodes)
            .isCounterclockwiseRegularizingTransformation(numberOfPolygonNodes),
        "Transformation for " + numberOfPolygonsAsString
            + "-gons is not regularizing.");
  }
}

void Smoothing::checkTransformations(
    const Mesh::PolygonalMesh& mesh,
    const std::vector<Mathematics::GeneralizedPolygonTransformation>&
        transformations) {
  checkTransformations(mesh.getMaximalNumberOfPolygonNodes(), transformations);
}
