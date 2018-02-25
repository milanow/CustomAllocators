/* Created 02/23/2018
Author: Tianhe Wang
Email: wthvictor@gmail.com
*/

#pragma once

#include "LinkedList.h"

#include <cstdint>

/* It's quite common in game engine programming (and software engineering in general) to allocate lots of 
   small bnlocks of memory, each oof which are the same size. For example, we might want to allocate and 
   free matrices, or iterators, or links in a linked list, or renderable mesh instances. For this type of
   memory allocation pattern, a pool allocator is often the perfect choice. -- "Game Engine Architecture"
*/

// size_type represents size
// ptr represents pointer location
// ptrdiff represents pointer location difference
typedef uint32_t size_type;
typedef uintptr_t ptr_type;
typedef ptrdiff_t ptrdiff_type;

// T doesn't have to be a class, but to make it simple(class is larger than a pointer), I only support class here
template<class T>
class PoolAllocator {
public:
	// Allocator cannot be coppied
	PoolAllocator(const PoolAllocator& allocator) = delete;
	PoolAllocator& operator=(const PoolAllocator& allocator) = delete;

	// Constructor
	explicit PoolAllocator(const size_type numberOfElements);

	// Destructor
	~PoolAllocator();

	T* alloc();

	void free(void* ptr);

	// Return the number of elements the pool could have, meaning free+used
	size_type capacity() const;

	// Return the pool size, comparing capacity() function
	size_type capacityInBytes() const;

	// Clear and reset pool
	void clear();

private:
	// size per element
	size_type chunk_size;
	// number of elements in total
	size_type pool_size;
	// pool size
	size_type pool_size_bytes;
	// starting address of the pool
	void* start_ptr;
	
	struct FreeHeader {};
	LinkedList<FreeHeader> free_list;
};