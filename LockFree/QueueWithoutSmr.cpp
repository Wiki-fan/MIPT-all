#include "QueueWithoutSmr.h"

int main() {
    QueueWithoutSmr<int> lfq;
    lfq.enqueue(1);
    int t;
    lfq.dequeue(t);

    return 0;
}
