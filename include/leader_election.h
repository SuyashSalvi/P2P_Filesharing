#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>

class LeaderElection
{
private:
    std::string peerID;               // Current peer's ID
    std::vector<std::string> ring;    // The ring of peers (IDs)
    std::string leaderID;             // The ID of the elected leader
    mutable std::mutex electionMutex; // Mutex for thread safety
    std::condition_variable cv;       // Condition variable to signal the election result

public:
    LeaderElection(const std::string &peerID, const std::vector<std::string> &ring);

    void startElection();                                     // Initiates the leader election process
    void passToken(const std::vector<std::string> &token); // Receives the token and processes it
    std::string getLeader() const;                            // Returns the current leader's ID
};