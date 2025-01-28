#include "../include/routing.h"
#include <functional> // For std::hash
#include <iostream>
#include <sstream>


RoutingTable::RoutingTable(size_t id, size_t ringSize)
    : peerID(id), ringSize(ringSize), successorID(id), predecessorID(id) {}


void RoutingTable::setSuccessor(size_t id, const std::string &address) {
    successorID = id;
    peerMap[id] = address;
    std::cout << "Successor set to Peer " << id << " (" << address << ")" << std::endl;
}

void RoutingTable::setPredecessor(size_t id, const std::string &address) {
    predecessorID = id;
    peerMap[id] = address;
    std::cout << "Predecessor set to Peer " << id << " (" << address << ")" << std::endl;
}

std::string RoutingTable::findResponsiblePeer(const std::string &key) const {
    // Hash the key to find its position on the ring
    std::hash<std::string> hasher;
    size_t keyHash = hasher(key) % ringSize;

    // Debugging output
    std::cout << "Key: " << key << " | Hash: " << keyHash 
              << " | PredecessorID: " << predecessorID 
              << " | PeerID: " << peerID 
              << " | SuccessorID: " << successorID << std::endl;

    // Check if the current peer is responsible
    if ((predecessorID < peerID && keyHash > predecessorID && keyHash <= peerID) || 
        (predecessorID > peerID && (keyHash > predecessorID || keyHash <= peerID))) {
        return "Current Peer (" + std::to_string(peerID) + ")";
    }

    // Forward the request to the successor
    auto it = peerMap.find(successorID);
    if (it != peerMap.end()) {
        return "Forwarding to Successor Peer " + std::to_string(successorID) + " (" + it->second + ")";
    }

    return "No responsible peer found.";
}

size_t RoutingTable::getSuccessorID() const {
    return successorID;
}

size_t RoutingTable::getPredecessorID() const {
    return predecessorID;
}