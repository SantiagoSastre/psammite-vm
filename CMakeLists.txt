cmake_minimum_required(VERSION 3.15)
project(psammite-vm VERSION 0.1.0 LANGUAGES C)


set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Debug', as none was specified.")
  set(CMAKE_BUILD_TYPE "Debug")
endif()



add_executable(psammite_vm src/psammite.c)

target_compile_options(psammite_vm PRIVATE
    # MAC & LINUX (GCC / Clang)
    $<$<AND:$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>,$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>>:-Wall;-Wextra;-Werror>

    # WINDOWS (MSVC)
    $<$<AND:$<C_COMPILER_ID:MSVC>,$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>>:/W4;/WX>
)
