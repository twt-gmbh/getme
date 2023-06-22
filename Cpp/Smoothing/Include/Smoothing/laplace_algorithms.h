// Variants of Laplacian smoothing algorithms.
#pragma once

namespace Mesh {
class PolygonalMesh;
}

namespace Smoothing {
struct BasicLaplaceConfig;
struct SmartLaplaceConfig;
struct SmoothingResult;

// Classic Laplacian smoothing by iterative connected node averaging.
// Node updates are computed simultaneously and set afterwards. Thus the order
// of nodes has no impact on the smoothing results.
// Cf. Section 4.2.1 of the GETMe book.
SmoothingResult basicLaplace(Mesh::PolygonalMesh mesh,
                             const BasicLaplaceConfig& config);

// Smart Laplacian smoothing updating nodes only if this increases
// the arithmetic mean of the mean ratio quality numbers of attached elements.
// Node updates are computed simultaneously and set afterwards. Thus the order
// of nodes has no impact on the smoothing results.
// Cf. Section 4.2.1 of the GETMe book.
SmoothingResult smartLaplace(Mesh::PolygonalMesh mesh,
                             const SmartLaplaceConfig& config);
}  // namespace Smoothing
