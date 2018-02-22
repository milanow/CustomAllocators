//#include "stdafx.h"

#include "StackAllocator.h"
#include "stdlib.h"
#include "Utility.h"
#include <iostream>

StackAllocator::StackAllocator(const size_type stackSize_bytes)
{
	start_ptr = malloc(stackSize_bytes);
	stack_size = stackSize_bytes;
	current_marker = 0;
}

StackAllocator::~StackAllocator()
{
	if (start_ptr != nullptr) {
		free(start_ptr);
	}
	start_ptr = nullptr;
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
	ptr_type currentAddress = reinterpret_cast<ptr_type>(start_ptr) + current_marker;

	if (current_marker + expandedSize_bytes > stack_size) {
		std::cout << "StackAllocator::alloc: the remain memory of stack allocator is not enough, consider free or clear the stack allocator first to continue allocate new memory" << std::endl;
		return nullptr;
	}

	// Calculate the adjustment
	ptrdiff_type adjustment = Utility::calculateAdjustmentForAlignmentWithHeader(currentAddress, alignment, sizeof(AdditionalHeader));

	// calculate the adjusted address
	ptr_type alignedAddress = currentAddress + adjustment;

	// TODO: Assertion area, will be changed after adding test framework
	// AdditionalHeader has a char to store the padding, a char can only store 1 - 255 
	if (adjustment > 255) {
		std::cout << "StackAllocator::alloc: AdditionalHeader is not enough space to storing padding data, consider redefine AdditionalHeader. " << std::endl;
		return;
	}

	// Add header preceding the adjusted address to store the adjusted address
	ptr_type headerAddress = alignedAddress - sizeof(AdditionalHeader);
	AdditionalHeader header{ adjustment };
	AdditionalHeader* headerptr = reinterpret_cast<AdditionalHeader*>(headerAddress);
	headerptr = &header;

	current_marker += expandedSize_bytes;
	return reinterpret_cast<void*>(alignedAddress);
}

void StackAllocator::free(void * ptr)
{
	// calculate the start position of this allocation according to the header's padding data
	ptr_type alignedAddress = reinterpret_cast<ptr_type>(ptr);
	ptr_type headerAddress = alignedAddress - sizeof(AdditionalHeader);
	AdditionalHeader* header{ (AdditionalHeader*)headerAddress };

	current_marker = alignedAddress - header->padding;
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
	current_marker = 0;
}
