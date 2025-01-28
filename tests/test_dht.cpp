#include "../include/dht.h"
#include <iostream>

int main() {
    DHT dht;

    // Simulate adding keys to the DHT
    dht.put("file1", "peer1");
    dht.put("file2", "peer2");
    dht.put("file1", "peer3"); // Adding another peer to an existing key

    // Retrieve keys from the DHT
    std::vector<std::string> peersForFile1 = dht.get("file1");
    std::cout << "Peers for file1: ";
    for (const auto &peer : peersForFile1) {
        std::cout << peer << " ";
    }
    std::cout << std::endl;

    std::vector<std::string> peersForFile2 = dht.get("file2");
    std::cout << "Peers for file2: ";
    for (const auto &peer : peersForFile2) {
        std::cout << peer << " ";
    }
    std::cout << std::endl;

    // Try retrieving a key that doesn't exist
    std::vector<std::string> peersForFile3 = dht.get("file3");

    return 0;
}