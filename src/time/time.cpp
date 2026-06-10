#include <chrono>
#include "time/time.h"
#include <chrono>

namespace TimeManager {
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    int target_time = 0;
    bool search_aborted = false;
    long long nodes = 0;
    
    // 💡 CRITICAL ADDITION: Flag to tell the engine whether to enforce the clock
    bool time_limit_enforced = false; 

    // Use this when playing an actual game with a clock (e.g., UCI "go wtime 30000 btime 30000")
    void init(int my_remaining_time, int my_increment) {
        // Allocate budget (divide remaining time by 40, add increment)
        target_time = (my_remaining_time / 40) + my_increment;

        // Safety buffer so we don't lose on time due to GUI lag
        if (target_time > my_remaining_time - 50) {
            target_time = my_remaining_time - 50;
        }
        if (target_time < 5) target_time = 5; // Give at least 5ms

        start_time = std::chrono::steady_clock::now();
        search_aborted = false;
        nodes = 0; 
        time_limit_enforced = true; // Turn the enforcer ON
    }

    // 💡 NEW FUNCTION: Use this when doing fixed-depth or infinite testing in main()
    void init_infinite() {
        start_time = std::chrono::steady_clock::now();
        target_time = 2147483647; // Max int (effectively infinite)
        search_aborted = false;
        nodes = 0;
        time_limit_enforced = false; // Turn the clock enforcer OFF
    }

    void check_time() {
        // If we are just doing manual depth/infinite testing, don't abort!
        if (!time_limit_enforced) return;

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