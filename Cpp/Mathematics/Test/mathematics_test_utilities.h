// Utility functions for mathematics tests.
#pragma once

#include <complex>
#include <vector>

namespace Mathematics {
class Polygon;
class Vector2D;
}  // namespace Mathematics

namespace MathematicsTestUtilities {
std::vector<Mathematics::Vector2D> getSampleNodes();

// Get polygon with node indices 0 to numberOfNodes-1.
Mathematics::Polygon getPolygon(const std::size_t numberOfNodes);

std::vector<Mathematics::Vector2D> complexToVector2DNodes(
    const std::vector<std::complex<double>>& complexNodes);

std::vector<std::complex<double>> vector2DToComplexNodes(
    const std::vector<Mathematics::Vector2D>& vector2DNodes);
}  // namespace MathematicsTestUtilities
