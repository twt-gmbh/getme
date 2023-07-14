/*
Unit tests for bounding boxes.

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
#include "Mathematics/bounding_box.h"

#include "gtest/gtest.h"

#include <vector>

TEST(BoundingBox, basicGetters) {
  const double xMin = 4.0;
  const double xMax = 8.0;
  const double yMin = -2.0;
  const double yMax = 6.0;
  const Mathematics::BoundingBox boundingBox(xMin, xMax, yMin, yMax);

  EXPECT_EQ(xMin, boundingBox.getXMin());
  EXPECT_EQ(xMax, boundingBox.getXMax());
  EXPECT_EQ(yMin, boundingBox.getYMin());
  EXPECT_EQ(yMax, boundingBox.getYMax());

  EXPECT_EQ(xMax - xMin, boundingBox.getXDimension());
  EXPECT_EQ(yMax - yMin, boundingBox.getYDimension());
}

TEST(BoundingBox, validityChecks) {
  EXPECT_ANY_THROW(Mathematics::BoundingBox(2.0, -1.0, 0.0, 1.0));
  EXPECT_ANY_THROW(Mathematics::BoundingBox(2.0, 4.0, 4.0, 1.0));
  EXPECT_ANY_THROW(Mathematics::BoundingBox(8.0, 4.0, 4.0, 1.0));
  EXPECT_NO_THROW(Mathematics::BoundingBox(2.0, 4.0, 0.0, 1.0));
  EXPECT_NO_THROW(Mathematics::BoundingBox(1.0, 1.0, 2.0, 2.0));
}

TEST(BoundingBox, equalsOperator) {
  EXPECT_TRUE((Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.0}
               == Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.0}));
  EXPECT_FALSE((Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.0}
                == Mathematics::BoundingBox{1.1, 2.0, 3.0, 4.0}));
  EXPECT_FALSE((Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.0}
                == Mathematics::BoundingBox{1.0, 2.2, 3.0, 4.0}));
  EXPECT_FALSE((Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.0}
                == Mathematics::BoundingBox{1.0, 2.0, 3.3, 4.0}));
  EXPECT_FALSE((Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.0}
                == Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.4}));
}
