cmake_minimum_required(VERSION 3.14)
project(CloudManagement LANGUAGES CXX)

# 1. Standard C++ et options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic)

# 2. Google Test configuration
add_subdirectory(external/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 3. Sous‑répertoire src (bibliothèque cloudsim)
add_subdirectory(src)

# 4. Activation des tests et sous‑répertoire tests
enable_testing()
add_subdirectory(tests)
