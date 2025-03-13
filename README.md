# Custom Memory Allocator

## Overview
This project implements a custom memory allocator in C++ to manage dynamic memory more efficiently than the standard `malloc` and `free`. The allocator provides functions for allocating and deallocating memory while maintaining a free list to track available memory blocks.

## Features
- **Custom Memory Allocation (`FAMalloc`)**: Allocates memory blocks with metadata for tracking.
- **Custom Memory Deallocation (`myFree`)**: Frees allocated memory and updates the free list.
- **Free List Management**: Keeps track of available memory chunks to minimize fragmentation.
- **Benchmarking**: Includes utilities to measure allocation and deallocation performance.

## Files
- `MemoryAlloc.h` / `MemoryAlloc.cpp`: Implements the memory allocator.
- `Benchmarks.h` / `Benchmarks.cpp`: Contains benchmarking utilities.
- `main.cpp`: Demonstrates usage and testing of the allocator.

## Usage
### Compilation
Compile by going into the build directory

```sh
cd build
```
Using cmake create a new build with 

```sh
cmake -G "Unix Makefiles"
```
Then compile by calling make
```sh
make
```
Or manually compile:
```sh
g++ -std=c++20 -c MemoryAlloc.cpp -o MemoryAlloc.o
g++ -std=c++20 -c Benchmarks.cpp -o Benchmarks.o
g++ -std=c++20 -c main.cpp -o main.o
g++ -std=c++20 MemoryAlloc.o Benchmarks.o main.o -o main
```

### Running the Allocator Demo
Run the compiled executable:
```sh
./main
```

## Performance Benchmarking
Run the benchmark tests:
```cpp
AllocationTimeBenchmark();
```
This will measure allocation and deallocation speed and print results to the console.

## Future Improvements
- Add fragmentation benchmarking
- Optimize free list merging to reduce fragmentation.
- Add multi-threading support for concurrent allocations.
