#include "../include/transfer_protocol.h"
#include <iostream>


bool TransferProtocol::prepareTransfer(const std::string &chunkID, const std::string &peerID) {
    std::lock_guard<std::mutex> lock(transferMutex);

    // Check if the chunk is already being transferred
    if (transferState.find(chunkID) != transferState.end()) {
        std::cout << "Chunk " << chunkID << " is already in the process of transfer." << std::endl;
        return false;
    }

    // Simulate the preparation process (e.g., availability check)
    std::cout << "Preparing transfer for Chunk " << chunkID << " to Peer " << peerID << std::endl;

    // Set the state to PREPARED
    transferState[chunkID] = PREPARED;
    return true;
}

void TransferProtocol::commitTransfer(const std::string &chunkID, const std::string &peerID) {
    std::lock_guard<std::mutex> lock(transferMutex);

    // Ensure the chunk is in the prepared state
    auto it = transferState.find(chunkID);
    if (it != transferState.end() && it->second == PREPARED) {
        std::cout << "Committing transfer for Chunk " << chunkID << " to Peer " << peerID << std::endl;
        it->second = COMMITTED; // Update the state to COMMITTED
    } else {
        std::cout << "Cannot commit transfer for Chunk " << chunkID << " as it is not in PREPARED state." << std::endl;
    }
}


void TransferProtocol::rollbackTransfer(const std::string &chunkID, const std::string &peerID) {
    std::lock_guard<std::mutex> lock(transferMutex);

    // Ensure the chunk is in the prepared state
    auto it = transferState.find(chunkID);
    if (it != transferState.end() && it->second == PREPARED) {
        std::cout << "Rolling back transfer for Chunk " << chunkID << " to Peer " << peerID << std::endl;
        it->second = ABORTED; // Update the state to ABORTED
    } else {
        std::cout << "Cannot rollback transfer for Chunk " << chunkID << " as it is not in PREPARED state." << std::endl;
    }
}