cmake_minimum_required(VERSION 3.10)

# Define the project name for the library
project(BumbleAIBridge LANGUAGES CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Find the required Qt6 module(s)
find_package(Qt6 REQUIRED COMPONENTS Core Network)

# Specify the include directories for MOC
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/BumbleAIBridge
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/internal
)
# Set the directory of the current CMakeLists.txt file
set(CURRENT_CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR})


# Create the BumbleAIBridge library target.
# This adds BumbleAIBridge.cpp and an internal helper file.
# Add any other source files as needed.
# BUMBLEAIBRIDGE_LIBRARY_TYPE
add_library(BumbleAIBridge ${BUMBLEAIBRIDGE_LIBRARY_TYPE}
../include/BumbleAIBridge/AIClient.h
AIBridge.cpp
AIClient.cpp
AIClient_p.h
AIFactory.cpp
internal/helper.cpp
internal/OllamaClient.h
internal/OllamaClient.cpp
internal/OpenAIClient.h
internal/OpenAIClient.cpp
internal/LLMOutputProcessor.h 
internal/LLMOutputProcessor_p.h 
internal/LLMOutputProcessor.cpp

)

# Make the public headers available to users of this library.
# The include directory is expected to be located at the project root.
target_include_directories(BumbleAIBridge
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

# Link against Qt6 Core since the library uses Qt classes.
target_link_libraries(BumbleAIBridge
    PUBLIC Qt6::Core Qt6::Network
)

# Specify include directories for the target
target_include_directories(BumbleAIBridge
PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/BumbleAIBridge
PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/internal
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