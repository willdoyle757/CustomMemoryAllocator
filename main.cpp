#include <iostream>
#include "MemoryAlloc.h"
#include "Benchmarks.h"

void debugDemo(){
    CustomAllocator MA;

    int* intPtr1 = (int*)MA.FAMalloc(sizeof(int));
    
    std::cout << "Address of header: " << MA.freeList << std::endl;
    std::cout << "Header Struct size: " << sizeof(BlockHeader) << std::endl;
    //header 24 bytes
    std::cout << "Address of the ptr1 int: " << intPtr1 << std::endl;

    int* intPtr2 = (int*)MA.FAMalloc(sizeof(int));
    *intPtr2 = 24;
    std::cout << "Address of ptr2: " << intPtr2 << std::endl;
    std::cout << "value in ptr 2: " << *intPtr2 << std::endl;

    std::cout << "ptr2 after free " << *intPtr2 << std::endl;
    int* intPtr3 = (int*)MA.FAMalloc(sizeof(int));

    MA.freeListVisual();

    MA.myFree(intPtr2);
    MA.myFree(intPtr3);

    MA.freeListVisual();

    double *doubPtr = (double*)MA.FAMalloc(sizeof(double));

    MA.freeListVisual();
    
    std::cout << "Testing";
}

int main()
{
    //debugDemo();   
    StandardLibraryAllocTimeBenchmark<char>(100);
    CustomAllocationTimeBenchmark<char>(100);
}