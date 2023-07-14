/*
Configuration of the GETMe smoothing algorithm.

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
// Cf. Section 6.2.1 of the GETMe book.
#pragma once

#include "Smoothing/getme_sequential_config.h"
#include "Smoothing/getme_simultaneous_config.h"

namespace Smoothing {
struct GetmeConfig final {
  // Constructor based on the simultaneous and sequential config constructors.
  explicit GetmeConfig(
      const std::size_t maxNumberOfPolygonNodes,
      const DefaultConfiguration::PolygonTransformationSet transformationSet =
          DefaultConfiguration::PolygonTransformationSet::GETMeBookExamples)
    : getmeSimultaneousConfig(
        GetmeSimultaneousConfig(maxNumberOfPolygonNodes, transformationSet))
    , getmeSequentialConfig(
          GetmeSequentialConfig(maxNumberOfPolygonNodes, transformationSet)) {}

  // Constructor based on given configs.
  explicit GetmeConfig(const GetmeSimultaneousConfig& getmeSimultaneousConfig,
                       const GetmeSequentialConfig& getmeSequentialConfig)
    : getmeSimultaneousConfig(getmeSimultaneousConfig)
    , getmeSequentialConfig(getmeSequentialConfig) {}

  GetmeSimultaneousConfig getmeSimultaneousConfig;
  GetmeSequentialConfig getmeSequentialConfig;
};
}  // namespace Smoothing
