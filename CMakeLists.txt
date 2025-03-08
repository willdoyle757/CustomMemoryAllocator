cmake_minimum_required(VERSION 3.30.0)
project(MemoryManager LANGUAGES C CXX)

add_executable(main main.cpp MemoryAlloc.cpp MemoryAlloc.h Benchmarks.cpp Benchmarks.h)