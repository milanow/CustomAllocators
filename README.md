# CustomAllocators
## Introduction
A series of custom allocators implementation, representing different ways of using memory.

## Motivation
Game developers always want to make the code run quickly. Except for improving algorithms it employ, how program utilizes RAM memory can also affect performance a lot.

Here is a quote from *"Game Engine Architecture - Jason Gregory"*, pointing out there are two aspects memory may affect performance

> 1. Dynamic memory allocation via malloc() or C++’s global operator new is a very slow operation. We can improve the performance of our code by either avoiding dynamic allocation altogether or by making use of custom memory allocators that greatly reduce allocation costs. 
> 2. On modern CPUs, the performance of a piece of software is often dom- inated by its memory access patterns. As we’ll see, data that is located in small, contiguous blocks of memory can be operated on much more efficiently by the CPU than if that same data were to be spread out across a wide range of memory addresses. Even the most efficient algorithm, coded with the utmost care, can be brought to its knees if the data upon which it operates is not laid out efficiently in memory.

## Usage Instruction
This program is developed in Visual Studio 2015, simply click '.sln' file to open it in VS2015 and then build the project. In the future I may add makefile for it.

## Custom Allocators
* **Free list Allocator**: The basic requirement of custom allocator is to avoid context switch cost in runtime.
   malloc/new is designed for all kinds of memory allocation and it might be a critical
   performance hit in game programming.
   FreelistAllocator is a custom allocator that allows allocating any bytes (larger than a 
   header actually, because it wastes memory) and free the memory quickly. It uses
   a linkedlist to connect each memory blocks together and use info int blocks' headers to 
   store the blocks' info (blocksize & alignment ajustment).

* **Stack Allocator**: A stack allocator can be used to avoid fragmentation and it can be implemented pretty easily. Many games allocate memory in a stack-like fashion. Whenever a new game level is loaded, memory is allocated for it. Once the level has been loaded, little or no dynamic memory allocation takes place. At the conclusion of the level, its data is unloaded and all of its memory can be freed. It makes a lot of sense to use a stack-like data structure for thesse kinds of memory allocations.

* **Double-Ended Stack Allocator**: A single memory block can actully contain two stack allocators, one that allocates
   up from the bottom of the block and one that allocates down from the top of the block.
   The implementation of it is much similar with StackAllocator, but have two markers instead
   The bottom marker always indicates the start of allocated memory,
   The top marker always indicates the end of allocated memory.

* **Pool Allocator**: In game there are many objects can be repeatedly used, such as every time player fires with a gun, a bullet is generated and may destroyed when hitting surface or went out of sight. In this situation we can reuse bullet instance in a pool instead of dynamically allocating/free a new bullet object repeatedly. For this type ofmemory allocation pattern, a pool allocator is often the perfect choice. 

* **Single Frame Allocator**: Sometimes data we need allocate at least some temporary data during the game loop.
   This data is either discarded at the end of each iteration of the loop or used on the
   next frame and then discarded. 
   
* **Double Buffered Allocator**: Different with Single Frame Allocator, it has two stack allocators. One serves as an allocator that is used in thie frame, the other allocator store the game data of last game loop.

## Future Work/Under Construction
Adding test cases/benchmark for all allocators. Remove redundant VS project files.
