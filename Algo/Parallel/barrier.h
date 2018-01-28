#include <mutex>
#include <condition_variable>

class barrier {
public:
    explicit barrier(size_t num_threads) : n(num_threads), gate1(0), gate2(0) {}

    void enter() {
        std::unique_lock<std::mutex> lock(m);

        if (gate1 == n - 1) {
            gate1 = 0;
            cv.notify_all();
        } else {
            ++gate1;
            cv.wait(lock, [&] { return gate1 == 0; });
        }

        if (gate2 == n - 1) {
            gate2 = 0;
            cv.notify_all();
        } else {
            ++gate2;
            cv.wait(lock, [&] { return gate2 == 0; });
        }

    }

private:
    std::mutex m;
    std::condition_variable cv;
    size_t n, gate1, gate2;
};
