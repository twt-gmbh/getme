// Representation of a planar polygonal mesh with fixed topology but adjustable
// nodes.
#include "Mesh/polygonal_mesh.h"

#include "Utility/exception_handling.h"

#include <algorithm>

namespace Mesh {
PolygonalMesh::PolygonalMesh(
    const std::vector<Mathematics::Vector2D>& nodes,
    const std::vector<Mathematics::Polygon>& polygons,
    const std::unordered_set<std::size_t>& fixedNodeIndices)
  : nodes(nodes)
  , polygons(polygons)
  , fixedNodeIndices(fixedNodeIndices)
  , areAllPolygonNodesFixed(polygons.size(), false)
  , indicesOfEdgeConnectedNodes(nodes.size(), std::unordered_set<std::size_t>())
  , attachedPolygonIndices(nodes.size(), std::unordered_set<std::size_t>())
  , indicesOfNeighborPolygons(polygons.size(),
                              std::unordered_set<std::size_t>()) {
  setNonFixedNodes();
  setFixedPolygonAndNodeTopologyData();
  setIndicesOfNeighborPolygons();
}

void PolygonalMesh::setNodes(
    const std::vector<Mathematics::Vector2D>& newNodes) {
  Utility::throwExceptionIfTrue(nodes.size() != newNodes.size(),
                                "Non matching number of nodes.");
  nodes = newNodes;
}

void PolygonalMesh::setNonFixedNodes() {
  const std::size_t numberOfMeshNodes = nodes.size();
  Utility::throwExceptionIfFalse(
      fixedNodeIndices.size() <= nodes.size(),
      "Number of fixed nodes cannot be larger than number of nodes.");
  nonFixedNodeIndices.reserve(nodes.size() - fixedNodeIndices.size());
  for (std::size_t nodeIndex = 0; nodeIndex < numberOfMeshNodes; ++nodeIndex) {
    if (!fixedNodeIndices.contains(nodeIndex)) {
      nonFixedNodeIndices.push_back(nodeIndex);
    }
  }
}

void PolygonalMesh::setFixedPolygonAndNodeTopologyData() {
  const auto isNodeFixedPredicate = [this](const std::size_t nodeIndex) {
    return fixedNodeIndices.contains(nodeIndex);
  };

  for (std::size_t polygonIndex = 0; polygonIndex < polygons.size();
       ++polygonIndex) {
    // Set all polygon nodes fixed info
    const auto& polygon = polygons.at(polygonIndex);
    areAllPolygonNodesFixed.at(polygonIndex) =
        std::ranges::all_of(polygon.getNodeIndices(), isNodeFixedPredicate);
    // Update maximal number of polygon nodes
    const std::size_t numberOfPolygonNodes = polygon.getNumberOfNodes();
    if (numberOfPolygonNodes > maximalNumberOfPolygonNodes) {
      maximalNumberOfPolygonNodes = numberOfPolygonNodes;
    }
    // Set topology info
    for (std::size_t nodeNumber = 0; nodeNumber < numberOfPolygonNodes;
         ++nodeNumber) {
      const auto predecessorNodeIndex =
          polygon.getPredecessorNodeIndex(nodeNumber);
      const auto currentNodeIndex = polygon.getNodeIndex(nodeNumber);
      const auto successorNodeIndex = polygon.getSuccessorNodeIndex(nodeNumber);

      Utility::throwExceptionIfTrue(currentNodeIndex >= nodes.size(),
                                    "Node index exceeds number of mesh nodes.");

      indicesOfEdgeConnectedNodes.at(currentNodeIndex)
          .insert(predecessorNodeIndex);
      indicesOfEdgeConnectedNodes.at(currentNodeIndex)
          .insert(successorNodeIndex);

      attachedPolygonIndices.at(currentNodeIndex).insert(polygonIndex);
    }
  }
}

void PolygonalMesh::setIndicesOfNeighborPolygons() {
  for (std::size_t polygonIndex = 0; polygonIndex < polygons.size();
       ++polygonIndex) {
    // Collect all polygon indices attached to the nodes of the polygon.
    for (const auto nodeIndex : polygons.at(polygonIndex).getNodeIndices()) {
      const auto& nodeAttachedPolygonIndices =
          attachedPolygonIndices.at(nodeIndex);
      indicesOfNeighborPolygons.at(polygonIndex)
          .insert(nodeAttachedPolygonIndices.begin(),
                  nodeAttachedPolygonIndices.end());
    }
    // Remove self index.
    indicesOfNeighborPolygons.at(polygonIndex).erase(polygonIndex);
  }
}
}  // namespace Mesh
