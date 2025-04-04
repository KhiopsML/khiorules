cmake_minimum_required(VERSION 3.16.3)

project(khiorules CXX)

# Use Khiops 
include(FetchContent)
FetchContent_Declare(
    khiops
    GIT_REPOSITORY "https://github.com/KhiopsML/khiops.git"
    GIT_TAG "10.3.0")

FetchContent_MakeAvailable(khiops)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib/>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib/>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin/>)
message(STATUS "Executables will be stored in ${CMAKE_BINARY_DIR}/bin/")
message(STATUS "Libraries will be stored in ${CMAKE_BINARY_DIR}/lib/")

file(GLOB files ${PROJECT_SOURCE_DIR}/src/*cpp ${PROJECT_SOURCE_DIR}/src/*cc ${PROJECT_SOURCE_DIR}/src/*h )
add_executable(khiorules ${files} )
set_khiops_options(khiorules)

# Link with Khiops libraries
target_link_libraries(khiorules KMDRRuleLibrary KWLearningProblem)
