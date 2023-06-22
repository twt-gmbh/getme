// Configuration of the GETMe smoothing algorithm.
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
