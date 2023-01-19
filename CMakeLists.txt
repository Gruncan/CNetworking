cmake_minimum_required(VERSION 3.24)
project(NetworkProgramming)

set(CMAKE_CXX_STANDARD 17)

add_executable(Client client/client.c)
add_executable(Server server/server.c)
