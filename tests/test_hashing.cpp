#include "../include/file_chunking.h"
#include <iostream>

int main() {
    std::string filePath = "test_file.txt"; // Path to the sample test file
    size_t chunkSize = 1024;               // Size of each chunk in bytes

    // Split the file into chunks
    std::vector<std::string> chunks = splitFile(filePath, chunkSize);

    // Display chunk hashes
    std::cout << "Chunk Hashes:" << std::endl;
    for (size_t i = 0; i < chunks.size(); ++i) {
        std::string chunkHash = calculateHash(chunks[i]);
        std::cout << "Chunk " << i + 1 << " Hash: " << chunkHash << std::endl;
    }

    // Calculate the hash of the entire file
    std::string fileHash = calculateFileHash(chunks);
    std::cout << "Full File Hash: " << fileHash << std::endl;

    return 0;
}