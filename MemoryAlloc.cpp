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

            InitHeader(freeList, MEMORY_SIZE - sizeof(BlockHeader));
        };
    
        void InitHeader(BlockHeader* header, size_t size){
            header->isFree = true;
            header->size = size;
            header->nextHeader = nullptr;
        }

        /*
        
        [Header | data  | Header | data            ]

        h1->h2->nullptr
        
        */

        //When myMalloc called recieve the pointer to the address right after the header
        //and then add new header at the end of the given memory and attach to the free list
        void* myMalloc(size_t size){

            void* memPtr;

            //search for next available spot in memory using freeList Linked List
            BlockHeader* currHead = freeList;
            BlockHeader* newHead;

            //find a way to break if it finds no available memory
            while (!(currHead->nextHeader == nullptr || currHead->isFree) || currHead->size < size)
            {
                currHead = currHead->nextHeader;
            }
            std::cout << "Memory Available!" << std::endl;

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
            InitHeader(newHead, oldSize - currHead->size - sizeof(BlockHeader));

            return memPtr;
        }

        //function for visualizing the free list
        //will only visualize the first 10 headers for testing and debuging
        void freeListVisual(){
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
        void myFree(void *ptr){
            //block header being freed
            BlockHeader* header = ((BlockHeader*)ptr - 1);

            //checking to make sure pointer arithmetic was correct
            //std::cout << "ptr header: "<< header << std::endl;

            //set current header to true
            header->isFree = true;

            //Here the free function check if the header above and below are free and then merges them

            //merge header with free header below
            if (header->nextHeader->isFree){
                //adjust size of merge
                header->size += (header->nextHeader->size + sizeof(BlockHeader));
                //sets next header
                header->nextHeader = header->nextHeader->nextHeader;
            }


            //merger header with free header above
            //go through the freelist to find the prev header 
            //currHeader is the previous block header to of the header being freed
            BlockHeader* currHeader = freeList;
            while(currHeader->nextHeader != header){

                currHeader = currHeader->nextHeader;
            }

            //check prev header availablilty
            if (currHeader->isFree){

                currHeader->nextHeader = header->nextHeader;
                currHeader->size += (header->size + sizeof(BlockHeader));
                
            }

            //merge headers that are adacent and free
            
        }
};


int main()
{   
    CustomAllocator MA;

    int* intPtr1 = (int*)MA.myMalloc(sizeof(int));
    
    std::cout << "Address of header: " << MA.freeList << std::endl;
    std::cout << "Header Struct size: " << sizeof(BlockHeader) << std::endl;
    //header 24 bytes
    std::cout << "Address of the ptr1 int: " << intPtr1 << std::endl;

    int* intPtr2 = (int*)MA.myMalloc(sizeof(int));
    *intPtr2 = 24;
    std::cout << "Address of ptr2: " << intPtr2 << std::endl;
    std::cout << "value in ptr 2: " << *intPtr2 << std::endl;

    std::cout << "ptr2 after free " << *intPtr2 << std::endl;
    int* intPtr3 = (int*)MA.myMalloc(sizeof(int));

    MA.freeListVisual();

    MA.myFree(intPtr2);

    MA.freeListVisual();

    double *doubPtr = (double*)MA.myMalloc(sizeof(double));

    MA.freeListVisual();

    //24+4+24
    

}