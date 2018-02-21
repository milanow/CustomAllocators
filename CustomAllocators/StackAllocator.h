#pragma once
#include <cstdint>

typedef uint32_t Marker;

class StackAllocator 
{
public:
	// Stack marker: Represents the current top of
	// stack, You can only roll back to a marker, not to
	// arbitary locations within the stack
	StackAllocator(uint32_t stackSize_bytes);

	// Allocator cannot be coppied
	StackAllocator(const StackAllocator& allocator) = delete;
	StackAllocator& operator=(const StackAllocator& allocator) = delete;

	// Constructs a stack allocator with the given total size
	explicit StackAllocator(uint32_t stackSize_bytes);

	// Allocates a new block of the given size from stack
	void* alloc(const uint32_t size_bytes, const uint32_t alignment = 16);

	// Returns a marker to the current stack top
	Marker getMarker();

	// Rolls the stack back to a previous marker
	void freeToMarker(Marker marker);

	// Clears the entire stack (rolls the stack back to zero)
	void clear();

private:
	void* start_ptr;

	Marker current_marker;

	uint32_t stack_size;
};

