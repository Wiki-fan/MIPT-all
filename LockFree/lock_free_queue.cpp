#include "lock_free_queue.h"

int main() {
    lock_free_queue<int> q;
    q.enqueue(0);

    return 0;
}