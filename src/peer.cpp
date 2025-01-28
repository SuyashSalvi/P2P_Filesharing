#include "../include/peer.h"

// Constructor to initialize a new peer with its identity and network details
Peer::Peer(const std::string &id, const std::string &ip, int portNumber)
    : peerID(id),
      ipAddress(ip),
      port(portNumber),
      acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      socket(ioContext) {}

// Start listening for other peers trying to connect to this peer
// void Peer::startListening()
// {
//     std::cout << "Peer " << peerID << " listening on " << ipAddress << ":" << port << std::endl;

//     // Wait for incoming connections
//     acceptor.async_accept(
//         [this](boost::system::error_code ec, boost::asio::ip::tcp::socket newSocket)
//         {
//             if (!ec)
//             {
//                 // Successfully accepted a new connection
//                 std::cout << "New connection accepted from: "
//                           << newSocket.remote_endpoint().address().to_string() << ":"
//                           << newSocket.remote_endpoint().port() << std::endl;

//                 // Create a buffer to store incoming messages
//                 char data[1024] = {0}; // Initialize empty buffer of 1024 bytes
//                 boost::system::error_code error;
//                 size_t length = newSocket.read_some(boost::asio::buffer(data), error);

//                 if (!error)
//                 {
//                     // Successfully received a message
//                     std::string receivedMessage(data, length);
//                     std::cout << "Received message: " << receivedMessage << std::endl;
//                     handleGossipMessage(receivedMessage);
//                 }

//                 // Send a welcome message back to the connecting peer
//                 std::string response = "Welcome to Peer " + peerID + "!\n";
//                 boost::asio::write(newSocket, boost::asio::buffer(response));
//             }
//             else if (ec != boost::asio::error::operation_aborted)
//             {
//                 // Connection failed
//                 std::cerr << "Error accepting connection: " << ec.message() << std::endl;
//             }

//             // Continue listening for more connections if the acceptor is still active
//             if (acceptor.is_open())
//             {
//                 startListening();
//             }
//         });

//     // Run the network operations in a separate thread
//     std::thread([this]()
//                 { ioContext.run(); })
//         .detach();
// }

void Peer::startListening() {
    std::cout << "Peer " << peerID << " listening on " << ipAddress << ":" << port << std::endl;

    acceptor.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket newSocket) {
            if (!ec) {
                std::cout << "New connection accepted from: "
                          << newSocket.remote_endpoint().address().to_string() << ":"
                          << newSocket.remote_endpoint().port() << std::endl;

                // Read incoming messages asynchronously
                auto buffer = std::make_shared<std::array<char, 1024>>();
                newSocket.async_read_some(boost::asio::buffer(*buffer),
                                          [this, buffer](boost::system::error_code ec, std::size_t length) {
                                              if (!ec) {
                                                  std::string receivedMessage(buffer->data(), length);
                                                  std::cout << "Received message: " << receivedMessage << std::endl;
                                                  handleGossipMessage(receivedMessage);
                                              } else {
                                                  std::cerr << "Error reading message: " << ec.message() << std::endl;
                                              }
                                          });
            } else {
                std::cerr << "Error accepting connection: " << ec.message() << std::endl;
            }

            // Continue listening for more connections
            if (acceptor.is_open()) {
                startListening();
            }
        });

    // Run the `ioContext` in a dedicated thread
    std::thread([this]() { ioContext.run(); }).detach();
}

