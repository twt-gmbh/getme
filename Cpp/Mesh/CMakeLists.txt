set(target mesh)

set(sourcefiles
   "Source/mesh_quality.cpp"
   "Source/polygonal_mesh_algorithms.cpp"
   "Source/polygonal_mesh.cpp"
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
   PRIVATE 
      utility
)

add_subdirectory(Test)
