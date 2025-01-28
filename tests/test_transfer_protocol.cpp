#include "../include/transfer_protocol.h"
#include <iostream>

int main() {
    TransferProtocol tp;

    // Simulate a chunk transfer process
    std::string chunkID = "chunk1";
    std::string peerID = "peer1";

    // Step 1: Prepare transfer
    if (tp.prepareTransfer(chunkID, peerID)) {
        // Step 2: Commit transfer
        tp.commitTransfer(chunkID, peerID);
    } else {
        // If preparation fails, rollback
        tp.rollbackTransfer(chunkID, peerID);
    }

    // Test rollback for an unprepared chunk
    std::string chunkID2 = "chunk2";
    tp.rollbackTransfer(chunkID2, peerID);

    return 0;
}