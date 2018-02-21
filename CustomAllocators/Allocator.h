/* Created 11/14/2017 
	Author: Tianhe Wang
	Email: wthvictor@gmail.com
*/

/* This is a custom allocator where still using malloc & free to
	alloc & free memory. However, classic malloc/new & free/delete is
	a critical performance hit due to context switch between user mode
	with kernal mode.

	In game development, dynamic allocation should be reduced as less as possible.
	Thus many allocators that game uses is actually "fake allocator",
	Which means the memory is not managed by OS but manually.
	They allocate enough memory and implement different allocators,
	such as Stack Allocator, Pool Allocator, Single-Frame Allocator, Double-Buffered
	Allocator.
*/

#pragma once

#include <cstddef>

extern size_t gAllocateedSize;

template <typename T> 
class Allocator 
{
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

	/// Default constructor
	Allocator() noexcept;

	/// Copy constructor
	Allocator(const Allocator& other) noexcept;

	/// Copy constructor with another type
	template<typename U>
	Allocator(const Allocator<U>&) noexcept;

	/// Copy
	Allocator<T>& operator=(const Allocator& other);

	/// Copy with another type
	template<typename U>
	Allocator& operator=(const Allocator<U>& other);

	/// Get the address of a reference
	pointer address(reference x) const;

	/// Get the address of a const reference
	const_pointer address(const_reference x) const;

	/// Allocate n elements of type T
	pointer allocate(size_type n, const void* hint = 0);

	/// Free memory of pointer p
	void deallocate(void* p, size_type n);

	/// Call the constructor of p
	void construct(pointer p, const T& val);

	/// Call the constructor of p with many arguments
	template<typename U, typename... Args>
	void construct(U* p, Args&&... agrs);

	/// Call the destructor of p
	void destroy(pointer p);

	/// Call the destructor of p of type U
	template<typename U>
	void destroy(U* p);

	/// Get the max allocation size
	size_type max_size() const;

	/// A struct to rebind the allocator to another allocator of type
	template<typename U>
	struct rebind
	{
		typedef Allocator<U> other;
	};
};