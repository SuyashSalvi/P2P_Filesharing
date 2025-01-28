#include "../include/stabilization.h"
#include <iostream>
#include <thread>

int main() {
    // Define the ring of neighbors
    std::vector<std::string> neighbors = {"peer1", "peer2", "peer3"};

    // Create a Stabilization object
    Stabilization stabilization("peer0", neighbors);

    // Assign chunks to peers
    stabilization.addChunk("peer1", "chunk1");
    stabilization.addChunk("peer1", "chunk2");
    stabilization.addChunk("peer2", "chunk3");
    stabilization.addChunk("peer3", "chunk4");
    stabilization.addChunk("peer3", "chunk5");

    // Start the stabilization process
    stabilization.startStabilization();

    // Simulate a peer failure
    std::this_thread::sleep_for(std::chrono::seconds(2));
    stabilization.markPeerUnhealthy("peer2");

    // Wait to allow stabilization to handle the failure
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Simulate another peer failure
    stabilization.markPeerUnhealthy("peer3");

    // Wait to allow stabilization to handle the failure
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Display the system state
    stabilization.displayState();

    // Stop the stabilization process
    stabilization.stopStabilization();

    return 0;
}