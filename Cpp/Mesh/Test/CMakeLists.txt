set(target mesh_test)

set(sourcefiles
   "mesh_quality_test.cpp"
   "polygonal_mesh_algorithms_test.cpp"
   "polygonal_mesh_test.cpp"
)

add_executable(${target} ${sourcefiles})

target_link_libraries(${target} 
   PRIVATE 
      GTest::gtest_main
      mathematics
      mesh
      testdata
      utility
)

include(GoogleTest)
gtest_discover_tests(${target})
