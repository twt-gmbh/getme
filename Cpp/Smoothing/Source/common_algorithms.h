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
#pragma once

#include "Mathematics/generalized_polygon_transformation.h"
#include "Mathematics/polygon.h"
#include "Mathematics/polygon_algorithms.h"
#include "Mathematics/vector2d.h"

#include <vector>

namespace Mesh {
class PolygonalMesh;
struct MeshQuality;
}  // namespace Mesh

namespace Smoothing {
// Compute the edge length scaled polygon according to Definition 5.5 of the
// GETMe book. Both, the original nodes p_k as well as the transformed nodes
// p''_k have to be provided. The nodes p''_k will be adjusted to their edge
// scaled variants. Here it is used, that the original as well as the
// transformed polygon share the same centroid.
void applyEdgeLengthScaling(
    const Mathematics::Polygon& polygon,
    const std::vector<Mathematics::Vector2D>& originalMeshNodes,
    std::vector<Mathematics::Vector2D>& transformedElementNodes);

// Compute the squared distance d between a node and its new position and
// update the maximal squared distance dmax if dmax < d.
inline void updateMaxSquaredNodeRelocationDistance(
    const Mathematics::Vector2D& oldNodePosition,
    const Mathematics::Vector2D& newNodePosition,
    double& maxSquaredNodeRelocationDistance) {
  if (const double squaredNodeRelocationDistance =
          (newNodePosition - oldNodePosition).getLengthSquared();
      squaredNodeRelocationDistance > maxSquaredNodeRelocationDistance) {
    maxSquaredNodeRelocationDistance = squaredNodeRelocationDistance;
  }
}

// Transform a polygon and apply edge length scaling.
inline std::vector<Mathematics::Vector2D> transformAndScaleElement(
    const Mathematics::GeneralizedPolygonTransformation& transformation,
    const Mathematics::Polygon& polygon,
    const std::vector<Mathematics::Vector2D>& meshNodes) {
  auto transformedElementNodes =
      transformation.getNodesOfTransformedPolygon(polygon, meshNodes);
  applyEdgeLengthScaling(polygon, meshNodes, transformedElementNodes);
  return transformedElementNodes;
}

// Transform a polygon, apply edge length scaling and relaxation
// according to Definition 5.6 of the GETMe book.
inline std::vector<Mathematics::Vector2D> transformScaleAndRelaxElement(
    const Mathematics::GeneralizedPolygonTransformation& transformation,
    const double relaxationFactorRho,
    const Mathematics::Polygon& polygon,
    const std::vector<Mathematics::Vector2D>& meshNodes) {
  auto newElementNodes =
      transformAndScaleElement(transformation, polygon, meshNodes);
  if (relaxationFactorRho != 1.0) {
    const double oneMinusRho = 1.0 - relaxationFactorRho;
    for (std::size_t nodeNumber = 0; nodeNumber < polygon.getNumberOfNodes();
         ++nodeNumber) {
      newElementNodes.at(nodeNumber) =
          oneMinusRho * meshNodes.at(polygon.getNodeIndex(nodeNumber))
          + relaxationFactorRho * newElementNodes.at(nodeNumber);
    }
  }
  return newElementNodes;
}

// Iteratively reset nodes of invalid mesh elements to preserve mesh validity
// after applying a quality based simultaneous smoothing step. Updates all
// provided parameters.
Mesh::MeshQuality
iterativelyResetNodesResultingInInvalidElementsSetNewMeshNodesAndUpdateElementQualityNumbers(
    std::vector<Mathematics::Vector2D>& newNodePositions,
    std::vector<double>& polygonMeanRatioValuesForMesh,
    Mesh::PolygonalMesh& mesh);

// Check given transformations if they are suitable for GETMe smoothing for
// meshes with the given maximal number of polygon nodes.
void checkTransformations(
    const std::size_t maxNumberOfPolygonNodes,
    const std::vector<Mathematics::GeneralizedPolygonTransformation>&
        transformations);

// Check given transformations if they are suitable for GETMe smoothing of the
// given mesh.
void checkTransformations(
    const Mesh::PolygonalMesh& mesh,
    const std::vector<Mathematics::GeneralizedPolygonTransformation>&
        transformations);
}  // namespace Smoothing
