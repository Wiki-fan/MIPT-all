#pragma once

#include <thread>
#include <functional>
#include <vector>

template<class Task>
void hello_world(std::function<void(const Task&)> func, const std::vector<Task>& tasks) {
    std::vector<std::thread> threads;
    for (auto&& task: tasks) {
        threads.emplace_back(func, task);
    }
    for (auto&& thread: threads) {
        thread.join();
    }
}
