/*
Generalized planar polygon transformations according to Definition 5.4 of the

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
// GETMe book.
#pragma once

#include <vector>

namespace Mathematics {
class Vector2D;
class Polygon;

class GeneralizedPolygonTransformation final {
public:
  // Constructor for given transformation parameters lambda and theta.
  GeneralizedPolygonTransformation(const double lambda, const double theta);

  // Constructor for polygons with n nodes using lambda = 0.5 and theta = pi/n.
  // According to Lemma 5.7 of the GETMe book this choice of theta is the middle
  // of the interval (theta_0, theta_1) thus resulting in a regularizing
  // transformation.
  explicit GeneralizedPolygonTransformation(
      const std::size_t numberOfPolygonNodes);

  double getLambda() const { return lambda; }

  double getTheta() const { return theta; }

  // Apply the transformation to the given polygon and nodes.
  std::vector<Vector2D> getNodesOfTransformedPolygon(
      const Polygon& polygon,
      const std::vector<Vector2D>& nodes) const;

  // Compute eigenvalues according to Lemma 5.2 of the GETMe book.
  std::vector<double> getEigenvalues(
      const std::size_t numberOfPolygonNodes) const;

  // Check if iteratively applying the transformation to polygons with the given
  // number of nodes results in counterclockwise regular polygons according to
  // Theorem 5.1 of the GETMe book. I.e. if the transformation is regularizing.
  bool isCounterclockwiseRegularizingTransformation(
      const std::size_t numberOfPolygonNodes) const;

private:
  double lambda;
  double theta;

  // Derived parameters according to Lemma 5.12 of the GETMe book.
  double c1;
  double c2;
  double c3;
};
}  // namespace Mathematics
