set(target example_platonicmeshes)

set(sourcefiles
   "main.cpp"
)

add_executable(${target} ${sourcefiles})

target_link_libraries(${target} 
   PRIVATE 
      smoothing
      common
)

file(GLOB meshFileList "${PROJECT_SOURCE_DIR}/../Meshes/platonic*.mesh")
add_custom_command(TARGET ${target} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_if_different
   ${meshFileList}
   $<TARGET_FILE_DIR:${target}>
)
