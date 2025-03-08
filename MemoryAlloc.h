#include <iostream>
#pragma once

struct BlockHeader{
    size_t size;
    bool isFree;
    BlockHeader* nextHeader;
};

class CustomAllocator
{
    public:
        size_t MEMORY_SIZE = 1024 * 1024 * 10;
        void* memoryPool;
        BlockHeader* freeList;

        CustomAllocator();
        static void InitHeader(BlockHeader* header, size_t size);
        void* FAMalloc(size_t size);
        void freeListVisual();  
        void myFree(void *ptr);
};

