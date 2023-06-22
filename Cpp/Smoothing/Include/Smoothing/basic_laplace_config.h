// Configuration for basic Laplacian smoothing not involving element quality.
#pragma once

#include "Smoothing/default_configuration.h"

#include <cstddef>

namespace Smoothing {
struct BasicLaplaceConfig final {
  explicit BasicLaplaceConfig(const double maxNodeRelocationDistanceThreshold)
    : maxSquaredNodeRelocationDistanceThreshold(
        maxNodeRelocationDistanceThreshold
        * maxNodeRelocationDistanceThreshold) {}

  // Terminated if the maximal distance of a node relocation drops below the
  // given threshold. This threshold depends on the scale of the mesh elements.
  // It is squared to avoid square root evaluations.
  double maxSquaredNodeRelocationDistanceThreshold;

  // Terminate if the number of iterations exceeds this limit.
  std::size_t maxIterations = DefaultConfiguration::maxIterations;
};
}  // namespace Smoothing
