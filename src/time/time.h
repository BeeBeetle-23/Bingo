#pragma once
#include <chrono>

namespace TimeManager {
    extern std::chrono::time_point<std::chrono::steady_clock> start_time;
    extern int target_time;
    extern bool search_aborted;
    extern long long nodes;

    // Call this when the "go" command is received
    void init(int my_remaining_time, int my_increment);
    void check_time();
    // Call this inside negamax to check if time is up
    void init_infinite();
}