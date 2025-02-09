cmake_minimum_required(VERSION 3.10)
project(BumbleAIBridgeBasicExample)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

add_executable(basic_example main.cpp)
# Assuming the BumbleAIBridge library is installed or available via add_subdirectory
target_link_libraries(basic_example BumbleAIBridge)
