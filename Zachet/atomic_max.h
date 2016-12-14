#pragma once

int fetch_and_set_max(std::atomic<int>& x, int y) {
    int xcur;
    do {
        xcur = x.load();
        int max = std::max(xcur, y);
    } while (!x.compare_exchange_strong(xcur, max));
    // weak вроде тоже прокатит. Да, прокатит.
    return xcur;
}
