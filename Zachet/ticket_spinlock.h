#pragma once

#include <atomic>
#include <cstddef>
#include <thread>

class ticket_spinlock {
public:
    ticket_spinlock() : currentTicket(0), nextFreeTicket(0) {}

    void lock() {
        size_t this_thread_ticket = nextFreeTicket.fetch_add(1);
        while (currentTicket.load() != this_thread_ticket) {
            std::this_thread::yield();
        }
    }

    void unlock() {
        currentTicket.store(currentTicket.load()+1);
    }

private:
    std::atomic<size_t> currentTicket;
    std::atomic<size_t> nextFreeTicket;
};
