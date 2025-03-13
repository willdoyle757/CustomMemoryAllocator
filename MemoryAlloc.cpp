#include <iostream>
#include <windows.h>
#include "MemoryAlloc.h"
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

//Heap based memory managment

//Allocates 10mb of memory to memoryPool pointer
void* memoryPool;

BlockHeader* freeList;

//Contsructor 
CustomAllocator::CustomAllocator(){
    //provides pool of memory to be allocated 
    memoryPool = VirtualAlloc(NULL, CustomAllocator::MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!memoryPool)
    {
        std::cout << "Error Allocating Memory";
        exit(1);
    }

    //Initiallizes the free list by setting the pointer to the start of the memory pool
    freeList = (BlockHeader*)memoryPool;

    //initializes the first block header in freelist
    InitHeader(freeList, MEMORY_SIZE - sizeof(BlockHeader));
};

//Initialized the block header when it is created
void CustomAllocator::InitHeader(BlockHeader* header, size_t size){
    header->isFree = true;
    header->size = size;
    header->nextHeader = nullptr;
}

/*
[Header | data  | Header | data            ]
h1->h2->nullptr
*/

//FAMalloc Allocated memory using first available allocation strategy
//takes in the size in bytes of the desired spae in memory and return the pointer to the space
void* CustomAllocator::FAMalloc(size_t size){

    //memory pointer that will be returned
    void* memPtr;

    //search for next available spot in memory using freeList Linked List
    BlockHeader* currHead = freeList;
    BlockHeader* newHead;
    //TODO break if it finds no available memory
    while (!(currHead->nextHeader == nullptr || currHead->isFree) || currHead->size < size)
    {
        currHead = currHead->nextHeader;
    }
    //std::cout << "Memory Available!" << std::endl;

    //pointer to memory after block header which is where the actual memory starts 
    //this is whats returned
    memPtr = (void*)(currHead + 1);

    //stash old size value of the current head
    int oldSize = currHead->size;

    //sets current mem block to occupied and currHead size to allocated memory size
    currHead->isFree = false;
    currHead->size = size;

    //new header ptr after allocated memory by moving the pointer, size amount of bytes
    //from the memPtr to where the next header would be
    //(char*) == 1 byte
    newHead = (BlockHeader*)((char*)memPtr + size);
    currHead->nextHeader = newHead;

    //need to find a way to get size of current header
    CustomAllocator::InitHeader(newHead, oldSize - currHead->size - sizeof(BlockHeader));

    return memPtr;
}

//function for visualizing the free list
//will only visualize the first 10 headers for testing and debuging
void CustomAllocator::freeListVisual(){
    BlockHeader* currHead = freeList;
    int headerCount = 1;
    char free;
    while (currHead->nextHeader != nullptr)
    {
        free = (currHead->isFree) ? 'y' : 'n';
        std::cout << "Header: " << headerCount << ", size: " << currHead->size << " is free: " << free << "\n";
        std::cout << "---------------------------------------------------------------------" << "\n";
        currHead = currHead->nextHeader;
        headerCount++;
    }
    free = (currHead->isFree) ? 'y' : 'n';
    std::cout << "Header: " << headerCount << ", size: " << currHead->size << " is free: " << free << "\n";

}

//Set block header to free
//Check if the block above and below are free then coalesce with those blocks
void CustomAllocator::myFree(void *ptr){
    //block header being freed
    BlockHeader* header = ((BlockHeader*)ptr - 1);


    //checking to make sure pointer arithmetic was correct
    //std::cout << "ptr header: "<< header << std::endl;

    //set current header to true
    header->isFree = true;

    //Here the free function check if the header above and below are free and then merges them
    
    //merge header with free header below
    //potential null pointer here fix later
    if (header->nextHeader->isFree){
        //adjust size of merge
        header->size += (header->nextHeader->size + sizeof(BlockHeader));
        //sets next header
        header->nextHeader = header->nextHeader->nextHeader;
    }


    //merger header with free header above
    //go through the freelist to find the prev header or if header is the first
    //header on the freelist skip
    //currHeader is the previous block header to of the header being freed
    BlockHeader* currHeader = freeList;
    while(currHeader->nextHeader != header && currHeader !=header){
        currHeader = currHeader->nextHeader;
    }
    //check prev header availablilty
    if (currHeader->isFree && currHeader != header){

        currHeader->nextHeader = header->nextHeader;
        currHeader->size += (header->size + sizeof(BlockHeader));
        
    }

    //merge headers that are adacent and free
    
}

