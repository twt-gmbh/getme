set(target utility)

set(sourcefiles
   "Source/exception_handling.cpp"
   "Source/stop_watch.cpp"
)

add_library(${target} STATIC ${sourcefiles})

target_include_directories(${target}
   PUBLIC
      Include
   PRIVATE
      Source
)

add_subdirectory(Test)
