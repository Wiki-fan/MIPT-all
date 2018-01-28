#pragma once

#include <deque>
#include <cstddef>
#include <shared_mutex>
#include <exception>
#include <future>
#include <queue>

class MyException : public std::exception {
public:
    explicit MyException(const std::string& what_) : cause(what_) {}

    const char* what() const throw() override {
        return cause.c_str();
    }

private:
    std::string cause;
};

template<class Value, class Container = std::deque<Value>>
class thread_safe_queue {
public:

    explicit thread_safe_queue(size_t capacity_) {
        size = capacity_;
        isOpen = true;
    }

    void enqueue(Value v) {
        std::unique_lock<std::mutex> lock(mutex);
        if (!isOpen) {
            throw MyException("Push to closed queue");
        }
        cvPush.wait(lock, [this]() { return queue.size() != size || !isOpen; });
        if (!isOpen) {
            throw MyException("Queue was closed, so we can't push");
        }

        queue.push_back(std::move(v));
        cvPop.notify_one();
    }

    void pop(Value& v) {
        std::unique_lock<std::mutex> lock(mutex);
        if (!isOpen && queue.size() == 0) {
            throw MyException("Pop from empty closed queue");
        }
        cvPop.wait(lock, [this]() { return queue.size() != 0 || !isOpen; });
        if (!isOpen) {
            if (queue.size() == 0) {
                throw MyException("Storage was closed, and queue was emptied yet");
            }
        }
        v = std::move(queue.front());
        queue.pop_front();
        cvPush.notify_one();
    }

    // Если элемент есть, возвращает true и кладёт элемент в v. Иначе не блокируется и возвращает false.
    bool try_pop(Value& v) {

        std::unique_lock<std::mutex> lock(mutex);
        if (!isOpen && queue.size() == 0) {
            throw MyException("Pop from empty closed queue");
        }
        if (queue.size() == 0)
            return false;
        v = std::move(queue.front());
        queue.pop_front();
        cvPush.notify_one();
        return true;
    }

    void shutdown() {
        isOpen = false;
        cvPush.notify_all();
        cvPop.notify_all();
    }

private:
    std::mutex mutex;
    std::condition_variable cvPush, cvPop;
    Container queue;
    size_t size;
    std::atomic<bool> isOpen;
};

#include <deque>
#include <cstddef>
#include <shared_mutex>
#include <exception>
#include <future>
#include <queue>
//#include "thread_safe_queue.h"

template<class Value>
class thread_pool {
public:

    explicit thread_pool(size_t num_workers)
            : size(num_workers), closed(false), queue(std::numeric_limits<size_t>::max()) {
        for (size_t i = 0; i < size; ++i)
            threads.emplace_back(
                    [this] {
                        while (true) {
                            std::packaged_task<Value()> task;
                            try {
                                queue.pop(task);
                            } catch (MyException& e) {
                                // Если хранилище закрыто, то все потоки будут отваливаться с исключениями.
                                --size;
                                if (size == 0)
                                    cv.notify_one();
                                break;
                            }
                            task();
                        }
                    }
            );
    }

    std::future<Value> submit(std::function<Value()> f) {
        std::unique_lock<std::mutex> lock(mutex);
        if (closed) {
            throw MyException("Pool is closed");
        }
        std::packaged_task<Value()> task(f);
        std::future<Value> future = task.get_future();
        queue.enqueue(std::move(task));
        return future;
    }

    void shutdown() {
        closed = true;
        std::unique_lock<std::mutex> lock(mutex);
        queue.shutdown();
        cv.wait(lock, [this] { return size == 0; });
        for (size_t i = 0; i < threads.size(); ++i) {
            threads[i].join();
        }
    }

    void wait(std::future<Value>& async_result) {
        //std::unique_lock<std::mutex> lock(mutex);
        // Если очередь не пуста, делаем новые задания. Выходим, если было сделано то, которое мы ждём.
        // Если в очереди не осталось заданий, значит, нужное уже выполняется и скоро будет сделано.
        // Можем выйти из цикла и встать на wait'е.
        std::packaged_task<Value()> task;
        while (async_result.wait_for(std::chrono::duration<int>(0)) != std::future_status::ready) {
            if (!queue.try_pop(task)) {
                break;
            } else {
                task();
            }
        }
        async_result.wait();
    }

private:

    std::atomic<size_t> size;
    std::atomic<bool> closed;
    thread_safe_queue<std::packaged_task<Value()>> queue;
    std::vector<std::thread> threads;
    std::mutex mutex;
    std::condition_variable cv;
};