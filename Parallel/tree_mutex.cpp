#include "tree_mutex.h"

int main() {
    MutexTreeTester tester;
    for (int i = 0; i < 4; ++i)
        tester.test(36, 10000);
}