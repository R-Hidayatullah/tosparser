cmake_minimum_required(VERSION 3.10)
project(tosparser VERSION 0.1.0 LANGUAGES C)

# Set C standard to C11 with strict ISO compliance
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)  # Ensures strict ISO C compliance

# Include directories
include_directories(include)
include_directories(external/nuklear-4.12.4)  # Add Nuklear
# include_directories(external/glad33core/include)
include_directories(external/stb-master)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# # Enable all warnings and additional info messages
# if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
#     add_compile_options(-Wall -Wextra -Wpedantic -Wstrict-prototypes -Wconversion)
# elseif (MSVC)
#     add_compile_options(/W4)
# endif()

# Enable static linking
# set(CMAKE_EXE_LINKER_FLAGS "-static")

# Add executable
add_executable(tosparser 
# external/glad33core/src/glad.c 
source/main.c)

# Link libraries
target_link_libraries(
    tosparser
    glfw
    GLEW::GLEW
    GL
    z
    m
)