// Connect to another peer in the network
void Peer::connectToPeer(const std::string &ip, int port)
{
    std::cout << "Peer " << peerID << " connecting to " << ip << ":" << port << std::endl;

    // Resolve the target peer's address
    boost::asio::ip::tcp::resolver resolver(ioContext);
    auto endpoints = resolver.resolve(ip, std::to_string(port));

    // Try to connect to the target peer
    boost::asio::async_connect(socket, endpoints,
                               [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint)
                               {
                                   if (!ec)
                                   {
                                       // Successfully connected to the peer
                                       std::cout << "Connected to peer successfully!" << std::endl;

                                       // Send a greeting message
                                       std::string message = "Hello from Peer " + peerID + "!\n";
                                       boost::asio::async_write(socket, boost::asio::buffer(message),
                                        [this](boost::system::error_code ec, std::size_t /*length*/) {
                                            if (!ec) {
                                                std::cout << "Message sent successfully to peer." << std::endl;
                                            } else {
                                                std::cerr << "Error sending message: " << ec.message() << std::endl;
                                            }
                                        });

                                       // Wait for and read the response
                                       char data[1024] = {0}; // Initialize empty buffer
                                       boost::system::error_code error;
                                       size_t length = socket.read_some(boost::asio::buffer(data), error);

                                       if (!error)
                                       {
                                           std::string receivedMessage(data, length);
                                           std::cout << "Received response: " << receivedMessage << std::endl;
                                       }
                                   }
                                   else
                                   {
                                       // Connection attempt failed
                                       std::cerr << "Error connecting to peer: " << ec.message() << std::endl;
                                   }
                               });

    ioContext.run();
}

// Tell all known peers about this peer's existence
// void Peer::broadcastAvailability()
// {
//     std::lock_guard<std::mutex> lock(peerListMutex); // Ensure thread-safe access to peer list
//     for (const auto &peer : peerList)
//     {
//         try
//         {
//             // Try to connect to each known peer
//             boost::asio::ip::tcp::resolver resolver(ioContext);
//             auto endpoints = resolver.resolve(peer.first, std::to_string(peer.second));
//             boost::asio::ip::tcp::socket tempSocket(ioContext);

//             boost::asio::connect(tempSocket, endpoints);

//             // Send a message announcing this peer's presence
//             std::string message = "GOSSIP:ADD " + ipAddress + " " + std::to_string(port) + "\n";
//             boost::asio::write(tempSocket, boost::asio::buffer(message));

//             tempSocket.close();
//         }
//         catch (const std::exception &e)
//         {
//             std::cerr << "Error broadcasting to peer " << peer.first << ":" << peer.second
//                       << " - " << e.what() << std::endl;
//         }
//     }
// }

void Peer::broadcastAvailability() {
    std::lock_guard<std::mutex> lock(peerListMutex);

    std::cout << "Broadcasting availability from Peer " << peerID << "..." << std::endl;

    for (const auto &peer : peerList) {
        try {
            boost::asio::ip::tcp::resolver resolver(ioContext);
            auto endpoints = resolver.resolve(peer.first, std::to_string(peer.second));
            boost::asio::ip::tcp::socket tempSocket(ioContext);

            boost::asio::connect(tempSocket, endpoints);

            // Include the sender's ID in the broadcast message
            std::string availabilityMessage = "GOSSIP:BROADCAST " + peerID + " " + ipAddress + " " + std::to_string(port) + "\n";
            boost::asio::write(tempSocket, boost::asio::buffer(availabilityMessage));
            tempSocket.close();

            std::cout << "Notified Peer " << peer.first << ":" << peer.second << " about availability." << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error notifying Peer " << peer.first << ":" << peer.second
                      << " - " << e.what() << std::endl;
        }
    }
}

// Process messages received from other peers
// void Peer::handleGossipMessage(const std::string &message)
// {
//     std::cout << "Processing received message: " << message << std::endl;

//     // Parse the received message
//     std::istringstream stream(message);
//     std::string command, peerIP;
//     int peerPort;
//     stream >> command >> peerIP >> peerPort;

//     if (command == "GOSSIP:ADD")
//     {
//         // A new peer wants to join the network
//         std::lock_guard<std::mutex> lock(peerListMutex);
//         auto peer = std::make_pair(peerIP, peerPort);

//         if (peerList.find(peer) == peerList.end())
//         {
//             // Add the new peer to our list and spread the word
//             peerList.insert(peer);
//             std::cout << "Added new peer: " << peerIP << ":" << peerPort << std::endl;
//             broadcastAvailability();
//         }
//     }
//     else if (command == "GOSSIP:REMOVE")
//     {
//         // A peer is leaving the network
//         std::lock_guard<std::mutex> lock(peerListMutex);
//         auto peer = std::make_pair(peerIP, peerPort);

