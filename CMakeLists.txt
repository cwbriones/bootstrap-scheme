# Specify the version as well as the language
cmake_minimum_required(VERSION 2.7)
# Name of project
project(bootstrap-scheme)

# Send flags to the compiler
# Set the compiler
SET(CMAKE_CXX_COMPILER "g++")
# Set compile flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-std=c++11)

# This tells Cmake to compile and name it
set(EXECUTABLE_NAME "scheme")
include_directories("${PROJECT_SOURCE_DIR}/include")
set(SOURCE_FILES
        src/SchemeObject.cpp
        src/SchemeObjectCreator.cpp
        src/InputOutput.cpp
        src/Reader.cpp
        src/Scheme.cpp
        src/SchemeEvaluator.cpp
        src/SchemeGarbageCollector.cpp
        src/SchemePrimProcedure.cpp
        src/StringProcedures.cpp
        src/TypeConversions.cpp
        src/VectorProcedures.cpp
        src/Environment.cpp
        src/SchemeWriter.cpp
        src/main.cpp
)
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
