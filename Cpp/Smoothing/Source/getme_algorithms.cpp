// Variants of Laplacian smoothing algorithms.
#include "Smoothing/getme_algorithms.h"

#include "Mathematics/vector2d.h"
#include "Mesh/polygonal_mesh.h"
#include "Mesh/polygonal_mesh_algorithms.h"
#include "Smoothing/basic_getme_simultaneous_config.h"
#include "Smoothing/getme_config.h"
#include "Smoothing/getme_result.h"
#include "Smoothing/getme_sequential_config.h"
#include "Smoothing/getme_simultaneous_config.h"
#include "Smoothing/smoothing_result.h"
#include "Utility/exception_handling.h"
#include "Utility/stop_watch.h"
#include "common_algorithms.h"
#include "getme_sequential.h"
#include "polygon_quality_min_heap.h"

#include <algorithm>
#include <cmath>
#include <execution>
#include <string>

Smoothing::SmoothingResult Smoothing::basicGetmeSimultaneous(
    Mesh::PolygonalMesh mesh,
    const BasicGetmeSimultaneousConfig& config) {
  checkTransformations(mesh, config.polygonTransformations);
  std::size_t iteration = 0;
  const auto& polygons = mesh.getPolygons();
  std::vector<Mathematics::Vector2D> newNodePositions(
      mesh.getNumberOfNodes(), Mathematics::Vector2D(0.0, 0.0));

  Utility::StopWatch stopWatch;
  while (true) {
    for (const auto& polygon : polygons) {
      const auto numberOfPolygonNodes = polygon.getNumberOfNodes();
      auto transformedNodes = transformAndScaleElement(
          config.polygonTransformations.at(numberOfPolygonNodes), polygon,
          mesh.getNodes());
      for (std::size_t nodeNumber = 0; nodeNumber < numberOfPolygonNodes;
           ++nodeNumber) {
        newNodePositions.at(polygon.getNodeIndex(nodeNumber)) +=
            transformedNodes.at(nodeNumber);
      }
    }
    double maxSquaredNodeRelocationDistance = 0.0;
    for (const auto nodeIndex : mesh.getNonFixedNodeIndices()) {
      const auto newNodePosition =
          newNodePositions.at(nodeIndex)
          / static_cast<double>(
              mesh.getAttachedPolygonIndices(nodeIndex).size());
      updateMaxSquaredNodeRelocationDistance(mesh.getNodes().at(nodeIndex),
                                             newNodePosition,
                                             maxSquaredNodeRelocationDistance);
      mesh.getMutableNodes().at(nodeIndex) = newNodePosition;
    }
    if (++iteration == config.maxIterations
        || maxSquaredNodeRelocationDistance
               <= config.maxSquaredNodeRelocationDistanceThreshold) {
      break;
    }
    newNodePositions.assign(mesh.getNumberOfNodes(),
                            Mathematics::Vector2D(0.0, 0.0));
  }
  stopWatch.stop();
  return SmoothingResult("Basic GETMe simultaneous", mesh,
                         stopWatch.getElapsedTimeInSeconds(), iteration);
}

