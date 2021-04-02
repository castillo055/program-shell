add_library(program_shell SHARED program_shell.cpp program_shell.h terminal.cpp terminal.h)
target_link_libraries(program_shell pthread)
