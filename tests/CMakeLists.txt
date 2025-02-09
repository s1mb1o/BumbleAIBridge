cmake_minimum_required(VERSION 3.10)
project(BumbleAIBridgeTests)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Locate the Qt6 Test module (adjust version/path as needed)
find_package(Qt6 REQUIRED COMPONENTS Core Test)

include_directories(.)

# Optionally, add include directories if you have headers in a custom location.

# Specify the source files for the executable
add_executable(BumbleAIBridgeTest 
BumbleAIBridgeTest.cpp
BumbleAIBridgeTest.h
main.cpp 
)

# Add the subdirectory containing your library to the linker search path.
target_link_directories(BumbleAIBridgeTest PRIVATE ${CMAKE_BINARY_DIR}/lib)

# Link the executable with the Qt6 Test module and the static library located in build/src/
target_link_libraries(BumbleAIBridgeTest Qt6::Core Qt6::Test BumbleAIBridge)
