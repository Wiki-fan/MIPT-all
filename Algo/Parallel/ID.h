#pragma once

#include <mutex>
#include <set>

class CMyMutex {
public:
    explicit CMyMutex(int id);

    void lock();
    void unlock();

private:
    const int id;
    std::mutex mut;
    static thread_local std::set<int> minNumber;
};