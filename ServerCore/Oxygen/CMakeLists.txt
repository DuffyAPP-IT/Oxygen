cmake_minimum_required(VERSION 3.15)
project(Oxygen C)

set(CMAKE_C_STANDARD 99)

add_executable(Oxygen main.c vmManage.c vmManage.h iLibX.c iLibX.h serverInit.c)