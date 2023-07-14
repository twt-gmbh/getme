/*
Result structure of the GETMe smoothing algorithm.

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
