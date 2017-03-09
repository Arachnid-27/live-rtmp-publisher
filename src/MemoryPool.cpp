#include "MemoryPool.h"

const int MemoryPool::mTable[8] = { 256, 512, 1024, 2048, 4096, 8192, 16384, 32768 };

MemoryPool::~MemoryPool() {
    char *head, *next;
    for (int i = 0; i != 8; ++i) {
        head = mList[i];
        while (head != NULL) {
            next = *(reinterpret_cast<char**>(head));
            delete[] head;
            head = next;
        }
    }
}

char* MemoryPool::getChunk(int bytes) {
    int idx = getIndex(bytes);
    std::lock_guard<std::mutex> lock(mMutex);
    char *head = mList[idx];

    if (!head) {
        return new char[mTable[idx]];
    }

    mList[idx] = *(reinterpret_cast<char**>(head));

    return head;
}

void MemoryPool::putChunk(int bytes, char* data) {
    int idx = getIndex(bytes);
    std::lock_guard<std::mutex> lock(mMutex);

    *(reinterpret_cast<char**>(data)) = mList[idx];
    mList[idx] = data;
}

inline int MemoryPool::getIndex(int bytes) const {
    return std::upper_bound(mTable, mTable + 8, bytes) - mTable;
}
