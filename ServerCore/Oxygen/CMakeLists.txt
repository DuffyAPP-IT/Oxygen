cmake_minimum_required(VERSION 3.16)
project(oxygenCore C)

set(CMAKE_C_STANDARD 99)

add_executable(oxygenCore main.c iLibX.c iLibX.h vmManage.c vmManage.h)