aux_source_directory(src SRC)
add_executable(EchoServer ${SRC})
target_link_libraries(EchoServer meshy Mswsock)
