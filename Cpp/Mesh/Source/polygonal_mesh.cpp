/*
Representation of a planar polygonal mesh with fixed topology but adjustable

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
