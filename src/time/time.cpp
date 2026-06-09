#include <chrono>
#include "time/time.h"
namespace TimeManager {
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    int target_time = 0;
    bool search_aborted = false;
    long long nodes = 0;

    void init(int my_remaining_time, int my_increment) {
        // Allocate budget (divide remaining time by 40, add increment)
        target_time = (my_remaining_time / 40) + my_increment;

        // Safety buffer so we don't flag on GUI lag
        if (target_time > my_remaining_time - 50) {
            target_time = my_remaining_time - 50;
        }
        if (target_time < 1) target_time = 1;

        start_time = std::chrono::steady_clock::now();
        search_aborted = false;
        nodes = 0; // Reset nodes for the new move
    }

    void check_time() {
        // Bitwise trick: Only poll the heavy system clock every 2048 nodes
        if ((nodes & 2047) == 0) {
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();

            if (elapsed >= target_time) {
                search_aborted = true;
            }
        }
    }
}