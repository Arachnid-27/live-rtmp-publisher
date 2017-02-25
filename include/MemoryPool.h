#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include <cstring>
#include <algorithm>

class MemoryPool {
public:
    MemoryPool() { memset(mList, 0, 8 * sizeof(char*)); }

    ~MemoryPool();

    char* getChunk(int bytes);

    void putChunk(int bytes, char* data);
private:
    int getIndex(int bytes) const;
private:
    char *mList[8];
    static const int mTable[8];
};

#endif
