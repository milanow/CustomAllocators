#include "PoolAllocator.h"

#include <assert.h>
#include <iostream>

template<class T>
PoolAllocator<T>::PoolAllocator(const size_type numberOfElements)
{
	pool_size = numberOfElements;
	chunk_size = sizeof(T);
	// In general, the pool_size_bytes won't be larger than an unsigned int, so no need to check here
	pool_size_bytes = pool_size * chunk_size;
	start_ptr = malloc(pool_size_bytes);
	free_list = LinkedList<FreeHeader>(nullptr);

	for (int i = 0; i < numberOfElements; i++) {
		size_type address = reinterpret_cast<size_type>(start_ptr) + i*chunk_size;
		free_list.push((Node*)address);
	}
}

template<class T>
PoolAllocator<T>::~PoolAllocator()
{
	if (start_ptr != nullptr) {
		free(start_ptr);
	}
	start_ptr = nullptr;
}

template<class T>
T * PoolAllocator<T>::alloc()
{
	Node* freeNode = free_list.pop();
	
	if (freeNode == nullptr) {
		std::cout << 
		return nullptr;
	}

	return nullptr;
}

template<class T>
void PoolAllocator<T>::free(void * ptr)
{
	free_list.push((Node*)ptr);
}

template<class T>
inline size_type PoolAllocator<T>::capacity() const
{
	return pool_size;
}

template<class T>
size_type PoolAllocator<T>::capacityInBytes() const
{
	return pool_size_bytes;
}

template<class T>
void PoolAllocator<T>::clear()
{
	free_list = LinkedList<FreeHeader>(nullptr);

	for (int i = 0; i < numberOfElements; i++) {
		size_type address = reinterpret_cast<size_type>(start_ptr) + i*chunk_size;
		free_list.push((Node*)address);
	}
}
