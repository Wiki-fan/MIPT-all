#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>

//When semaphore is 0 some thread is captured it
class Semaphore {
public:
    Semaphore() {}
    Semaphore(int counter_) : counter(counter_) {}

    // leave
    void post() {
        std::unique_lock<std::mutex> lock(m);
        ++counter;
        cv.notify_one();
    }

    // wait while counter becomes >0 and enter
    void wait() {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&] { return counter > 0; });
        --counter;
    }
private:
    std::mutex m;
    std::condition_variable cv;
    int counter;
};
