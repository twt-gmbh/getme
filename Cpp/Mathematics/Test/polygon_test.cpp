// Unit tests for polygonal elements.
#include "Mathematics/polygon.h"

#include "gtest/gtest.h"

#include <vector>

TEST(Polygon, constructorThrows) {
  EXPECT_ANY_THROW(Mathematics::Polygon({}));
  EXPECT_ANY_THROW(Mathematics::Polygon({0}));
  EXPECT_ANY_THROW(Mathematics::Polygon({0, 1}));
  EXPECT_NO_THROW(Mathematics::Polygon({0, 1, 2}));
  EXPECT_ANY_THROW(Mathematics::Polygon({0, 1, 1}));
  EXPECT_ANY_THROW(Mathematics::Polygon({0, 1, 2, 3, 4, 0}));
}

TEST(Polygon, basicGetters) {
  const std::vector<std::size_t> nodeIndices{17, 0, 12, 14, 5, 3, 1};
  const Mathematics::Polygon polygon(nodeIndices);

  EXPECT_EQ(nodeIndices, polygon.getNodeIndices());
  EXPECT_EQ(nodeIndices.size(), polygon.getNumberOfNodes());
}

TEST(Polygon, nodeIndexGetters) {
  const std::vector<std::size_t> nodeIndices{17, 0, 12, 14, 5, 3, 1};
  const Mathematics::Polygon polygon(nodeIndices);

  for (std::size_t nodeNumber = 0; nodeNumber < nodeIndices.size();
       ++nodeNumber) {
    EXPECT_EQ(nodeIndices.at(nodeNumber), polygon.getNodeIndex(nodeNumber));
    EXPECT_EQ(nodeIndices.at((nodeNumber + nodeIndices.size() - 1)
                             % nodeIndices.size()),
              polygon.getPredecessorNodeIndex(nodeNumber));
    EXPECT_EQ(nodeIndices.at((nodeNumber + 1) % nodeIndices.size()),
              polygon.getSuccessorNodeIndex(nodeNumber));
  }
}

TEST(Polygon, equalsOperator) {
  const Mathematics::Polygon first({1, 2, 3});
  const Mathematics::Polygon second(
      {2, 3, 1});  // Cyclic shift detection is not supported.
  const Mathematics::Polygon third({1, 2, 3, 4});

  EXPECT_EQ(first, first);
  EXPECT_NE(first, second);
  EXPECT_NE(first, third);
}
