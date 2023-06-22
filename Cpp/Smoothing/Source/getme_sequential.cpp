// Algorithm class for GETMe sequential smoothing according to Section 6.1.3 of
// the GETMe book.
#include "getme_sequential.h"

#include "Smoothing/smoothing_result.h"
#include "Utility/stop_watch.h"
#include "common_algorithms.h"

#include <execution>

namespace Smoothing {
GetmeSequential::GetmeSequential(const Mesh::PolygonalMesh& mesh,
                                 const GetmeSequentialConfig& config)
  : mesh(mesh), config(config), minHeap(mesh) {
  checkInputData();
  initHelperData();
  applySmoothing();
  Utility::throwExceptionIfFalse(minHeap.isConsistent(),
                                 "Inconsistent min heap.");
}

SmoothingResult GetmeSequential::getResult() const {
  return SmoothingResult("GETMe sequential", mesh, smoothingTimeInSeconds,
                         iterationsApplied);
}

void GetmeSequential::checkInputData() const {
  Utility::throwExceptionIfTrue(
      minHeap.containsAnInvalidPolygon(),
      "GETMe sequential can only be applied to valid initial meshes.");
  Utility::throwExceptionIfFalse(
      config.qualityEvaluationCycleLength < config.maxIterations,
      "Quality evaluation cycle length must be <= maximal number of "
      "iterations.");
  checkTransformations(mesh, config.polygonTransformations);
}

void GetmeSequential::initHelperData() {
  isNodeFixed = std::vector<bool>(mesh.getNumberOfNodes(), false);
  const auto setNodeFixed = [this](const std::size_t fixedNodeIndex) {
    isNodeFixed.at(fixedNodeIndex) = true;
  };
  const auto& fixedNodeIndices = mesh.getFixedNodeIndices();
  std::for_each(std::execution::par_unseq, fixedNodeIndices.begin(),
                fixedNodeIndices.end(), setNodeFixed);

  temporaryNodes = mesh.getNodes();
}

void GetmeSequential::applySmoothing() {
  std::size_t iteration = 0;
  const auto& polygons = mesh.getPolygons();

  std::size_t lastTransformedPolygonIndex =
      std::numeric_limits<std::size_t>::max();

  double lastQMinStar = minHeap.getQMinStar();
  double bestQMinStarValue = lastQMinStar;
  auto bestQMinStarNodes = mesh.getNodes();
  std::size_t numberOfConsecutiveNoImproveCycles = 0;

  Utility::StopWatch stopWatch;
  while (true) {
    ++iteration;
    const std::size_t transformedPolygonIndex =
        minHeap.getLowestQualityPolygonIndex();

    if (lastTransformedPolygonIndex == transformedPolygonIndex) {
      minHeap.addToPenaltySum(transformedPolygonIndex, config.penaltyRepeated);
    }

    const auto& transformedPolygon = polygons.at(transformedPolygonIndex);
    transformPolygonAndSetTemporaryNodes(transformedPolygon);
    if (const auto localQualityInfo =
            assessLocalQuality(transformedPolygonIndex);
        !localQualityInfo.areAllElementsValid) {
      // Reset temporary nodes.
      copyNodes(transformedPolygonIndex, mesh.getNodes(), temporaryNodes);
      minHeap.addToPenaltySum(transformedPolygonIndex, config.penaltyInvalid);
    } else {
      // Set final nodes and update element qualities.
      copyNodes(transformedPolygonIndex, temporaryNodes,
                mesh.getMutableNodes());
      minHeap.updateMeanRatioNumberAndAddToPenaltySum(
          transformedPolygonIndex,
          localQualityInfo.transformedElementMeanRatioNumber,
          -config.penaltySuccess);
      for (const auto& [polygonIndex, newMeanRatioNumber] :
           localQualityInfo.neighborElementIndexAndMeanRatioNumber) {
        minHeap.updateMeanRatioNumberIfNotFixedPolygon(polygonIndex,
                                                       newMeanRatioNumber);
      }
    }
    lastTransformedPolygonIndex = transformedPolygonIndex;

    if (iteration % config.qualityEvaluationCycleLength == 0) {
      const double qMinStar = minHeap.getQMinStar();
      if (qMinStar > bestQMinStarValue) {
        bestQMinStarValue = qMinStar;
        bestQMinStarNodes = mesh.getNodes();
        numberOfConsecutiveNoImproveCycles = 0;
      } else {
        ++numberOfConsecutiveNoImproveCycles;
      }
    }
    if (iteration == config.maxIterations
        || numberOfConsecutiveNoImproveCycles
               == config.maxNoImprovementCycles) {
      break;
    }
  }
  stopWatch.stop();

  // Set result data.
  mesh.getMutableNodes() = bestQMinStarNodes;
  iterationsApplied = iteration;
  smoothingTimeInSeconds = stopWatch.getElapsedTimeInSeconds();
}

void GetmeSequential::transformPolygonAndSetTemporaryNodes(
    const Mathematics::Polygon& polygon) {
  auto transformedNodes = transformScaleAndRelaxElement(
      config.polygonTransformations.at(polygon.getNumberOfNodes()),
      config.relaxationParameterRho, polygon, mesh.getNodes());
  const auto& nodeIndices = polygon.getNodeIndices();
  for (std::size_t nodeNumber = 0; nodeNumber < nodeIndices.size();
       ++nodeNumber) {
    const std::size_t nodeIndex = nodeIndices.at(nodeNumber);
    if (!isNodeFixed.at(nodeIndex)) {
      temporaryNodes.at(nodeIndex) = transformedNodes.at(nodeNumber);
    }
  }
}

GetmeSequential::LocalQualityResult GetmeSequential::assessLocalQuality(
    const std::size_t transformedPolygonIndex) const {
  const auto& polygons = mesh.getPolygons();
  LocalQualityResult result;
  result.transformedElementMeanRatioNumber = Mathematics::getMeanRatio(
      polygons.at(transformedPolygonIndex), temporaryNodes);
  if (result.transformedElementMeanRatioNumber <= 0.0) {
    return result;
  }
  for (const auto neighborPolygonIndex :
       mesh.getIndicesOfNeighborPolygons(transformedPolygonIndex)) {
    const double neighborMeanRatioNumber = Mathematics::getMeanRatio(
        polygons.at(neighborPolygonIndex), temporaryNodes);
    // Preliminary termination if one neighbor was invalidated.
    if (neighborMeanRatioNumber <= 0.0) {
      return result;
    }
    result.neighborElementIndexAndMeanRatioNumber.emplace_back(
        neighborPolygonIndex, neighborMeanRatioNumber);
  }
  result.areAllElementsValid = true;
  return result;
}

void GetmeSequential::copyNodes(
    const std::size_t polygonIndex,
    const std::vector<Mathematics::Vector2D>& sourceNodes,
    std::vector<Mathematics::Vector2D>& targetNodes) const {
  for (const auto nodeIndex :
       mesh.getPolygons().at(polygonIndex).getNodeIndices()) {
    targetNodes.at(nodeIndex) = sourceNodes.at(nodeIndex);
  }
}
}  // namespace Smoothing
