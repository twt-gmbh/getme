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
#pragma once

#include "Mathematics/polygon.h"
#include "Mathematics/vector2d.h"

#include <unordered_set>
#include <vector>

namespace Mesh {
class PolygonalMesh final {
public:
  // Constructor of mesh by nodes an polygons. Fixed node indices indicate
  // nodes, which should not be modified during smoothing.
  PolygonalMesh(const std::vector<Mathematics::Vector2D>& nodes,
                const std::vector<Mathematics::Polygon>& polygons,
                const std::unordered_set<std::size_t>& fixedNodeIndices =
                    std::unordered_set<std::size_t>());

  const std::vector<Mathematics::Vector2D>& getNodes() const { return nodes; }

  std::vector<Mathematics::Vector2D>& getMutableNodes() { return nodes; }

  void setNodes(const std::vector<Mathematics::Vector2D>& newNodes);

  std::size_t getNumberOfNodes() const { return nodes.size(); }

  const std::vector<Mathematics::Polygon>& getPolygons() const {
    return polygons;
  }

  const std::unordered_set<std::size_t>& getFixedNodeIndices() const {
    return fixedNodeIndices;
  }

  const std::vector<std::size_t>& getNonFixedNodeIndices() const {
    return nonFixedNodeIndices;
  }

  bool isFixedPolygon(const std::size_t polygonIndex) const {
    return areAllPolygonNodesFixed.at(polygonIndex);
  }

  std::size_t getNumberOfPolygons() const { return polygons.size(); }

  const std::unordered_set<std::size_t>& getIndicesOfEdgeConnectedNodes(
      const std::size_t nodeIndex) const {
    return indicesOfEdgeConnectedNodes.at(nodeIndex);
  }

  const std::unordered_set<std::size_t>& getAttachedPolygonIndices(
      const std::size_t nodeIndex) const {
    return attachedPolygonIndices.at(nodeIndex);
  }

  const std::unordered_set<std::size_t>& getIndicesOfNeighborPolygons(
      const std::size_t polygonIndex) const {
    return indicesOfNeighborPolygons.at(polygonIndex);
  }

  std::size_t getMaximalNumberOfPolygonNodes() const {
    return maximalNumberOfPolygonNodes;
  }

private:
  // Helper functions used for data initialization.
  void setNonFixedNodes();
  void setFixedPolygonAndNodeTopologyData();
  void setIndicesOfNeighborPolygons();

  // Basic constructor data.
  std::vector<Mathematics::Vector2D> nodes;
  std::vector<Mathematics::Polygon> polygons;
  std::unordered_set<std::size_t> fixedNodeIndices;

  // Derived fixed node/element information.

  // Vector of all node indices of non fixed nodes.
  std::vector<std::size_t> nonFixedNodeIndices;
  // For the polygon with index k areAllPolygonNodesFixed[k] indicates if all
  // nodes of the polygon are fixed.
  std::vector<bool> areAllPolygonNodesFixed;

  // Derived topology data.

  // For the node with index k, indicesOfEdgeConnectedNodes[k] gives the
  // indices of nodes connected by an edge.
  std::vector<std::unordered_set<std::size_t>> indicesOfEdgeConnectedNodes;

  // For the node with index k, attachedPolygonIndices[k] gives the
  // vector of attached polygon indices.
  std::vector<std::unordered_set<std::size_t>> attachedPolygonIndices;

  // For the polygon with index k, indicesOfNeighborPolygons[k] gives the
  // indices of neighboring polygons sharing a common edge or node.
  std::vector<std::unordered_set<std::size_t>> indicesOfNeighborPolygons;

  // The maximal number of polygon nodes for the polygons contained.
  std::size_t maximalNumberOfPolygonNodes = 0;
};
}  // namespace Mesh
