#pragma once
#include <string>
#include <vector>
#include <map>

class RoutingTable {
private:
    size_t peerID;               // The current peer's ID on the ring
    size_t ringSize;             // The size of the hash ring
    size_t successorID;          // The successor peer's ID
    size_t predecessorID;        // The predecessor peer's ID
    std::map<size_t, std::string> peerMap; // Map of peerID to peer address (e.g., IP:Port)

public:
    RoutingTable(size_t id, size_t ringSize);

    void setSuccessor(size_t successorID, const std::string &address);
    void setPredecessor(size_t predecessorID, const std::string &address);
    std::string findResponsiblePeer(const std::string &key) const;

    size_t getSuccessorID() const;
    size_t getPredecessorID() const;
};