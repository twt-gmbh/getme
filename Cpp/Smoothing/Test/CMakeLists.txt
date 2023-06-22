set(target smoothing_test)

set(sourcefiles
   "common_algorithms_test.cpp"
   "default_configuration_test.cpp"
   "getme_algorithms_test.cpp"
   "laplace_algorithms_test.cpp"
   "polygon_quality_min_heap_test.cpp"

   # Tests for internal algorithms and classes
   "../Source/common_algorithms.cpp"
   "../Source/polygon_quality_min_heap.cpp"
)

add_executable(${target} ${sourcefiles})

target_link_libraries(${target} 
   PRIVATE 
      GTest::gtest_main
      smoothing
      testdata
      utility
)

include(GoogleTest)
gtest_discover_tests(${target})
