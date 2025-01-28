#pragma once

#include <string>
#include <set>
#include <mutex>
#include <boost/asio.hpp>
#include <iostream>

class Peer {
private:
    std::string peerID;                       // Unique ID for the peer
    std::string ipAddress;                    // Peer’s IP address
    int port;                                 // Listening port

    boost::asio::io_context ioContext;        // Boost ASIO IO context
    boost::asio::ip::tcp::acceptor acceptor;  // TCP Acceptor for incoming connections
    boost::asio::ip::tcp::socket socket;      // TCP Socket for outgoing connections

    std::set<std::pair<std::string, int>> peerList; // List of known peers (IP, port)
    std::mutex peerListMutex;                 // Mutex for thread-safe access to peerList
    std::set<std::string> processedMessages; // Tracks message IDs to prevent rebroadcast loops

public:
    Peer(const std::string &id, const std::string &ip, int portNumber);

    void startListening();                    // Start listening for incoming connections
    void connectToPeer(const std::string &ip, int port); // Connect to another peer
    void joinNetwork(const std::string &ip, int port);   // Join an existing network
    void broadcastAvailability();             // Broadcast this peer's availability
    void handleGossipMessage(const std::string &message); // Handle incoming gossip messages
    void leaveNetwork();
    void discoverPeers();
};