#pragma once
#include "semaphore.h"
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>

class RobotWithCondvars {
public:
    RobotWithCondvars() {}

    void start() {
        std::thread t0(&RobotWithCondvars::step, this, 0);
        std::thread t1(&RobotWithCondvars::step, this, 1);
        t0.join();
        t1.join();
    }

private:
    bool current = 0;
    std::mutex m;
    std::condition_variable cv[2];

    void step(int leg) {
        std::unique_lock<std::mutex> lock(m);
        for (int i = 0; i < 1000; ++i) {
            cv[leg].wait(lock, [&] { return current == leg; });
            std::cout << (leg == 0 ? "left" : "right") << std::endl;
            std::cout.flush();
            current = 1 - leg;
            cv[1 - leg].notify_one();
        }

    }
};

class RobotWithSemaphores {
public:
    RobotWithSemaphores() : sem(2) {}

    void start() {
        std::thread t0(&RobotWithSemaphores::step, this, 0);
        std::thread t1(&RobotWithSemaphores::step, this, 1);
        t0.join();
        t1.join();
    }

private:
    Semaphore sem;

    void step(int leg) {
        for (int i = 0; i < 1000; ++i) {
            sem.wait();
            std::cout << (leg == 0 ? "left" : "right") << std::endl;
            std::cout.flush();
            sem.post();
        }

    }
};