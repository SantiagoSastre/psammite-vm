cmake_minimum_required(VERSION 3.15)
project(psammite-vm VERSION 0.1.0 LANGUAGES C)


set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(psammite_vm src/psammite.c)
