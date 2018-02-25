/* Created 02/22/2018
Author: Tianhe Wang
Email: wthvictor@gmail.com
*/

#pragma once
#include <cstdint>

/* A single memory block can actully contain two stack allocators, one that allocates
   up from the bottom of the block and one that allocates down from the top of the block.
   The implementation of it is much similar with StackAllocator, but have two markers instead
   The bottom marker always indicates the start of allocated memory,
   The top marker always indicates the end of allocated memory.
*/

// size_type represents size
// ptr represents pointer location
// ptrdiff represents pointer location difference
typedef uint32_t size_type;
typedef uintptr_t ptr_type;
typedef ptrdiff_t ptrdiff_type;


enum DoubleEndedStackAllocatorIndex
{
	Bottom, Top
};

class DoubleEndedStackAllocator 
{
public:
	DoubleEndedStackAllocator(const DoubleEndedStackAllocator& allocator) = delete;
	DoubleEndedStackAllocator& operator=(const DoubleEndedStackAllocator& allocator) = delete;

	// Construct with total size
	explicit DoubleEndedStackAllocator(const size_type stackSize_bytes);

	// Destructor for releasing memory back to OS
	~DoubleEndedStackAllocator();

	// Allocates a new block of given size from stack with Top/Bottom option
	void* alloc(const size_type size_bytes, const DoubleEndedStackAllocatorIndex index, const size_type alignment);

	// Returns a Top/Bottom marker to the current stack top
	size_type getMarker(DoubleEndedStackAllocatorIndex index) const;

	// Rolls the stack back to a previous marker with Top/Bottom option
	void freeToMaker(size_type marker, DoubleEndedStackAllocatorIndex index);

	// Reset the entire stack (rolls the stack back to orginal state)
	void clear();

private:
	// Bottom of a stack allocator
	void* start_ptr;
	// Top of a stack allocator
	void* end_ptr;
	// Bottom marker
	ptr_type current_marker_bottom;
	// Top marker
	ptr_type current_marker_top;
	// Stack size
	size_type stack_size;

	// Header info storing padding;
	struct AdditionalHeader {
		char padding;
	};

};