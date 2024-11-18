cmake_minimum_required(VERSION 3.10)
project(audioeqdriver_project)

# Set the C standard
set(CMAKE_C_STANDARD 11)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the source files
add_executable(audioeqdriver
    src/main.c
    src/audio_processing.c
)

# Set output directory to the project root
set_target_properties(audioeqdriver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Link the ALSA library
find_library(ALSA_LIB asound REQUIRED)
target_link_libraries(audioeqdriver ${ALSA_LIB})
