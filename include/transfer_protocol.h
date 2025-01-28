#pragma once
#include <string>
#include <map>
#include <mutex>

// Simulated state for tracking transfer preparation status
enum TransferState { PREPARED, COMMITTED, ABORTED };

class TransferProtocol {
private:
    // A map to track the transfer state of chunks
    std::map<std::string, TransferState> transferState;
    std::mutex transferMutex; // Mutex to ensure thread safety

public:
    // Prepare the transfer of a chunk to a peer
    bool prepareTransfer(const std::string &chunkID, const std::string &peerID);

    // Commit the transfer of a chunk
    void commitTransfer(const std::string &chunkID, const std::string &peerID);

    // Rollback the transfer of a chunk
    void rollbackTransfer(const std::string &chunkID, const std::string &peerID);
};