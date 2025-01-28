#pragma once

#include <string>
#include <map>
#include <vector>
#include <functional> // For std::hash

class DHT {
private:
    // A hash table to store key-value pairs (key -> list of peer IDs)
    std::map<std::string, std::vector<std::string>> hashTable;

    // A method to hash a key to a consistent hash value
    size_t consistentHash(const std::string &key, size_t ringSize);

public:
    // Store a key-value pair (key -> peerID)
    void put(const std::string &key, const std::string &peerID);

    // Retrieve a list of peer IDs associated with a key
    std::vector<std::string> get(const std::string &key);
};