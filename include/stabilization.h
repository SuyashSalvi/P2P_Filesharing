#pragma once
#include <string>
#include <map>
#include <set>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>

class Stabilization
{
private:
    std::string peerID;                                      // Current peer's ID
    std::vector<std::string> neighbors;                      // Neighboring peers
    std::map<std::string, bool> peerHealth;                  // Tracks the health of peers (true = healthy, false = failed)
    std::map<std::string, std::set<std::string>> peerChunks; // Peer-to-chunk assignments
    mutable std::mutex stabilizationMutex;                   // Mutex for thread safety
    std::condition_variable cv;                              // Condition variable for thread signaling
    bool running;                                            // Flag to control the stabilization thread

    void checkNeighbors();                              // Periodically check neighbor health
    void reassignChunks(const std::string &failedPeer); // Reassign chunks of a failed peer

public:
    Stabilization(const std::string &peerID, const std::vector<std::string> &neighbors);

    void startStabilization();                                            // Start the stabilization process
    void stopStabilization();                                             // Stop the stabilization process
    void markPeerUnhealthy(const std::string &peerID);                    // Simulate a peer failure
    void addChunk(const std::string &peerID, const std::string &chunkID); // Assign a chunk to a peer
    void displayState() const;                                            // Display the current state of the system
    void balanceLoad(); // Redistribute chunks for load balancing
};