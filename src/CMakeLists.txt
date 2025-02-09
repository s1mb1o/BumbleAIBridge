cmake_minimum_required(VERSION 3.10)

# Define the project name for the library
project(BumbleAIBridgeLibrary LANGUAGES CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Find the required Qt6 module(s)
find_package(Qt6 REQUIRED COMPONENTS Core)

# Create the BumbleAIBridge library target.
# This adds BumbleAIBridge.cpp and an internal helper file.
# Add any other source files as needed.
add_library(BumbleAIBridge
    BumbleAIBridge.cpp
    internal/helper.cpp
)

# Make the public headers available to users of this library.
# The include directory is expected to be located at the project root.
target_include_directories(BumbleAIBridge
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)

# Link against Qt6 Core since the library uses Qt classes.
target_link_libraries(BumbleAIBridge
    PUBLIC Qt6::Core
)

# Set output directories for built targets
set_target_properties(BumbleAIBridge PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# (Optional) Installation rules for the library:
install(TARGETS BumbleAIBridge
    EXPORT BumbleAIBridgeTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)