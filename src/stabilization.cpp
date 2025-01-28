#include "../include/stabilization.h"
#include <iostream>
#include <chrono>
#include <algorithm>

Stabilization::Stabilization(const std::string &peerID, const std::vector<std::string> &neighbors)
    : peerID(peerID), neighbors(neighbors), running(false) {
    // Initialize all neighbors as healthy
    for (const auto &neighbor : neighbors) {
        peerHealth[neighbor] = true;
    }
}

void Stabilization::startStabilization() {
    running = true;
    std::thread([this]() {
        while (running) {
            checkNeighbors();
            std::this_thread::sleep_for(std::chrono::seconds(5)); // Periodic check every 5 seconds
            if (!running) break; // Exit if the stabilization process is stopped
        }
    }).detach();
}

void Stabilization::stopStabilization() {
    running = false;
    cv.notify_all(); // Wake up any waiting threads
}

void Stabilization::checkNeighbors() {
    std::lock_guard<std::mutex> lock(stabilizationMutex);

    for (const auto &neighbor : neighbors) {
        if (!peerHealth[neighbor]) { // If a neighbor is unhealthy
            std::cout << "[DEBUG] Neighbor " << neighbor << " is unavailable. Reassigning chunks." << std::endl;
            reassignChunks(neighbor); // Reassign its chunks
        }
    }
    std::cout << "[DEBUG] Finished checking neighbors." << std::endl;
}

void Stabilization::reassignChunks(const std::string &failedPeer) {
    std::lock_guard<std::mutex> lock(stabilizationMutex);

    std::cout << "[DEBUG] Reassigning chunks of failed Peer " << failedPeer << "." << std::endl;

    // Remove the failed peer's chunk assignments
    auto it = peerChunks.find(failedPeer);
    if (it != peerChunks.end()) {
        std::cout << "[DEBUG] Peer " << failedPeer << " had chunks: ";
        for (const auto &chunk : it->second) {
            std::cout << chunk << " ";
        }
        std::cout << std::endl;
        peerChunks.erase(it);
    } else {
        std::cout << "[DEBUG] Peer " << failedPeer << " had no chunks to reassign." << std::endl;
    }

    // Trigger load balancing to evenly distribute remaining chunks
    std::cout << "[DEBUG] Triggering load balancing after failure of " << failedPeer << "." << std::endl;
    balanceLoad();
}

void Stabilization::markPeerUnhealthy(const std::string &peerID) {
    std::lock_guard<std::mutex> lock(stabilizationMutex);
    peerHealth[peerID] = false;
    std::cout << "Marked Peer " << peerID << " as unhealthy." << std::endl;
}

void Stabilization::addChunk(const std::string &peerID, const std::string &chunkID) {
    std::lock_guard<std::mutex> lock(stabilizationMutex);
    peerChunks[peerID].insert(chunkID);
    std::cout << "Assigned Chunk " << chunkID << " to Peer " << peerID << "." << std::endl;
}

void Stabilization::displayState() const {
    std::lock_guard<std::mutex> lock(stabilizationMutex);
    std::cout << "Current System State:" << std::endl;

    for (const auto &peer : neighbors) {
        std::cout << "Peer " << peer << " [Health: " << (peerHealth.at(peer) ? "Healthy" : "Unhealthy") << "] has chunks: ";
        auto it = peerChunks.find(peer);
        if (it != peerChunks.end()) {
            for (const auto &chunk : it->second) {
                std::cout << chunk << " ";
            }
        }
        std::cout << std::endl;
    }
}

void Stabilization::balanceLoad() {
    std::lock_guard<std::mutex> lock(stabilizationMutex);

    // Collect all chunks from all peers
    std::vector<std::string> allChunks;
    for (const auto &peer : peerChunks) {
        for (const auto &chunk : peer.second) {
            allChunks.push_back(chunk);
        }
    }

    // Log all chunks collected
    std::cout << "[DEBUG] Collected Chunks: ";
    for (const auto &chunk : allChunks) {
        std::cout << chunk << " ";
    }
    std::cout << std::endl;

    // Clear all current assignments
    for (auto &peer : peerChunks) {
        peer.second.clear();
    }

    // Get a list of healthy peers
    std::vector<std::string> healthyPeers;
    for (const auto &neighbor : neighbors) {
        if (peerHealth[neighbor]) {
            healthyPeers.push_back(neighbor);
        }
    }

    // Log healthy peers
    std::cout << "[DEBUG] Healthy Peers: ";
    for (const auto &peer : healthyPeers) {
        std::cout << peer << " ";
    }
    std::cout << std::endl;

    if (healthyPeers.empty()) {
        std::cerr << "[ERROR] No healthy peers available for load balancing. Chunks cannot be reassigned." << std::endl;
        return;
    }

    // Distribute chunks evenly among healthy peers
    size_t peerIndex = 0;
    for (const auto &chunk : allChunks) {
        if (peerIndex >= healthyPeers.size()) {
            std::cerr << "[ERROR] Peer index out of bounds. This should not happen!" << std::endl;
            break;
        }
        peerChunks[healthyPeers[peerIndex]].insert(chunk);
        std::cout << "[DEBUG] Reassigned Chunk " << chunk << " to Peer " << healthyPeers[peerIndex] << "." << std::endl;
        peerIndex = (peerIndex + 1) % healthyPeers.size(); // Move to the next peer in a round-robin manner
    }

    std::cout << "Load balancing completed. Chunks redistributed among healthy peers." << std::endl;
}