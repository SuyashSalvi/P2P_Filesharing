#include "../include/file_chunking.h"
#include <iostream>

int main() {
    std::string filePath = "test_file.txt"; // Path to a sample test file
    size_t chunkSize = 1024;               // Size of each chunk in bytes

    // Split the file into chunks
    std::vector<std::string> chunks = splitFile(filePath, chunkSize);

    // Display information about the chunks
    std::cout << "Number of chunks: " << chunks.size() << std::endl;
    for (size_t i = 0; i < chunks.size(); ++i) {
        std::cout << "Chunk " << i + 1 << " size: " << chunks[i].size() << " bytes" << std::endl;
    }

    return 0;
}