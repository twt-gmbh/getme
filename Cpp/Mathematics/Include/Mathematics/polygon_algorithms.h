// Algorithms for planar polygons.
#pragma once

#include <vector>

namespace Mathematics {
class Polygon;
class Vector2D;

// Get nodes of a counter clockwise oriented regular polygon with centroid
// (0,0) and radius 1.0.
std::vector<Vector2D> getNodesOfRegularPolygon(const std::size_t numberOfNodes);

// Compute mean ratio quality number for the given polygon and nodes according
// to Equation 2.6 of the GETMe book. Returns a quality number in [0,1] for
// valid polygons and -1 for invalid polygons.
double getMeanRatio(const Polygon& polygon, const std::vector<Vector2D>& nodes);
}  // namespace Mathematics
