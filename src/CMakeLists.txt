add_library(program_shell SHARED program_shell.cpp program_shell.h terminal.cpp terminal.h ../include/program_shell.h)
target_link_libraries(program_shell pthread)

install(TARGETS program_shell DESTINATION ${CMAKE_SOURCE_DIR}/lib)
install(FILES ../include/program_shell.h DESTINATION include)
install(TARGETS program_shell DESTINATION lib)
