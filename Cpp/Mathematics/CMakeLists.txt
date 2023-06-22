set(target mathematics)

set(sourcefiles
   "Source/bounding_box.cpp"
   "Source/generalized_polygon_transformation.cpp"
   "Source/polygon_algorithms.cpp"
   "Source/polygon.cpp"   
   "Source/vector2d_algorithms.cpp"
)

add_library(${target} STATIC ${sourcefiles})

target_include_directories(${target}
   PUBLIC
      Include
   PRIVATE
      Source
)

target_link_libraries(${target} 
   PRIVATE 
      utility
)

add_subdirectory(Test)
