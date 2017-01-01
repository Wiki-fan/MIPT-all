#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdlib.h>

class recursive_mutex {
public:
    void lock() {
        std::unique_lock<std::mutex> lock(mutex);
        while (counter > 0 && ownerId != std::this_thread::get_id()) {
            cv.wait(lock);
        }
        ownerId = std::this_thread::get_id();
        ++counter;
    }

    void unlock() {
        std::unique_lock<std::mutex> lock(mutex);
        --counter;

        if (counter == 0) {
            cv.notify_one();
        }
    }
private:
    std::mutex mutex;
    std::condition_variable cv;
    size_t counter = 0;
    std::thread::id ownerId;
};
