#include <iostream>
#include <chrono>
#include "Benchmarks.h"
#include "MemoryAlloc.h"

//Functions to measure time and fragmentation

//Measure the time it takes to allocate 
void AllocationTimeBenchmark(){
    
    std::chrono::time_point<std::chrono::high_resolution_clock> allocStart, allocEnd;

    CustomAllocator CustomAlloc;

    //sets amount of allocated spaces
    int allocationAmount = 10;
    //pointers of allocated mem
    int* allocatedTestMem[allocationAmount];

    //type of value allocated
    size_t typeSize = sizeof(int);

    //start timer
    allocStart = std::chrono::high_resolution_clock::now();

    //allocation
    for (int i = 0; i < allocationAmount; i++){
        allocatedTestMem[i] = (int*)CustomAlloc.FAMalloc(typeSize);
    }

    //end timer
    allocEnd = std::chrono::high_resolution_clock::now();

    std::chrono::duration allocDur = allocEnd - allocStart;

    std::cout <<"Time of allocation " << allocDur.count() << " nano seconds" << std::endl;

    
    std::chrono::time_point<std::chrono::high_resolution_clock> freeStart, freeEnd;

    freeStart = std::chrono::high_resolution_clock::now(); 
    
    for (int i = 0; i < allocationAmount; i++){
        CustomAlloc.myFree(allocatedTestMem[i]);
    }

    freeEnd = std::chrono::high_resolution_clock::now();

    std::chrono::duration freeDur = freeEnd - freeStart;


    std::cout <<"Time to free memory " << freeDur.count() << " nano seconds" << std::endl;


}

void FragmentationBenchmark(){

}