/*
Variants of Laplacian smoothing algorithms.

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
#include "Smoothing/laplace_algorithms.h"

#include "Mathematics/vector2d.h"
#include "Mesh/mesh_quality.h"
#include "Mesh/polygonal_mesh.h"
#include "Mesh/polygonal_mesh_algorithms.h"
#include "Smoothing/basic_laplace_config.h"
#include "Smoothing/smart_laplace_config.h"
#include "Smoothing/smoothing_result.h"
#include "Utility/exception_handling.h"
#include "Utility/stop_watch.h"
#include "common_algorithms.h"

#include <algorithm>
#include <cmath>
#include <execution>

namespace {
Mathematics::Vector2D computeArithmeticMeanOfEdgeConnectedNodes(
    const Mesh::PolygonalMesh& mesh,
    const std::size_t nodeIndex) {
  Mathematics::Vector2D newNodePosition(0.0, 0.0);
  const auto& currentNodes = mesh.getNodes();
  const auto& indicesOfEdgeConnectedNodes =
      mesh.getIndicesOfEdgeConnectedNodes(nodeIndex);
  for (const auto connectedNodeIndex : indicesOfEdgeConnectedNodes) {
    newNodePosition += currentNodes.at(connectedNodeIndex);
  }
  newNodePosition /= static_cast<double>(indicesOfEdgeConnectedNodes.size());
  return newNodePosition;
}
}  // namespace

Smoothing::SmoothingResult Smoothing::basicLaplace(
    Mesh::PolygonalMesh mesh,
    const BasicLaplaceConfig& config) {
  std::size_t iteration = 0;

  Utility::StopWatch stopWatch;
  while (true) {
    ++iteration;
    double maxSquaredNodeRelocationDistance = 0.0;
    auto newNodePositions = mesh.getNodes();
    for (const auto nodeIndex : mesh.getNonFixedNodeIndices()) {
      const auto newNodePosition =
          computeArithmeticMeanOfEdgeConnectedNodes(mesh, nodeIndex);
      newNodePositions.at(nodeIndex) = newNodePosition;
      updateMaxSquaredNodeRelocationDistance(mesh.getNodes().at(nodeIndex),
                                             newNodePosition,
                                             maxSquaredNodeRelocationDistance);
    }
    mesh.setNodes(newNodePositions);
    if (iteration == config.maxIterations
        || maxSquaredNodeRelocationDistance
               <= config.maxSquaredNodeRelocationDistanceThreshold) {
      break;
    }
  }
  stopWatch.stop();

  return SmoothingResult("Basic Laplace", mesh,
                         stopWatch.getElapsedTimeInSeconds(), iteration);
}

namespace {
void updateNodePositionIfQualityIsImproved(
    const Mesh::PolygonalMesh& mesh,
    const std::vector<double>& polygonMeanRatioValuesForMesh,
    const std::size_t nodeIndexToUpdate,
    std::vector<Mathematics::Vector2D>& temporaryNewNodePositions,
    std::vector<Mathematics::Vector2D>& finalNewNodePositions) {
  const auto newNodePosition =
      computeArithmeticMeanOfEdgeConnectedNodes(mesh, nodeIndexToUpdate);
  temporaryNewNodePositions.at(nodeIndexToUpdate) = newNodePosition;

  double oldAttachedPolygonsMeanRatioSum = 0.0;
  double newAttachedPolygonsMeanRatioSum = 0.0;
  for (const auto attachedPolygonIndex :
       mesh.getAttachedPolygonIndices(nodeIndexToUpdate)) {
    oldAttachedPolygonsMeanRatioSum +=
        polygonMeanRatioValuesForMesh.at(attachedPolygonIndex);
    const double newPolygonMeanRatioValue = Mathematics::getMeanRatio(
        mesh.getPolygons().at(attachedPolygonIndex), temporaryNewNodePositions);
    if (newPolygonMeanRatioValue <= 0.0) {
      // Preliminary return if a polygon is invalidated;
      return;
    }
    newAttachedPolygonsMeanRatioSum += newPolygonMeanRatioValue;
  }
  if (newAttachedPolygonsMeanRatioSum > oldAttachedPolygonsMeanRatioSum) {
    finalNewNodePositions.at(nodeIndexToUpdate) = newNodePosition;
  }
}
}  // namespace

Smoothing::SmoothingResult Smoothing::smartLaplace(
    Mesh::PolygonalMesh mesh,
    const SmartLaplaceConfig& config) {
  std::size_t iteration = 0;
  auto polygonMeanRatioValues = Mesh::computeMeanRatioQualityNumberOfPolygons(
      mesh.getPolygons(), mesh.getNodes());
  auto oldMeshQuality = Mesh::MeshQuality(polygonMeanRatioValues, false);
  Utility::throwExceptionIfFalse(
      oldMeshQuality.isValidMesh(),
      "Smart Laplace can only be applied to valid initial meshes.");
  auto newNodePositions = mesh.getNodes();
  auto temporaryNodePositions = mesh.getNodes();
  // Data to be able to revert to mesh with best qMean at the end of smoothing.
  double bestQMeanValue = oldMeshQuality.getQMean();
  auto bestQMeanNodes = mesh.getNodes();

  Utility::StopWatch stopWatch;
  while (true) {
    ++iteration;
    for (const auto nodeIndex : mesh.getNonFixedNodeIndices()) {
      updateNodePositionIfQualityIsImproved(mesh, polygonMeanRatioValues,
                                            nodeIndex, temporaryNodePositions,
                                            newNodePositions);
      temporaryNodePositions.at(nodeIndex) = mesh.getNodes().at(nodeIndex);
    }
    const auto newMeshQuality =
        iterativelyResetNodesResultingInInvalidElementsSetNewMeshNodesAndUpdateElementQualityNumbers(
            newNodePositions, polygonMeanRatioValues, mesh);
    if (bestQMeanValue < newMeshQuality.getQMean()) {
      bestQMeanValue = newMeshQuality.getQMean();
      bestQMeanNodes = mesh.getNodes();
    }

    if (const double qMeanImprovement =
            newMeshQuality.getQMean() - oldMeshQuality.getQMean();
        iteration == config.maxIterations
        || qMeanImprovement <= config.qMeanImprovementThreshold) {
      break;
    }
    oldMeshQuality = newMeshQuality;
    newNodePositions = mesh.getNodes();
    temporaryNodePositions = mesh.getNodes();
  }
  stopWatch.stop();
  // Revert to mesh with best qMean value.
  mesh.getMutableNodes() = bestQMeanNodes;
  return SmoothingResult("Smart Laplace", mesh,
                         stopWatch.getElapsedTimeInSeconds(), iteration);
}
