/*
Mesh quality information class.

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

#include <optional>
#include <vector>

namespace Mesh {
class PolygonalMesh;

// Mesh quality numbers according to Equation 2.7 of the GETMe book if mesh
// consists of valid elements only. Otherwise q_min and q_mean are set to -1.0.
class MeshQuality final {
public:
  // Compute mesh quality for the given mesh. Computes all element mean ratio
  // numbers.
  explicit MeshQuality(const PolygonalMesh& mesh);

  // Compute mesh quality based on provided mean ratio numbers.
  // Terminates computation preliminary, if there is at least one
  // invalid element and determineNumberOfInvalidElements = false.
  explicit MeshQuality(const std::vector<double>& elementMeanRatioValues,
                       const bool determineNumberOfInvalidElements);

  // Get minimal mean ratio quality of all mesh elements.
  double getQMin() const { return qMin; }

  // Get minimal mean ratio quality of all mesh elements with at least one none
  // fixed node.
  const std::optional<double>& getQMinStar() const { return qMinStar; }

  // Arithmetic mean of all mesh element mean ratio quality numbers.
  double getQMean() const { return qMean; }

  // Get number of invalid elements. Always set if mesh constructor was used,
  // only set by quality numbers vector based constructor if requested.
  const std::optional<std::size_t>& getNumberOfInvalidElements() const {
    return numberOfInvalidElements;
  }

  bool isValidMesh() const { return qMin > 0.0; }

private:
  double qMin;
  std::optional<double> qMinStar;
  double qMean;
  std::optional<std::size_t> numberOfInvalidElements;
};
}  // namespace Mesh