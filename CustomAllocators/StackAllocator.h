#pragma once
#include <cstdint>

// for better readability
// size_type represents size
// ptr represents pointer location
// ptrdiff represents pointer location difference
typedef uint32_t size_type;
typedef uintptr_t ptr_type;
typedef ptrdiff_t ptrdiff_type;

class StackAllocator 
{
public:
	// Stack marker: Represents the current top of
	// stack, You can only roll back to a marker, not to
	// arbitary locations within the stack

	// Allocator cannot be coppied
	StackAllocator(const StackAllocator& allocator) = delete;
	StackAllocator& operator=(const StackAllocator& allocator) = delete;

	// Constructs a stack allocator with the given total size
	explicit StackAllocator(const size_type stackSize_bytes);

	// Destructor for releasing memory back to OS
	~StackAllocator();

	// Allocates a new block of the given size from stack
	void* alloc(const size_type size_bytes, const size_type alignment = 16);

	// Returns a marker to the current stack top
	size_type getMarker() const;

	// Rolls the stack back to a previous marker
	void freeToMarker(size_type marker);

	// Clears the entire stack (rolls the stack back to zero)
	void clear();

private:
	// Start location of stack allocator
	void* start_ptr;

	// Stack top
	ptr_type current_marker;

	// Total stack size/ability
	size_type stack_size;
};

