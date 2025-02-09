cmake_minimum_required(VERSION 3.10)
project(BumbleAIBridgeAdvancedExample)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

add_executable(advanced_example main.cpp)
target_link_libraries(advanced_example BumbleAIBridge)
