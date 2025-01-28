#include "../include/dht.h"
#include <iostream>
#include <algorithm> // For std::find

size_t DHT::consistentHash(const std::string &key, size_t ringSize) {
    std::hash<std::string> hasher;
    return hasher(key) % ringSize; // Map hash value to a position on the ring
}

void DHT::put(const std::string &key, const std::string &peerID) {
    // Check if the key already exists in the hash table
    if (hashTable.find(key) == hashTable.end()) {
        hashTable[key] = std::vector<std::string>(); // Initialize an empty vector
    }

    // Add the peerID to the list of peers for this key if it's not already present
    if (std::find(hashTable[key].begin(), hashTable[key].end(), peerID) == hashTable[key].end()) {
        hashTable[key].push_back(peerID);
        std::cout << "Key " << key << " added to DHT by Peer " << peerID << std::endl;
    } else {
        std::cout << "Peer " << peerID << " already associated with key " << key << std::endl;
    }
}

std::vector<std::string> DHT::get(const std::string &key) {
    if (hashTable.find(key) != hashTable.end()) {
        return hashTable[key];
    } else {
        std::cout << "Key " << key << " not found in DHT." << std::endl;
        return std::vector<std::string>(); // Return an empty vector if the key is not found
    }
}