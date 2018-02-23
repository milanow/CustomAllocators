#include "stdlib.h"
#include "DoubleEndedStackAllocator.h"
#include "Utility.h"

#include <iostream>

DoubleEndedStackAllocator::DoubleEndedStackAllocator(const size_type stackSize_bytes)
{
	start_ptr = malloc(stackSize_bytes);
	current_marker_bottom = 0;
	current_marker_top = stackSize_bytes;
	stack_size = stackSize_bytes;
	ptr_type endAddress = reinterpret_cast<ptr_type>(start_ptr) + stackSize_bytes;
	end_ptr = reinterpret_cast<void*>(endAddress);
}

DoubleEndedStackAllocator::~DoubleEndedStackAllocator()
{
	if (start_ptr != nullptr) {
		free(start_ptr);
	}
	start_ptr = nullptr;
	ptr_type endAddress = reinterpret_cast<ptr_type>(start_ptr) + stack_size;
	end_ptr = reinterpret_cast<void*>(endAddress);

}

void * DoubleEndedStackAllocator::alloc(const size_type size_bytes, const DoubleEndedStackAllocatorIndex index, const size_type alignment)
{
	// TODO: Assertion area, will be changed after adding test framework
	if (alignment < 1 || alignment > 128 || ((alignment & (alignment - 1))) != 0) {
		std::cout << "DoubleEndedStackAllocator::alloc: Alignment doesn't meet requirements" << std::endl;
		return nullptr;
	}

	// total size after alignment
	size_type expandedSize_bytes = size_bytes + alignment;
	ptr_type currentAddress;
	if (index == DoubleEndedStackAllocatorIndex::Bottom) {
		// current used memory in bottom stack
		currentAddress = reinterpret_cast<ptr_type>(start_ptr) + current_marker_bottom;

		if (current_marker_bottom + expandedSize_bytes > current_marker_top) {
			std::cout << "DoubleEndedStackAllocator::alloc: the remain memory of DoubleEndedStackAllocator is not enough, consider free or clear the stack allocator first to continue allocate new memory" << std::endl;
			return nullptr;
		}
	}
	else if (index == DoubleEndedStackAllocatorIndex::Top) {
		// current used memory in top stack, need to minus the alloc memory because current address
		// points to the tail of an allocate block
		ptr_type currentAddress = reinterpret_cast<ptr_type>(end_ptr) - current_marker_top - expandedSize_bytes;

		if (current_marker_top - expandedSize_bytes < current_marker_top) {
			std::cout << "DoubleEndedStackAllocator::alloc: the remain memory of DoubleEndedStackAllocator is not enough, consider free or clear the stack allocator first to continue allocate new memory" << std::endl;
			return nullptr;
		}
	}
	else {
		std::cout << "DoubleEndedStackAllocator::alloc: the double ended stack allocator index is incorrect" << std::cout;
		return nullptr;
	}

	// Calculate the adjustment
	ptrdiff_type adjustment = Utility::calculateAdjustmentForAlignmentWithHeader(currentAddress, alignment, sizeof(AdditionalHeader));
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

	if(index == DoubleEndedStackAllocatorIndex::Bottom) current_marker_bottom += expandedSize_bytes;
	else current_marker_top -= expandedSize_bytes;

	return reinterpret_cast<void*>(alignedAddress);
}

size_type DoubleEndedStackAllocator::getMarker(DoubleEndedStackAllocatorIndex index) const
{
	if (index == DoubleEndedStackAllocatorIndex::Bottom) return current_marker_bottom;
	else if (index == DoubleEndedStackAllocatorIndex::Top) return current_marker_top;
	else {
		std::cout << "DoubleEndedStackAllocator::getMarker: double ended stack allocator index is incorrect. " << std::endl;
		return 0;
	}
}

void DoubleEndedStackAllocator::freeToMaker(size_type marker, DoubleEndedStackAllocatorIndex index)
{
	if (index == DoubleEndedStackAllocatorIndex::Bottom) {
		if (marker > current_marker_bottom) {
			std::cout << "DoubleEndedStackAllocator::freeToMaker: marker is out of range of bottom allocated stack." << std::endl;
			return;
		}
		current_marker_bottom = marker;
	}
	else if (index == DoubleEndedStackAllocatorIndex::Top) {
		if (marker < current_marker_bottom) {
			std::cout << "DoubleEndedStackAllocator::freeToMaker: marker is out of range of top allocated stack." << std::endl;
			return;
		}
		current_marker_top = marker;
	}
}

void DoubleEndedStackAllocator::clear()
{
	current_marker_bottom = 0;
	current_marker_top = stack_size;
}
