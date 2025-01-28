#include "../include/leader_election.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Define a ring of peers
    std::vector<std::string> ring = {"peer1", "peer2", "peer3", "peer4"};

    // Create a LeaderElection object for one of the peers
    LeaderElection election("peer2", ring);

    // Start the leader election process
    election.startElection();

    // Wait for the election to complete (simulate waiting in a real distributed system)
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Get the leader
    std::string leader = election.getLeader();
    std::cout << "Elected Leader: " << leader << std::endl;

    return 0;
}