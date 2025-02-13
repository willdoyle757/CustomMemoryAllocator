#include <iostream>
#include <windows.h>
/*
Make a syscall to windows for a chunk of heap and then build a custom dynamic memory allocator to effectively 

Syscalls
1. void* VirtualAlloc(addr, #ofbytes, flags, read write access);
2. bool VirtualFree(ptr, 0, flags)

Requirements
1. Memory pool - large chunk of resources for the 
2. Free list - keeping track datachunks which are free
3. Block chunk meta data - data for each of the chunks
4. Algo for fitting memory chunck
5. Coalescing - Every time a memory chunk is freed check if the above and below blocks are also free
6. functions malloc and free - called by user to allocated and free memory

BlockHeader* freeList -> memoryPool;
freelist


*/

struct BlockHeader {
    size_t size;
    bool isFree;
    BlockHeader* nextHeader;
};

//Heap based memory managment
class CustomAllocator
{
    public:
        //10mb mem pool
        const size_t MEMORY_SIZE = 1024 * 1024 * 10;
        //4kb chunk size
        const size_t CHUNCK_SIZE = 4096;
        //2560 chunks

        //Allocates 10mb of memory to memoryPool pointer
        void* memoryPool = VirtualAlloc(NULL, MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

        BlockHeader* freeList;


        CustomAllocator(){
            if (!memoryPool)
            {
                std::cout << "Error Allocating Memory";
                exit(1);
            }

            freeList = (BlockHeader*)memoryPool;
            freeList->isFree = true;
            freeList->size = MEMORY_SIZE - sizeof(BlockHeader);
            freeList->nextHeader = nullptr;
    
        };

        void* myMalloc(size_t size){

        }

        void myFree(void *ptr){

        }

};


int main()
{   
    CustomAllocator MA;
    std::cout << MA.freeList->size;

}