//         if (peerList.find(peer) != peerList.end())
//         {
//             peerList.erase(peer);
//             std::cout << "Removed peer: " << peerIP << ":" << peerPort << std::endl;
//         }
//     }
//     else
//     {
//         std::cerr << "Unknown command received: " << command << std::endl;
//     }
// }
void Peer::handleGossipMessage(const std::string &message) {
    std::istringstream messageStream(message);
    std::string command, ip;
    int port;

    messageStream >> command >> ip >> port;

    std::lock_guard<std::mutex> lock(peerListMutex);

    if (command == "GOSSIP:ADD") {
        // Validate IP and port before adding to peerList
        if (!ip.empty() && port > 0 && port <= 65535) {
            peerList.insert({ip, port});
            std::cout << "Added Peer " << ip << ":" << port << " to the peer list." << std::endl;
        } else {
            std::cerr << "Invalid GOSSIP:ADD message received: " << message << std::endl;
        }
    } else if (command == "GOSSIP:REMOVE") {
        // Remove the peer from the list
        peerList.erase({ip, port});
        std::cout << "Removed Peer " << ip << ":" << port << " from the peer list." << std::endl;

        // Prevent rebroadcasting the same message
        std::string messageID = command + ":" + ip + ":" + std::to_string(port);
        if (processedMessages.find(messageID) != processedMessages.end()) {
            return; // Ignore duplicate messages
        }
        processedMessages.insert(messageID);

        // Forward the removal message to other peers
        for (const auto &peer : peerList) {
            if (peer.first == ip && peer.second == port) {
                continue; // Skip the removed peer
            }

            try {
                boost::asio::ip::tcp::resolver resolver(ioContext);
                auto endpoints = resolver.resolve(peer.first, std::to_string(peer.second));
                boost::asio::ip::tcp::socket tempSocket(ioContext);

                boost::asio::connect(tempSocket, endpoints);

                boost::asio::write(tempSocket, boost::asio::buffer(message));
                tempSocket.close();

                std::cout << "Forwarded removal message to Peer " << peer.first << ":" << peer.second << "." << std::endl;
            } catch (const std::exception &e) {
                std::cerr << "Error forwarding removal message to Peer " << peer.first << ":" << peer.second
                          << " - " << e.what() << std::endl;
            }
        }
    } else if (command == "GOSSIP:BROADCAST") {
        std::string senderID = ip; // In this context, the `ip` field holds the sender ID
        ip = port > 0 ? ip : "";   // Reset IP and port for the sender
        messageStream >> ip >> port;

        // Prevent rebroadcasting the same message
        std::string messageID = senderID + ":" + ip + ":" + std::to_string(port);
        if (processedMessages.find(messageID) != processedMessages.end()) {
            return; // Ignore duplicate messages
        }
        processedMessages.insert(messageID);

        std::cout << "Broadcast message received from Peer " << senderID << " (" << ip << ":" << port << ")." << std::endl;

        // Forward the broadcast to other peers
        for (const auto &peer : peerList) {
            if (peer.first == ip && peer.second == port) {
                continue; // Don't send the message back to the sender
            }

            try {
                boost::asio::ip::tcp::resolver resolver(ioContext);
                auto endpoints = resolver.resolve(peer.first, std::to_string(peer.second));
                boost::asio::ip::tcp::socket tempSocket(ioContext);

                boost::asio::connect(tempSocket, endpoints);

                boost::asio::write(tempSocket, boost::asio::buffer(message));
                tempSocket.close();

                std::cout << "Forwarded broadcast to Peer " << peer.first << ":" << peer.second << "." << std::endl;
            } catch (const std::exception &e) {
                std::cerr << "Error forwarding broadcast to Peer " << peer.first << ":" << peer.second
                          << " - " << e.what() << std::endl;
            }
        }
    }
}

