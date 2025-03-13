#pragma once
#include <iostream>
#include <chrono>
#include "MemoryAlloc.h"

template <typename T>
void StandardLibraryAllocTimeBenchmark(int allocationAmount){
    std::chrono::time_point<std::chrono::high_resolution_clock> allocStart, allocEnd;
    
    //type size
    size_t typeSize = sizeof(T);

    //pointers of allocated mem
    T* allocatedTestMem[allocationAmount];

    //start timer
    allocStart = std::chrono::high_resolution_clock::now();

    //allocation
    for (int i = 0; i < allocationAmount; i++){
        allocatedTestMem[i] = (T*)malloc(typeSize);
    }

    //end timer
    allocEnd = std::chrono::high_resolution_clock::now();

    //Calculate duration
    std::chrono::duration allocDur = allocEnd - allocStart;

    std::cout <<"Time for STL malloc() to allocate " << allocDur.count() << " nano seconds" << std::endl;
    

    //Deallocation Time
    std::chrono::time_point<std::chrono::high_resolution_clock> freeStart, freeEnd;

    //Start deallocation timer
    freeStart = std::chrono::high_resolution_clock::now(); 
    
    //Deallocation
    for (int i = 0; i < allocationAmount; i++){
        free(allocatedTestMem[i]);
    }

    //end Deallocation timer
    freeEnd = std::chrono::high_resolution_clock::now();

    std::chrono::duration freeDur = freeEnd - freeStart;

    std::cout <<"Time for STL free() to free memory " << freeDur.count() << " nano seconds" << std::endl;
}


template <typename T>
void CustomAllocationTimeBenchmark(int allocationAmount){
    
    std::chrono::time_point<std::chrono::high_resolution_clock> allocStart, allocEnd;

    CustomAllocator CustomAlloc;

    //type size
    size_t typeSize = sizeof(T);

    //pointers of allocated mem
    T* allocatedTestMem[allocationAmount];

    //start timer
    allocStart = std::chrono::high_resolution_clock::now();

    //allocation
    for (int i = 0; i < allocationAmount; i++){
        allocatedTestMem[i] = (T*)CustomAlloc.FAMalloc(typeSize);
    }

    //end timer
    allocEnd = std::chrono::high_resolution_clock::now();

    //Calculate duration
    std::chrono::duration allocDur = allocEnd - allocStart;


    std::cout <<"Time for custom allocation " << allocDur.count() << " nano seconds" << std::endl;
    
    //Deallocation Time
    std::chrono::time_point<std::chrono::high_resolution_clock> freeStart, freeEnd;

    //Start deallocation timer
    freeStart = std::chrono::high_resolution_clock::now(); 
    
    //Deallocation
    for (int i = 0; i < allocationAmount; i++){
        CustomAlloc.myFree(allocatedTestMem[i]);
    }

    //end Deallocation timer
    freeEnd = std::chrono::high_resolution_clock::now();

    std::chrono::duration freeDur = freeEnd - freeStart;

    std::cout <<"Time for custom free memory " << freeDur.count() << " nano seconds" << std::endl;

}

void FragmentationBenchmark();
