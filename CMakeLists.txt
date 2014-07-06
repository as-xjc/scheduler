aux_source_directory(. schedule_srcs)
add_library(schedule_module ${schedule_srcs})
add_dependencies(schedule_module depend_uv)
