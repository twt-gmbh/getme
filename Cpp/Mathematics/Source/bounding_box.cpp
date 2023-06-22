// Representation of an axis aligned bounding box.
#include "Mathematics/bounding_box.h"

#include "Utility/exception_handling.h"

namespace Mathematics {
BoundingBox::BoundingBox(const double xMin,
                         const double xMax,
                         const double yMin,
                         const double yMax)
  : xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax) {
  Utility::throwExceptionIfFalse(xMin <= xMax, "xMin <= xMax expected.");
  Utility::throwExceptionIfFalse(yMin <= yMax, "yMin <= yMax expected.");
}
}  // namespace Mathematics
