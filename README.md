# P2P_FileSharing

A decentralized peer-to-peer (P2P) file-sharing system that allows multiple peers to connect, share files, and communicate efficiently without relying on a central server. The project demonstrates key concepts of distributed systems, including gossip protocols, broadcasting, leader election, stabilization, and fault tolerance.

---

## 🔗 Demo Video

Check out the demo video of the project on YouTube: [P2P File Sharing Demo](https://youtu.be/<your-video-id>)

---

## 🌟 Features

### 🖥️ Core Functionalities
1. **Peer-to-Peer Network**:
   - Dynamic joining and leaving of peers in the network.
   - No central server dependency.

2. **File Chunking and Transfer**:
   - Splits files into chunks and distributes them across peers.
   - Efficient reassembly of chunks.

3. **Gossip Protocol**:
   - Propagates peer information (e.g., joins, leaves) to all peers in the network.

4. **Leader Election**:
   - Uses the Ring Algorithm to elect a leader among peers.

5. **Stabilization**:
   - Periodically checks the health of peers and reassigns file chunks if a peer fails.

6. **Broadcasting**:
   - Efficiently notifies all peers about network changes or updates.

7. **Fault Tolerance**:
   - Handles dynamic peer failures and ensures the network stabilizes by redistributing responsibilities.

---

## 📂 Project Structure

P2P_FileSharing/
├── include/                # Header files for the project
│   ├── peer.h              # Declaration of Peer class
│   ├── file_chunking.h     # Declaration of file chunking functions
│   ├── leader_election.h   # Declaration of leader election functions
│   ├── stabilization.h     # Declaration of stabilization functions
├── src/                    # Source files
│   ├── main.cpp            # Entry point of the program
│   ├── peer.cpp            # Implementation of Peer class
│   ├── file_chunking.cpp   # File chunking implementation
│   ├── leader_election.cpp # Leader election implementation
│   ├── stabilization.cpp   # Stabilization logic
├── tests/                  # Test files
│   ├── test_file_chunking.cpp
│   ├── test_leader_election.cpp
│   ├── test_stabilization.cpp
├── test_file.txt           # Sample file for testing chunking
├── README.md               # Project documentation
└── CMakeLists.txt          # Build system configuration (if using CMake)

---

## 🚀 How to Run the Project

### Prerequisites
- **C++ Compiler**: Ensure `g++` with C++17 support is installed.
- **Boost Libraries**: Install the Boost library for `asio`.
