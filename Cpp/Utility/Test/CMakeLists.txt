set(target utility_test)

set(sourcefiles
   "exception_handling_test.cpp"
   "stop_watch_test.cpp"
)

add_executable(${target} ${sourcefiles})

target_link_libraries(${target} 
   PRIVATE 
      GTest::gtest_main
      utility
)

include(GoogleTest)
gtest_discover_tests(${target})
