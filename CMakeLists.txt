cmake_minimum_required(VERSION 3.15)
project(psammite-vm VERSION 0.1.0 LANGUAGES C)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Debug', as none was specified.")
  set(CMAKE_BUILD_TYPE "Debug")
endif()


add_library(psammite_compiler_config INTERFACE)
target_compile_options(psammite_compiler_config INTERFACE
    # MAC & LINUX (GCC / Clang)
    $<$<AND:$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>,$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>>:-Wall;-Wextra;-Werror>

    # WINDOWS (MSVC)
    $<$<AND:$<C_COMPILER_ID:MSVC>,$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>>:/W4;/WX>
)

add_library(psammite_vm_lib STATIC  src/psammite.c)
target_include_directories(psammite_vm_lib PUBLIC include)
target_link_libraries(psammite_vm_lib PRIVATE psammite_compiler_config)
add_executable(psammite_vm  src/main.c)
target_link_libraries(psammite_vm PRIVATE psammite_compiler_config psammite_vm_lib)
