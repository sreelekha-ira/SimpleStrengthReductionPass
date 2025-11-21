cmake_minimum_required(VERSION 3.13)
project(SimpleStrengthReductionPass LANGUAGES CXX)

# Find LLVM; expect LLVM_DIR set by the system package or environment
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to conform to")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Build as plugin (module)
add_library(SimpleStrengthReductionPass MODULE
    passes/StrengthReduction.cpp
)

# No RTTI for LLVM builds
set_target_properties(SimpleStrengthReductionPass PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
)
target_link_libraries(SimpleStrengthReductionPass PRIVATE ${LLVM_LIBS})
