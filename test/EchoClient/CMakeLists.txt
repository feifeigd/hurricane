aux_source_directory(src SRC)
add_executable(EchoClient ${SRC})
target_link_libraries(EchoClient meshy Mswsock fmt::fmt)
