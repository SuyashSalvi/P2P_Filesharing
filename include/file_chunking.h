#pragma once
#include <string>
#include <vector>

std::vector<std::string> splitFile(const std::string &filePath, size_t chunkSize);
std::string assembleChunks(const std::vector<std::string> &chunks);
std::string calculateHash(const std::string &data); // Function to calculate SHA-256 hash
std::string calculateFileHash(const std::vector<std::string> &chunks); // For the full file hash