set(target smoothing)

set(sourcefiles
   "Source/common_algorithms.cpp"
   "Source/default_configuration.cpp"
   "Source/getme_algorithms.cpp"
   "Source/getme_sequential.cpp"
   "Source/laplace_algorithms.cpp"
   "Source/polygon_quality_min_heap.cpp" 
)

add_library(${target} STATIC ${sourcefiles})

target_include_directories(${target}
   PUBLIC
      Include
   PRIVATE
      Source
)

target_link_libraries(${target} 
   PUBLIC 
      mathematics
      mesh
   PRIVATE 
      utility
)

add_subdirectory(Test)