// Join an existing P2P network through a known peer
void Peer::joinNetwork(const std::string &ip, int port)
{
    std::cout << "Joining network via peer: " << ip << ":" << port << std::endl;

    try
    {
        // Connect to the known peer
        boost::asio::ip::tcp::resolver resolver(ioContext);
        auto endpoints = resolver.resolve(ip, std::to_string(port));
        boost::asio::connect(socket, endpoints);

        // Send a message announcing this peer's presence
        std::string message = "GOSSIP:ADD " + ipAddress + " " + std::to_string(port) + "\n";
        boost::asio::async_write(socket, boost::asio::buffer(message),
            [this](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    std::cout << "Message sent successfully to peer." << std::endl;
                } else {
                    std::cerr << "Error sending message: " << ec.message() << std::endl;
                }
            });

        // Add the initial peer to our list
        {
            std::lock_guard<std::mutex> lock(peerListMutex);
            peerList.insert(std::make_pair(ip, port));
        }

        // Let other peers know about our existence
        broadcastAvailability();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error joining network: " << e.what() << std::endl;
    }
}

// Gracefully leave the P2P network
// void Peer::leaveNetwork()
// {
//     std::cout << "Peer " << peerID << " leaving the network." << std::endl;

//     // Stop accepting new connections
//     acceptor.close();
//     ioContext.stop();

//     std::lock_guard<std::mutex> lock(peerListMutex);

//     // Notify all peers that we're leaving
//     for (const auto &peer : peerList)
//     {
//         try
//         {
//             boost::asio::ip::tcp::resolver resolver(ioContext);
//             auto endpoints = resolver.resolve(peer.first, std::to_string(peer.second));
//             boost::asio::ip::tcp::socket tempSocket(ioContext);

//             boost::asio::connect(tempSocket, endpoints);

//             std::string message = "GOSSIP:REMOVE " + ipAddress + " " + std::to_string(port) + "\n";
//             boost::asio::write(tempSocket, boost::asio::buffer(message));

//             tempSocket.close();
//         }
//         catch (const std::exception &e)
//         {
//             std::cerr << "Error broadcasting removal to peer " << peer.first << ":" << peer.second
//                       << " - " << e.what() << std::endl;
//         }
//     }

//     // Clear our list of peers
//     peerList.clear();
// }

void Peer::leaveNetwork() {
    std::lock_guard<std::mutex> lock(peerListMutex);

    // Notify all peers about leaving the network
    for (const auto &peer : peerList) {
        try {
            boost::asio::ip::tcp::resolver resolver(ioContext);
            auto endpoints = resolver.resolve(peer.first, std::to_string(peer.second));
            boost::asio::ip::tcp::socket tempSocket(ioContext);

            boost::asio::connect(tempSocket, endpoints);

            std::string leaveMessage = "GOSSIP:REMOVE " + ipAddress + " " + std::to_string(port) + "\n";
            boost::asio::write(tempSocket, boost::asio::buffer(leaveMessage));
            tempSocket.close();

            std::cout << "Notified Peer " << peer.first << ":" << peer.second << " about leaving." << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error notifying Peer " << peer.first << ":" << peer.second
                      << " - " << e.what() << std::endl;
        }
    }

    // Clear peerList and stop the acceptor
    peerList.clear();
    acceptor.close();
    std::cout << "Peer " << peerID << " has left the network." << std::endl;
}

void Peer::discoverPeers() {
    std::lock_guard<std::mutex> lock(peerListMutex);
    for (const auto &peer : peerList) {
        try {
            boost::asio::ip::tcp::resolver resolver(ioContext);
            auto endpoints = resolver.resolve(peer.first, std::to_string(peer.second));
            boost::asio::ip::tcp::socket tempSocket(ioContext);

            boost::asio::connect(tempSocket, endpoints);

            // Request peer list
            std::string request = "GOSSIP:REQUEST_PEER_LIST\n";
            boost::asio::write(tempSocket, boost::asio::buffer(request));

            // Read the response
            char data[1024] = {0};
            boost::asio::read(tempSocket, boost::asio::buffer(data));
            std::istringstream responseStream(data);
            std::string command, ip;
            int port;

            while (responseStream >> command >> ip >> port) {
                if (command == "GOSSIP:ADD") {
                    peerList.insert({ip, port});
                }
            }

            tempSocket.close();
        } catch (const std::exception &e) {
            std::cerr << "Error discovering peers from " << peer.first << ":" << peer.second
                      << " - " << e.what() << std::endl;
        }
    }
}