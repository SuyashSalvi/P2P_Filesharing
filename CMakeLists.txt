cmake_minimum_required(VERSION 3.10)
project(P2P_FileSharing)

set(CMAKE_CXX_STANDARD 17)

# Add source files
add_executable(P2P_FileSharing
    src/main.cpp
    src/peer.cpp
    src/file_chunking.cpp
    src/dht.cpp
    src/gossip.cpp
    src/leader_election.cpp
    src/transfer_protocol.cpp
)

# Find Boost with required components
find_package(Boost REQUIRED COMPONENTS system)

# Include directories
target_include_directories(P2P_FileSharing PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(P2P_FileSharing PUBLIC 
    Boost::system
    ${Boost_LIBRARIES}
) 