Smoothing::SmoothingResult Smoothing::getmeSimultaneous(
    Mesh::PolygonalMesh mesh,
    const GetmeSimultaneousConfig& config) {
  checkTransformations(mesh, config.polygonTransformations);
  std::size_t iteration = 0;
  const auto& polygons = mesh.getPolygons();
  auto polygonMeanRatioValues =
      Mesh::computeMeanRatioQualityNumberOfPolygons(polygons, mesh.getNodes());
  auto oldMeshQuality = Mesh::MeshQuality(polygonMeanRatioValues, false);
  Utility::throwExceptionIfFalse(
      oldMeshQuality.isValidMesh(),
      "GETMe simultaneous can only be applied to valid initial meshes.");
  std::vector<Mathematics::Vector2D> transformedNodeSums(
      mesh.getNumberOfNodes(), Mathematics::Vector2D(0.0, 0.0));
  auto newNodePositions = mesh.getNodes();
  std::vector<double> nodeWeightSums(mesh.getNumberOfNodes(), 0.0);
  double bestQMeanValue = oldMeshQuality.getQMean();
  auto bestQMeanNodes = mesh.getNodes();

  Utility::StopWatch stopWatch;
  while (true) {
    // Transform all polygons and sum up nodes.
    for (std::size_t polygonIndex = 0; polygonIndex < polygons.size();
         ++polygonIndex) {
      const auto& polygon = polygons.at(polygonIndex);
      const auto numberOfPolygonNodes = polygon.getNumberOfNodes();
      auto transformedNodes = transformScaleAndRelaxElement(
          config.polygonTransformations.at(numberOfPolygonNodes),
          config.relaxationParameterRho, polygon, mesh.getNodes());
      const double weight =
          config.weightExponentEta == 0.0
              ? 1.0
              : std::pow(1.0 - polygonMeanRatioValues.at(polygonIndex),
                         config.weightExponentEta);
      for (std::size_t nodeNumber = 0; nodeNumber < numberOfPolygonNodes;
           ++nodeNumber) {
        const auto nodeIndex = polygon.getNodeIndex(nodeNumber);
        transformedNodeSums.at(nodeIndex) +=
            weight * transformedNodes.at(nodeNumber);
        nodeWeightSums.at(nodeIndex) += weight;
      }
    }
    // Compute new nodes and assess.
    for (const auto nodeIndex : mesh.getNonFixedNodeIndices()) {
      if (nodeWeightSums.at(nodeIndex) > 0.0) {
        newNodePositions.at(nodeIndex) =
            transformedNodeSums.at(nodeIndex) / nodeWeightSums.at(nodeIndex);
      }
    }
    const auto newMeshQuality =
        iterativelyResetNodesResultingInInvalidElementsSetNewMeshNodesAndUpdateElementQualityNumbers(
            newNodePositions, polygonMeanRatioValues, mesh);
    if (bestQMeanValue < newMeshQuality.getQMean()) {
      bestQMeanValue = newMeshQuality.getQMean();
      bestQMeanNodes = mesh.getNodes();
    }
    // Check termination criteria and set data.
    if (const double qMeanImprovement =
            newMeshQuality.getQMean() - oldMeshQuality.getQMean();
        ++iteration == config.maxIterations
        || qMeanImprovement <= config.qMeanImprovementThreshold) {
      break;
    }
    oldMeshQuality = newMeshQuality;
    transformedNodeSums.assign(mesh.getNumberOfNodes(),
                               Mathematics::Vector2D(0.0, 0.0));
    nodeWeightSums.assign(mesh.getNumberOfNodes(), 0.0);
    newNodePositions = mesh.getNodes();
  }
  stopWatch.stop();

  mesh.getMutableNodes() = bestQMeanNodes;
  return SmoothingResult("GETMe simultaneous", mesh,
                         stopWatch.getElapsedTimeInSeconds(), iteration);
}

Smoothing::SmoothingResult Smoothing::getmeSequential(
    const Mesh::PolygonalMesh& mesh,
    const GetmeSequentialConfig& config) {
  // Since GETMe sequential uses more helper data, an algorithm class is used.
  GetmeSequential algorithm(mesh, config);
  return algorithm.getResult();
}

Smoothing::GetmeResult Smoothing::getme(const Mesh::PolygonalMesh& mesh,
                                        const GetmeConfig& config) {
  const auto getmeSimultaneousResult =
      getmeSimultaneous(mesh, config.getmeSimultaneousConfig);
  const auto getmeSequentialResult = getmeSequential(
      getmeSimultaneousResult.mesh, config.getmeSequentialConfig);
  return GetmeResult(getmeSimultaneousResult, getmeSequentialResult);
}
