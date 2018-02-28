#include "FreeListAllocator.h"
#include "stdlib.h"
#include "Utility.h"

#include <cassert>
#include <iostream>
#include <climits>

FreeListAllocator::FreeListAllocator(const size_type allocatorSize_bytes)
{
	if (!start_ptr) {
		start_ptr = nullptr;
	}
	start_ptr = malloc(allocatorSize_bytes);
	this->clear();
}

FreeListAllocator::~FreeListAllocator()
{
	if (start_ptr != nullptr) {
		free(start_ptr);
	}
	start_ptr = nullptr;
}

void * FreeListAllocator::alloc(const size_type size_bytes, const size_type alignment)
{
	assert(size_bytes >= sizeof(Node) && "Allocation size must be larger than the additional header");
	size_type allocationHeaderSize = sizeof(AllocationHeader);

	size_type padding = 0;
	Node *tarNode = nullptr, *prevNode = nullptr;
	this->find(size_bytes, alignment, padding, prevNode, tarNode);

	if (tarNode == nullptr) {
		std::cout << "FreeListAllocator::alloc: No enough memory for allocation" << std::endl;
		return nullptr;
	}

	// Note: in StackAllocator, the required size equals to size_bytes + alignment, I implemented it different
	// way because I don't know which one is better. Technically, to make data alignment, added up padding
	// is enough. However, adding up alignment may make the block more "aligned"
	size_type requiredSize = size_bytes + padding;
	
	size_type rest = tarNode->val.blockSize - requiredSize;
	// there is enough space for a new block
	if (rest > allocationHeaderSize) {
		Node* newNode = reinterpret_cast<Node*>(reinterpret_cast<ptr_type>(tarNode) + requiredSize);
		newNode->val.blockSize = rest;
		newNode->val.padding = 0;
		free_list.insert(tarNode, newNode);
	}
	free_list.remove(prevNode, tarNode);

	ptr_type dataAddress = reinterpret_cast<ptr_type>(tarNode) + padding;
	// Add header information
	ptr_type headerAddress = dataAddress - allocationHeaderSize;
	AllocationHeader* header = reinterpret_cast<AllocationHeader*>(headerAddress);
	header->blockSize = requiredSize;
	header->padding = padding;

	return reinterpret_cast<void*>(dataAddress);
}

void FreeListAllocator::free(void * ptr)
{
	ptr_type dataAddress = reinterpret_cast<ptr_type>(ptr);
	size_type allocationHeaderSize = sizeof(AllocationHeader);
	ptr_type headerAddress = dataAddress - allocationHeaderSize;
	AllocationHeader* header = reinterpret_cast<AllocationHeader*>(headerAddress);

	// store first because it mgiht be overwritten by new node
	size_type padding = header->padding;
	size_type blockSize = header->blockSize;

	// create a new node locating at the start of the block
	ptrdiff_type newNodeAddress = dataAddress - padding;
	Node* newNode = reinterpret_cast<Node*>(newNodeAddress);
	newNode->val.blockSize = blockSize;
	newNode->val.padding = 0;
	newNode->next = nullptr;

	// found the freed node's previous node and insert it
	Node* it = free_list.head, *prevNode = nullptr;
	while (it != nullptr) {
		if (reinterpret_cast<ptr_type>(ptr) < reinterpret_cast<ptr_type>(it)) {
			free_list.insert(prevNode, newNode);
			break;
		}
		prevNode = it;
		it = it->next;
	}

	// merge two free blocks together
	coalesceTwoBlock(newNode, newNode->next);
	coalesceTwoBlock(prevNode, newNode);
}

void FreeListAllocator::clear()
{
	assert(total_size_bytes >= sizeof(Node) && "The allocator must be at least larger than size of Node");
	Node* headNode = reinterpret_cast<Node*>(start_ptr);
	headNode->val.blockSize = total_size_bytes;
	free_list.head = nullptr;
	headNode->next = nullptr;
	free_list.insert(nullptr, headNode);
}

inline FreeListAllocator::PlacementPolicy FreeListAllocator::currentAllocationPolicy() const
{
	return policy;
}

void FreeListAllocator::coalesceTwoBlock(Node * prevBlock, Node * freeBlock)
{
	if (freeBlock != nullptr &&
		reinterpret_cast<ptr_type>(prevBlock) + prevBlock->val.blockSize == reinterpret_cast<ptr_type>(freeBlock)) {
		prevBlock->val.blockSize += freeBlock->val.blockSize;
		free_list.remove(prevBlock, freeBlock);
	}
}

void FreeListAllocator::find(const size_type size, const size_type alignment, size_type & padding, Node *& prevNode, Node *& tarNode)
{
	if (policy == FIND_BEST_FIT) {
		findBestFit(size, alignment, padding, prevNode, tarNode);
	}
	else if (policy == FIND_FIRST_FIT) {
		findFirstFit(size, alignment, padding, prevNode, tarNode);
	}
	else {
		std::cout << "FreeListAllocator::find: no policy found" << std::endl;
	}
}
					
void FreeListAllocator::findBestFit(const size_type size, const size_type alignment, size_type & padding, Node *& prevNode, Node *& tarNode)
{
	// find the block that has the smallest blocksize difference with required size
	size_type mnDiff = std::numeric_limits<size_type>::max();
	Node* cand = nullptr, *candPrev = nullptr;
	size_type candPadding = 0, itPadding = 0;
	Node* it = free_list.head, *prev = nullptr;
	size_type allocationHeaderSize = sizeof(AllocationHeader);
	while (it) {
		itPadding = Utility::calculateAdjustmentForAlignmentWithHeader(reinterpret_cast<ptr_type>(it), alignment, allocationHeaderSize);
		size_type requiredSpace = size + itPadding;
		if (it->val.blockSize >= requiredSpace && it->val.blockSize - requiredSpace < mnDiff) {
			cand = it;
			candPrev = prev;
			candPadding = itPadding;
		}
		prev = it;
		it = it->next;
	}
	padding = candPadding;
	prevNode = candPrev;
	tarNode = cand;
}

void FreeListAllocator::findFirstFit(const size_type size, const size_type alignment, size_type & padding, Node *& prevNode, Node *& tarNode)
{
	Node* it = free_list.head, *prev = nullptr;
	size_type allocationHeaderSize = sizeof(AllocationHeader);

	// Once found the first block that is larger than required size,
	while (it) {
		padding = Utility::calculateAdjustmentForAlignmentWithHeader(reinterpret_cast<ptr_type>(it), alignment, allocationHeaderSize);
		size_type requiredSpace = padding + size;
		if (it->val.blockSize >= requiredSpace) {
			break;
		}
		prev = it;
		it = it->next;
	}
	prevNode = prev;
	tarNode = it;
}
