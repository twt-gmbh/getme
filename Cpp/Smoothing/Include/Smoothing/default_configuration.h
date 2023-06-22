// Common defaults for smoothing algorithm configurations.
#pragma once

#include "Mathematics/generalized_polygon_transformation.h"

#include <cstddef>
#include <vector>

namespace Smoothing::DefaultConfiguration {
// Maximal number of smoothing iterations for algorithms improving all elements
// within one smoothing step.
constexpr const std::size_t maxIterations = 10'000;

// Quality based termination threshold for algorithms improving all elements
// within one smoothing step. Terminate if the improvement of the arithmetic
// mean of all element quality numbers of one iteration drops below the
// given threshold.
constexpr const double qMeanImprovementThreshold = 1.0e-4;

// Regularizing transformation set to use for polygons in GETMe variants.
enum class PolygonTransformationSet {
  // Generic transformation scheme using lambda=1/2 and theta=pi/n for polygons
  // with n nodes.
  Generic,

  // Transformation scheme as used in the GETMe book examples chapter. Like
  // generic but uses theta=pi/4 for n=3 and theta=pi/6 for n=4, respectively.
  GETMeBookExamples,
};

// Get default regularizing polygon transformation set vector. Polygons with n
// nodes will be transformed by the n-th entry transformation of this vector.
std::vector<Mathematics::GeneralizedPolygonTransformation>
getRegularizingPolygonTransformations(
    const std::size_t maxNumberOfPolygonNodes,
    const PolygonTransformationSet transformationSet);
}  // namespace Smoothing::DefaultConfiguration
