#include "pch.h"
#include "StackAllocator.h"
#include "stdlib.h"
#include <iostream>

StackAllocator::StackAllocator(const size_type stackSize_bytes)
{
	if (start_ptr != nullptr) {
		clear();
	}
	start_ptr = malloc(stackSize_bytes);
	stack_size = stackSize_bytes;
	current_marker = 0;
}

void * StackAllocator::alloc(const size_type size_bytes, const size_type alignment)
{
	// TODO: Assertion area, will be changed after adding test framework
	if (alignment < 1 || alignment > 128 || ((alignment & (alignment - 1))) != 0) {
		std::cout << "StackAllocator::alloc: Alignment doesn't meet requirements" << std::endl;
		return nullptr;
	}

	// the total size after alignment
	size_type expandedSize_bytes = size_bytes + alignment;
	// current used memory in stack
	ptr_type currentAddress = (ptr_type)start_ptr + current_marker;

	if (current_marker + expandedSize_bytes > stack_size) {
		std::cout << "StackAllocator::alloc: the remain memory of stack allocator is not enough, consider free or clear the stack allocator first to continue allocate new memory" << std::endl;
		return nullptr;
	}

	// Calculate the adjustment by masking off the lower bits
	// of the addressm to determine how "misaligned" it is
	size_type mask = (alignment - 1);
	ptr_type misalignment = (currentAddress & mask);
	ptrdiff_type adjustment = alignment - misalignment;

	// calculate the adjusted address
	ptr_type alignedAddress = currentAddress + adjustment;

	// TODO: Assertion area, will be changed after adding test framework
	if (adjustment > 256) {
		std::cout << "StackAllocator::alloc: adjustment is larger than alignment, something wrong" << std::endl;
		return nullptr;
	}
	// Store the adjustment in the byte immediately
	// preceding the adjusted address
	uint8_t* pAliognedMem = reinterpret_cast<uint8_t*>(alignedAddress);
	pAliognedMem[-1] = static_cast<uint8_t>(adjustment);

	current_marker += expandedSize_bytes;
	return static_cast<void*>(pAliognedMem);
}

size_type StackAllocator::getMarker() const
{
	return current_marker;
}

void StackAllocator::freeToMarker(size_type marker)
{
	// TODO: Assertion area, will be changed after adding test framework
	if (marker > stack_size) {
		std::cout << "StackAllocator::freeToMarker: marker is out of range of stack size" << std::endl;
		return;
	}

	current_marker = marker;
}

void StackAllocator::clear()
{
}
