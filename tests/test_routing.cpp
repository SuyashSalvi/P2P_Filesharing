#include "../include/routing.h"
#include <iostream>

int main() {
    // Define the hash ring size
    size_t ringSize = 1024;

    // Create a routing table for Peer 100
    RoutingTable rt(100, ringSize);

    // Set the successor and predecessor
    rt.setSuccessor(200, "192.168.1.2:8080");
    rt.setPredecessor(50, "192.168.1.1:8080");

    // Test finding the responsible peer for various keys
    std::string key1 = "file1";
    std::string key2 = "file2";
    std::string key3 = "file3";

    std::cout << "Key: " << key1 << " -> " << rt.findResponsiblePeer(key1) << std::endl;
    std::cout << "Key: " << key2 << " -> " << rt.findResponsiblePeer(key2) << std::endl;
    std::cout << "Key: " << key3 << " -> " << rt.findResponsiblePeer(key3) << std::endl;

    return 0;
}