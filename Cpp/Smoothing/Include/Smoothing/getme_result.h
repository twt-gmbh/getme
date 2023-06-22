// Result structure of the GETMe smoothing algorithm.
#pragma once

#include "Mesh/polygonal_mesh.h"
#include "Smoothing/smoothing_result.h"

#include <string>

namespace Smoothing {
struct GetmeResult final : public SmoothingResultBase {
  explicit GetmeResult(const SmoothingResult& getmeSimultaneousSmoothingResult,
                       const SmoothingResult& getmeSequentialSmoothingResult)
    : SmoothingResultBase(
        "GETMe",
        getmeSequentialSmoothingResult.mesh,
        getmeSimultaneousSmoothingResult.smoothingWallClockTimeInSeconds
            + getmeSequentialSmoothingResult.smoothingWallClockTimeInSeconds)
    , getmeSimultaneousIterations(getmeSimultaneousSmoothingResult.iterations)
    , getmeSequentialIterations(getmeSequentialSmoothingResult.iterations) {}

  const std::size_t getmeSimultaneousIterations;
  const std::size_t getmeSequentialIterations;
};
}  // namespace Smoothing
