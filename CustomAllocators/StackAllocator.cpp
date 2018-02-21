#include "pch.h"
#include "StackAllocator.h"
#include "stdlib.h"

StackAllocator::StackAllocator(const uint32_t stackSize_bytes)
{
	if (start_ptr != nullptr) {
		clear();
	}
	start_ptr = malloc(stackSize_bytes);
	stack_size = stackSize_bytes;
	current_marker = 0;
}

void * StackAllocator::alloc(const uint32_t size_bytes, const uint32_t alignment)
{
	return nullptr;
}

Marker StackAllocator::getMarker()
{
	return Marker();
}

void StackAllocator::freeToMarker(Marker marker)
{
}

void StackAllocator::clear()
{
}
