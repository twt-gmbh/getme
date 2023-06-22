// Configuration for the smart Laplacian smoothing algorithm involving element
// quality.
#pragma once

#include "Smoothing/default_configuration.h"

#include <cstddef>

namespace Smoothing {
struct SmartLaplaceConfig {
  // Terminate if the improvement of the arithmetic mean of all element quality
  // numbers of one iteration drops below the given threshold.
  double qMeanImprovementThreshold =
      DefaultConfiguration::qMeanImprovementThreshold;

  // Terminate if the number of iterations exceeds this limit.
  std::size_t maxIterations = DefaultConfiguration::maxIterations;
};
}  // namespace Smoothing
