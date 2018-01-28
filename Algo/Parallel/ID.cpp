#include <assert.h>
#include "ID.h"

thread_local std::set<int> CMyMutex::minNumber;

void CMyMutex::lock()
{
    assert(minNumber.size() == 0);
}