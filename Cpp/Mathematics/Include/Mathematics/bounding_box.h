// Representation of an axis aligned bounding box.
#pragma once

namespace Mathematics {
class BoundingBox final {
public:
  BoundingBox(const double xMin,
              const double xMax,
              const double yMin,
              const double yMax);

  bool operator==(const BoundingBox& other) const = default;

  double getXMin() const { return xMin; }

  double getXMax() const { return xMax; }

  double getYMin() const { return yMin; }

  double getYMax() const { return yMax; }

  double getXDimension() const { return xMax - xMin; }

  double getYDimension() const { return yMax - yMin; }

private:
  const double xMin;
  const double xMax;
  const double yMin;
  const double yMax;
};
}  // namespace Mathematics
