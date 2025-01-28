#include "../include/file_chunking.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

std::vector<std::string> splitFile(const std::string &filePath, size_t chunkSize) {
    std::vector<std::string> chunks;

    // Open the file in binary mode
    std::ifstream inputFile(filePath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Could not open file " << filePath << " for reading." << std::endl;
        return chunks; // Return an empty vector on failure
    }

    // Read the file chunk by chunk
    while (!inputFile.eof()) {
        std::string chunk(chunkSize, '\0'); // Create a buffer of chunkSize
        inputFile.read(&chunk[0], chunkSize); // Read up to chunkSize bytes into the buffer
        size_t bytesRead = inputFile.gcount(); // Get the actual number of bytes read

        // Resize the chunk to the actual number of bytes read (if less than chunkSize)
        chunk.resize(bytesRead);

        // Add the chunk to the vector
        if (!chunk.empty()) {
            chunks.push_back(chunk);
        }
    }

    inputFile.close();
    std::cout << "File successfully split into " << chunks.size() << " chunks." << std::endl;

    return chunks;
}


std::string calculateHash(const std::string &data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

    // Convert the hash to a hexadecimal string
    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return oss.str();
}

std::string calculateFileHash(const std::vector<std::string> &chunks) {
    std::string combinedHashInput;

    // Concatenate the hashes of all chunks
    for (const auto &chunk : chunks) {
        combinedHashInput += calculateHash(chunk);
    }

    // Compute the SHA-256 hash of the concatenated hashes
    return calculateHash(combinedHashInput);
}