// Variants of GETMe smoothing algorithms.
#pragma once

namespace Mesh {
class PolygonalMesh;
}

namespace Smoothing {
struct BasicGetmeSimultaneousConfig;
struct GetmeConfig;
struct GetmeResult;
struct GetmeSequentialConfig;
struct GetmeSimultaneousConfig;
struct SmoothingResult;

// Basic GETMe simultaneous smoothing according to Section 6.1.1
// of the GETMe book.
SmoothingResult basicGetmeSimultaneous(
    Mesh::PolygonalMesh mesh,
    const BasicGetmeSimultaneousConfig& config);

// GETMe simultaneous smoothing according to Section 6.1.2 of the GETMe book.
SmoothingResult getmeSimultaneous(Mesh::PolygonalMesh mesh,
                                  const GetmeSimultaneousConfig& config);

// GETMe sequential smoothing according to Section 6.1.3 of the GETMe book.
SmoothingResult getmeSequential(const Mesh::PolygonalMesh& mesh,
                                const GetmeSequentialConfig& config);

// GETMe sequential smoothing according to Section 6.2.1 of the GETMe book.
GetmeResult getme(const Mesh::PolygonalMesh& mesh, const GetmeConfig& config);
}  // namespace Smoothing
