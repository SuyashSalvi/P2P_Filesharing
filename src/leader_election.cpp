#include "../include/leader_election.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

LeaderElection::LeaderElection(const std::string &peerID, const std::vector<std::string> &ring)
    : peerID(peerID), ring(ring), leaderID("") {}

void LeaderElection::startElection()
{
    std::lock_guard<std::mutex> lock(electionMutex);

    // Start with the current peer's ID in the token
    std::vector<std::string> token = {peerID};
    std::cout << "Peer " << peerID << " starting the election." << std::endl;

    // Pass the token to the next peer in the ring
    passToken(token);
}

void LeaderElection::passToken(const std::vector<std::string> &token)
{
    std::vector<std::string> mutableToken = token; // Create a local copy of the token
    size_t currentPeerIndex = std::find(ring.begin(), ring.end(), peerID) - ring.begin();

    // Add the current peer's ID to the token if not already present
    if (std::find(mutableToken.begin(), mutableToken.end(), peerID) == mutableToken.end())
    {
        mutableToken.push_back(peerID); // Add the current peer to the token
    }

    // Check if the token has completed the ring
    if (mutableToken.size() == ring.size())
    {
        // Election complete; determine the leader (peer with the highest ID)
        leaderID = *std::max_element(mutableToken.begin(), mutableToken.end());
        std::cout << "Election complete. Leader is Peer " << leaderID << "." << std::endl;
        return; // Stop the election process
    }

    // Determine the next peer in the ring
    size_t nextPeerIndex = (currentPeerIndex + 1) % ring.size();
    std::string nextPeer = ring[nextPeerIndex];

    // Simulate passing the token to the next peer
    std::cout << "Peer " << peerID << " passing token to Peer " << nextPeer << "." << std::endl;

    // Simulate token reception by the next peer
    LeaderElection nextPeerElection(nextPeer, ring);
    nextPeerElection.passToken(mutableToken);
}

std::string LeaderElection::getLeader() const
{
    std::lock_guard<std::mutex> lock(electionMutex);
    return leaderID;
}