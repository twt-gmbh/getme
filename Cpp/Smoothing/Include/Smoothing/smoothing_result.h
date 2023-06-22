// Result of a smoothing algorithm.
#pragma once

#include "Smoothing/smoothing_result_base.h"

#include <string>

namespace Smoothing {
struct SmoothingResult final : public SmoothingResultBase {
  explicit SmoothingResult(const std::string& algorithmName,
                           const Mesh::PolygonalMesh& mesh,
                           const double smoothingWallClockTimeInSeconds,
                           const std::size_t iterations)
    : SmoothingResultBase(algorithmName, mesh, smoothingWallClockTimeInSeconds)
    , iterations(iterations) {}

  const std::size_t iterations;
};
}  // namespace Smoothing
