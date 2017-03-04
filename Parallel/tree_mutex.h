#pragma once
//#include "../stdafx.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <stack>
#include <cmath>
#include <cassert>

class PetersonMutex {
public:
    PetersonMutex() {
        want_[0].store(false);
        want_[1].store(false);
        victim_.store(0);
    }

    PetersonMutex(PetersonMutex&& other) {
        victim_.store(other.victim_.load());
        want_[0].store(other.want_[0].load());
        want_[1].store(other.want_[1].load());
    }

    void lock(int t) {
        want_[t].store(true);
        victim_.store(t);
        while (want_[1 - t].load() && victim_.load() == t) {
            std::this_thread::yield();
        }
    }

    void unlock(int t) {
        want_[t].store(false);
    }

private:
    std::array<std::atomic<bool>, 2> want_;
    std::atomic<int> victim_;
};

class tree_mutex {
public:
    tree_mutex(std::size_t num_threads) {
        num_threads_ = num_threads;
        size_t leaves_count = 1;
        while (leaves_count < num_threads_) {
            leaves_count *= 2;
        }
        mutexes_.resize(leaves_count - 1);
        size_.store(leaves_count - 1);
    }

    void lock(std::size_t threadIndex) {
        threadIndex += size_.load();
        //recoursiveLock(threadIndex);
        iterativeLock(threadIndex);
    }

    void unlock(std::size_t threadIndex) {
        threadIndex += size_.load();
        //recoursiveUnlock( threadIndex );
        iterativeUnlock(threadIndex);
    }

private:
    size_t num_threads_;

    std::vector<PetersonMutex> mutexes_;
    std::atomic<size_t> size_;

    // Returns index of parent. Returns 0 if called with t==0 (root).
    std::size_t getParent(std::size_t t) {
        return t == 0 ? 0 : (t - 1) / 2;
    }

    // Returns 0 or 1 depending on in which branch node t lies.
    int getSide(size_t t) {
        return t % 2 == 0 ? 1 : 0;
    }

    inline void iterativeLock(std::size_t t) {
        while (t != 0) {
            auto parent = getParent(t);
            mutexes_[parent].lock(getSide(t));
            t = parent;
        }
    }

    /*void recoursiveLock( std::size_t t )
    {
        auto parent = getParent( t );
        if( !( parent == 0 && t == 0 )) {
            // locking side of the parent node
            //debug( "Locking %d\n", parent );
            mutexes_[parent].lock( getSide( t ));
            lock( parent );
        }
    }*/

    inline void iterativeUnlock(std::size_t t) {
        std::stack<size_t> st;
        while (t != 0) {
            st.push(t);
            t = getParent(t);
        }

        while (!st.empty()) {
            t = st.top();
            st.pop();
            auto parent = getParent(t);
            mutexes_[parent].unlock(getSide(t));
        }
    }

    /*void recoursiveUnlock( std::size_t t )
    {
        auto parent = getParent( t );
        if( !( parent == 0 && t == 0 )) {
            unlock( parent );
            //debug( "Unlocking %d\n", parent );
            mutexes_[parent].unlock( getSide(t) );
        }
    }*/
};


class MutexTreeTester {
public:
    MutexTreeTester() {}

    void test(size_t numberThreads, size_t numberIncrements) {
        tree_mutex m(numberThreads);
        size_t x = 0;
        std::vector<std::thread> threads;
        for (size_t i = 0; i < numberThreads; ++i) {
            threads.emplace_back(increment, &x, &m, i, numberIncrements);
        }
        for (size_t i = 0; i < numberThreads; ++i) {
            threads[i].join();
        }
        std::cout << x << std::endl;
        assert(x == numberIncrements * numberThreads && "Wrong answer");
    }
private:
    static void increment(size_t* x, tree_mutex* m, size_t thread, size_t numberIncrements) {
        for (size_t i = 0; i < numberIncrements; ++i) {
            m->lock(thread);
            ++*x;
            m->unlock(thread);
        }
    }
};