#pragma once

#include "stdlib.h"

class Utility {
public:
	// Calculate the adjustment by masking off the lower bits
	// of the address to determine how "misaligned" it is
	static ptrdiff_t calculateAdjustmentForAlignment(const uintptr_t address, const size_t alignment) {
		size_t mask = (alignment - 1);
		uintptr_t misalignment = (address & mask);
		ptrdiff_t adjustment = alignment - misalignment;
		return adjustment;
	}

	// Calculate the adjustment plus addtional header to store extra info
	static ptrdiff_t calculateAdjustmentForAlignmentWithHeader(const uintptr_t address, const size_t alignment, const size_t headersize) {
		ptrdiff_t adjustment = calculateAdjustmentForAlignment(address, alignment);
		size_t spaceneed = headersize;

		// need extra space to store the header
		if (adjustment < spaceneed) {
			spaceneed -= adjustment;
			auto n = spaceneed / alignment;
			adjustment += alignment * (n + (spaceneed % alignment) ? 1 : 0);
		}
		return adjustment;
	}


};