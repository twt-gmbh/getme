set(target mathematics_test)

set(sourcefiles
   "bounding_box_test.cpp"
   "generalized_polygon_transformation_test.cpp"
   "mathematics_test_utilities_test.cpp"
   "mathematics_test_utilities.cpp"
   "polygon_algorithms_test.cpp"
   "polygon_test.cpp"
   "vector2d_algorithms_test.cpp"
   "vector2d_test.cpp"
)

add_executable(${target} ${sourcefiles})

target_link_libraries(${target} 
   PRIVATE 
      GTest::gtest_main
      mathematics
      utility
)

include(GoogleTest)
gtest_discover_tests(${target})
