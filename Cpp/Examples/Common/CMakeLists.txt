set(target common)

set(sourcefiles
   "Source/reporting.cpp"
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
      smoothing
      utility
)
