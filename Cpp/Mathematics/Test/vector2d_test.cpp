/*
Unit tests for 2D vectors.

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
#include "Mathematics/vector2d.h"

#include "gtest/gtest.h"

TEST(Vector2D, coordinateGetters) {
  const double x = 1.43;
  const double y = -4.23;
  const Mathematics::Vector2D vector(x, y);

  EXPECT_EQ(x, vector.getX());
  EXPECT_EQ(y, vector.getY());
}

TEST(Vector2D, plusOperator) {
  const Mathematics::Vector2D first(1.0, 2.0);
  const Mathematics::Vector2D second(-3.0, 4.0);
  const Mathematics::Vector2D expectedResult(-2.0, 6.0);

  EXPECT_EQ(expectedResult, first + second);
}

TEST(Vector2D, compoundPlusOperator) {
  Mathematics::Vector2D first(1.0, 2.0);
  const Mathematics::Vector2D second(-3.0, 4.0);
  const Mathematics::Vector2D expectedResult(-2.0, 6.0);

  first += second;

  EXPECT_EQ(expectedResult, first);
}

TEST(Vector2D, minusOperator) {
  const Mathematics::Vector2D first(1.0, 2.0);
  const Mathematics::Vector2D second(-3.0, 4.0);
  const Mathematics::Vector2D expectedResult(4.0, -2.0);

  EXPECT_EQ(expectedResult, first - second);
}

TEST(Vector2D, compoundDivisionOperator) {
  Mathematics::Vector2D first(1.0, -2.0);
  const double divisor = -2.0;
  const Mathematics::Vector2D expectedResult(-0.5, 1.0);

  first /= divisor;

  EXPECT_EQ(expectedResult, first);
}

TEST(Vector2D, equalsOperator) {
  const Mathematics::Vector2D first(1.0, 2.0);
  const Mathematics::Vector2D second(-1.0, -2.0);
  const Mathematics::Vector2D third(1.0, 4.0);
  const Mathematics::Vector2D fourth(-1.0, 2.0);

  EXPECT_TRUE(first == first);
  EXPECT_FALSE(first == second);
  EXPECT_FALSE(first == third);
  EXPECT_FALSE(first == fourth);
}

TEST(Vector2D, scaleOperator) {
  const double factor = -4.0;
  const Mathematics::Vector2D vector(1.0, -2.0);
  const Mathematics::Vector2D expectedResult(-4.0, 8.0);

  EXPECT_EQ(expectedResult, factor * vector);
}

TEST(Vector2D, divideOperator) {
  const Mathematics::Vector2D vector(1.0, -2.0);
  const double divisor = -4.0;
  const Mathematics::Vector2D expectedResult(-0.25, 0.5);

  EXPECT_EQ(expectedResult, vector / divisor);
}

TEST(Vector2D, getLengthSquared) {
  const Mathematics::Vector2D vector(4.0, -3.0);
  const double expectedLengthSquared = 25.0;

  EXPECT_EQ(expectedLengthSquared, vector.getLengthSquared());
}

TEST(Vector2D, getLength) {
  const Mathematics::Vector2D vector(-4.0, 3.0);
  const double expectedLengthSquared = 5.0;

  EXPECT_EQ(expectedLengthSquared, vector.getLength());
}
