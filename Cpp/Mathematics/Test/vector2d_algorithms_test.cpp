/*
Unit tests for 2D vector algorithms.

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
#include "Mathematics/vector2d_algorithms.h"

#include "Mathematics/bounding_box.h"
#include "Mathematics/vector2d.h"

#include "gtest/gtest.h"

TEST(Vector2DAlgorithms, areEqual_singleVector) {
  const Mathematics::Vector2D vector1(1.0, -2.0);
  const Mathematics::Vector2D vector2(0.9, -1.99);

  EXPECT_TRUE(Mathematics::areEqual(vector1, vector1, 0.0));
  EXPECT_TRUE(Mathematics::areEqual(vector1, vector1, 1.0));
  EXPECT_FALSE(Mathematics::areEqual(vector1, vector2, 0.0));
  EXPECT_FALSE(Mathematics::areEqual(vector1, vector2, 0.1));
  EXPECT_TRUE(Mathematics::areEqual(vector1, vector2, 0.15));
}

TEST(Vector2DAlgorithms, areEqual_multipleVectors) {
  const Mathematics::Vector2D vector1(1.0, -2.0);
  const Mathematics::Vector2D vector2(0.9, -1.99);
  const Mathematics::Vector2D vector3(1.0, 2.0);

  const std::vector<Mathematics::Vector2D> vectors1{vector1, vector2, vector3};
  const std::vector<Mathematics::Vector2D> vectors2{vector2, vector2, vector3};
  const std::vector<Mathematics::Vector2D> vectors3{vector3, vector2, vector3};
  const std::vector<Mathematics::Vector2D> vectors4{vector3, vector2};

  EXPECT_TRUE(Mathematics::areEqual(vectors1, vectors1, 0.0));
  EXPECT_TRUE(Mathematics::areEqual(vectors1, vectors1, 1.0));
  EXPECT_FALSE(Mathematics::areEqual(vectors1, vectors2, 0.0));
  EXPECT_FALSE(Mathematics::areEqual(vectors1, vectors2, 0.1));
  EXPECT_TRUE(Mathematics::areEqual(vectors1, vectors2, 0.15));
  EXPECT_TRUE(Mathematics::areEqual(vectors1, vectors3, 4.0));
  EXPECT_FALSE(Mathematics::areEqual(vectors3, vectors4, 4.0));
}

TEST(Vector2DAlgorithms, getBoundingBox_throwIfEmpty) {
  const std::vector<Mathematics::Vector2D> points;
  EXPECT_ANY_THROW(Mathematics::getBoundingBox(points));
}

TEST(Vector2DAlgorithms, getBoundingBox_onePoint) {
  const double x = -1.7;
  const double y = 4.1;
  const std::vector<Mathematics::Vector2D> points{
      {x, y},
  };
  const Mathematics::BoundingBox expectedBoundingBox{x, x, y, y};
  const auto boundingBox = Mathematics::getBoundingBox(points);
  EXPECT_EQ(expectedBoundingBox, boundingBox);
}

TEST(Vector2DAlgorithms, getBoundingBox) {
  const std::vector<Mathematics::Vector2D> points{
      {1.3, -8.8},  {0.0, 2.1}, {-4.3, 4.7},
      {-2.7, -2.1}, {4.1, 0.0}, {3.3, 1.7},
  };
  const Mathematics::BoundingBox expectedBoundingBox{-4.3, 4.1, -8.8, 4.7};
  const auto boundingBox = Mathematics::getBoundingBox(points);
  EXPECT_EQ(expectedBoundingBox, boundingBox);
}

TEST(Vector2DAlgorithms, getRandomVector) {
  const double maxVectorLength = 10.0;
  Mathematics::Vector2D lastVector(maxVectorLength, maxVectorLength);
  for (int index = 0; index < 100; ++index) {
    const auto vector = Mathematics::getRandomVector(maxVectorLength);
    EXPECT_LE(vector.getLength(), maxVectorLength);
    EXPECT_NE(lastVector, vector);
    lastVector = vector;
  }
}

TEST(Vector2DAlgorithms, getRandomVector_invalidMaxLength) {
  EXPECT_ANY_THROW(Mathematics::getRandomVector(0.0));
  EXPECT_ANY_THROW(Mathematics::getRandomVector(-1.0));
}
