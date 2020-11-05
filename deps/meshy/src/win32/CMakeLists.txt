cmake_minimum_required(VERSION 3.10)
project(net)

add_executable(news_sender_win news_sender_win.cpp)
target_link_libraries(news_sender_win ws2_32)
add_executable(news_receiver_win news_receiver_win.cpp)
target_link_libraries(news_receiver_win ws2_32)
