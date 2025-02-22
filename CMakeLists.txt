cmake_minimum_required(VERSION 3.10)
project(tosparser VERSION 0.1.0 LANGUAGES C)

# Set C17 standard and enforce strict ISO compliance
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)  # Disables compiler-specific extensions

# Include directories
include_directories(include)
# include_directories(external/nuklear-4.12.4)  # Add Nuklear
# include_directories(external/glad33core/include)
# include_directories(external/stb-master)

# find_package(glfw3 REQUIRED)
# find_package(GLEW REQUIRED)

# Enable all warnings and additional info messages
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall -Wextra -Wpedantic -Wshadow -Wconversion -g
        -std=c17 -Wstrict-prototypes -Wold-style-definition
        -Wmissing-prototypes -Wmissing-declarations
        -Werror  # Treat warnings as errors
    )
elseif (MSVC)
    add_compile_options(/W4)
endif()

# Enable static linking
# set(CMAKE_EXE_LINKER_FLAGS "-static")

# Add executable
add_executable(tosparser 
# external/glad33core/src/glad.c 
source/main.c)

# Comments this options if gonna build as release
# Enable sanitizers if supported
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(tosparser PRIVATE -fsanitize=address,undefined,leak)
    target_link_options(tosparser PRIVATE -fsanitize=address,undefined,leak)
endif()

# Link libraries
if(UNIX)
    target_link_libraries(
        tosparser
        # glfw
        # GL
        z
        m
        pthread
        dl
    )
endif()
