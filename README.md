# P2P_FileSharing

A decentralized peer-to-peer (P2P) file-sharing system that allows multiple peers to connect, share files, and communicate efficiently without relying on a central server. The project demonstrates key concepts of distributed systems, including gossip protocols, broadcasting, leader election, stabilization, and fault tolerance.

---

## 🔗 Demo Video

Check out the demo video of the project on YouTube: [P2P File Sharing Demo](https://www.youtube.com/watch?v=kVdlwpGYqok)

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

## 🚀 How to Run the Project

### Prerequisites
- **C++ Compiler**: Ensure `g++` with C++17 support is installed.
- **Boost Libraries**: Install the Boost library for `asio`.

#### Install Boost

##### On macOS
```
brew install boost
```

##### On Ubuntu/Debian
```
sudo apt-get install libboost-all-dev
```

#### Build and Run
1.	Clone the Repository:
```
git clone https://github.com/SuyashSalvi/P2P_FileSharing.git
cd P2P_FileSharing
```

2.	Compile the Project:
```
g++ -std=c++17 -o P2P src/main.cpp src/peer.cpp -Iinclude -pthread -lboost_system
```

#### 📦 Features in Action

1. Peer Joining
	•	Peers can dynamically join the network by connecting to an existing peer.
	•	The joining peer is propagated to all other peers.

2. File Chunking and Transfer
	•	Large files are split into chunks for efficient transfer.
	•	Chunks are distributed across peers, and reassembly is supported.

3. Gossip Protocol
	•	Peer information is propagated using a gossip-based approach for scalability and fault tolerance.

4. Leader Election
	•	The Ring Algorithm ensures a leader is elected among peers.

5. Stabilization
	•	Periodically checks peer health and reassigns chunks if a peer fails.

### 💡 Future Enhancements
•	Peer Discovery: Implement a more robust discovery mechanism.
•	Encryption: Add encryption for secure file transfers.
•	Web Interface: Develop a web-based UI for managing peers and file sharing.
•	Cloud Integration: Use AWS S3 for backup storage of file chunks.

### 🛠️ Technologies Used
•	C++17: Core programming language.
•	Boost.Asio: Networking library for peer communication.
•	Multithreading: Ensures efficient and non-blocking operations.
