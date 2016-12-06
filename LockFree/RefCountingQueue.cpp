#include "RefCountingQueue.h"

int main() {
    lock_free_queue<int> q;
    q.push(0);
    return 0;
}