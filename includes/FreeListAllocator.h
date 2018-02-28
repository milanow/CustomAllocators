/* Created 02/26/2018
Author: Tianhe Wang
Email: wthvictor@gmail.com
*/

/* The basic requirement of custom allocator is to avoid context switch cost in runtime
   malloc/new is designed for all kinds of memory allocation and it might be a critical
   performance hit in game prograemming.

   FreelistAllocator is a custom allocator that allows allocating any bytes (larger than a 
   header actually, because it wastes memory) and free the memory with O(1) time. It uses
   a linkedlist to connect each memory blocks together and use info int blocks' headers to 
   store the blocks' info (blocksize & alignment ajustment).
*/

#pragma once

#include "LinkedList.h"

#include <cstdint>

// size_type represents size
// ptr represents pointer location
// ptrdiff represents pointer location difference
typedef uint32_t size_type;
typedef uintptr_t ptr_type;
typedef ptrdiff_t ptrdiff_type;

class FreeListAllocator {
public:
	// Free list allocator has two strategy to return available memory
	// Since memory is cutted into blocks, when allocating new memory,
	// we need to find out free block where sizeof(freeblock) >= requiredMemory
	// Find_best means finding the best block whose size is as close as target allocation
	// memory as possible
	// Fiund_first means when finding out the first block that is large enough for the
	// allocation request, return it.
	enum PlacementPolicy {
		FIND_BEST_FIT,
		FIND_FIRST_FIT
	};

private:
	// The allocation header of a block store the padding length and the block size
	struct AllocationHeader {
		size_type blockSize;
		char padding;
	};

	typedef LinkedList<AllocationHeader>::Node Node;

	void* start_ptr;
	size_type total_size_bytes;
	PlacementPolicy policy;
	// a linkedlist connecting all our free blocks
	// memory blocks are stored in the order of their addresses
	LinkedList<AllocationHeader> free_list;
	
public:
	// Allocator cannot be copied
	FreeListAllocator(const FreeListAllocator& other) = delete;
	FreeListAllocator& operator=(const FreeListAllocator& other) = delete;

	// construct a freelist allocator with total size
	explicit FreeListAllocator(const size_type allocatorSize_bytes);

	// destructor
	~FreeListAllocator();

	// allocating a new block of memory
	void* alloc(const size_type size_bytes, const size_type alignment = 16);

	// realse memory back to allocator
	void free(void* ptr);

	// reset the allocator
	void clear();

	// return current placement policy
	PlacementPolicy currentAllocationPolicy() const;

private:
	// after a block being freed, there is a chance that two blocks can be merged together to
	// make a big block
	void coalesceTwoBlock(Node* prevBlock, Node* freeBlock);

	void find(const size_type size, const size_type alignment, size_type& padding, Node*& prevNode, Node*& tarNode);
	void findBestFit(const size_type size, const size_type alignment, size_type& padding, Node*& prevNode, Node*& tarNode);
	void findFirstFit(const size_type size, const size_type alignment, size_type& padding, Node*& prevNode, Node*& tarNode);
};