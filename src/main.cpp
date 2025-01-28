#include "../include/peer.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    // Ensure proper command-line arguments
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <PeerID> <IP> <Port>" << std::endl;
        return 1;
    }

    // Parse command-line arguments
    std::string peerID = argv[1];
    std::string ipAddress = argv[2];
    int port = std::stoi(argv[3]);

    // Create the peer
    Peer peer(peerID, ipAddress, port);

    // Start listening for connections
    std::thread listenerThread(&Peer::startListening, &peer);

    // Provide an interactive menu for the peer
    while (true) {
        std::cout << "\nPeer " << peerID << " Menu:" << std::endl;
        std::cout << "1. Join Network" << std::endl;
        std::cout << "2. Broadcast Availability" << std::endl;
        std::cout << "3. Leave Network" << std::endl;
        std::cout << "4. Exit" << std::endl;

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::string targetIP;
            int targetPort;
            std::cout << "Enter target IP: ";
            std::cin >> targetIP;
            std::cout << "Enter target Port: ";
            std::cin >> targetPort;
            peer.joinNetwork(targetIP, targetPort);
        } else if (choice == 2) {
            peer.broadcastAvailability();
        } else if (choice == 3) {
            peer.leaveNetwork();
        } else if (choice == 4) {
            peer.leaveNetwork();
            break;
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    listenerThread.join();
    return 0;
}