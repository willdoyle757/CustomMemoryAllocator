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
        
        [Header | data  | Header                                    ]

        h1->h2->nullptr
        nf->f
        
        */

        //When myMalloc called recieve the pointer to the address right after the header
        //and then add new header at the end of the given memory and attach to the free list
        void* myMalloc(size_t size){

            void* memPtr;

            //search for next available spot in memory using freeList Linked List
            BlockHeader* currHead = freeList;
            BlockHeader* newHead;

            //find a way to break if it finds no available memory
            while (!(currHead->nextHeader == nullptr || currHead->isFree))
            {
                currHead = currHead->nextHeader;
            }
            std::cout << "Memory Available!" << std::endl;

            //pointer after block header which is where the actual memory starts
            memPtr = (void*)(currHead + 1);

            //sets current mem block to occupied and currHead size to memory size
            currHead->isFree = false;
            currHead->size = size;

            //new header ptr after allocated memory by moving the pointer, size amount of bytes
            //from the memPtr to where the next header would be
            //(char*) == 1 byte
            newHead = (BlockHeader*)((char*)memPtr + size);
            currHead->nextHeader = newHead;

           
            //need to find a way to get size of current header
            InitHeader(newHead, currHead->size - size - sizeof(BlockHeader));

            return memPtr;
        }

        
        void myFree(void *ptr){

            //Set block header to free
            //Check if the block above and below are free then merge with those blocks
            


        }

        void printHeaders(){

        }

};


int main()
{   
    CustomAllocator MA;

    int* intPtr1 = (int*)MA.myMalloc(sizeof(int));

    std::cout << "Address of header: " << MA.freeList << std::endl;
    std::cout << "Header Struct size: " << sizeof(BlockHeader) << std::endl;
    std::cout << "Address of the ptr1 int: " << intPtr1 << std::endl;

    int* intPtr2 = (int*)MA.myMalloc(sizeof(int));
    *intPtr2 = 24;
    std::cout << "Address of ptr2: " << intPtr2 << std::endl;
    std::cout << "value in ptr 2: " << *intPtr2 << std::endl;



    std::cout << "ptr2 after free " << *intPtr2 << std::endl;

    //24+4+24
    

}