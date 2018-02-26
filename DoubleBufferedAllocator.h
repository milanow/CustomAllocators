/* Created 02/26/2018
Author: Tianhe Wang
Email: wthvictor@gmail.com
*/

/* Virually all game engines allocate at least some temporary data during the game loop.
This data is either discarded at the end of each iteration of the loop or used on the
next frame and then discarded. This allocation pattern is so common that many engines
support single-frame and double-buffered allocators. - "Game Engine Architecture"
*/

#pragma once

#include "StackAllocator.h"

typedef uint32_t U32;

class DoubleBufferedAllocator {
public:
	// Allocator cannot be coppied
	DoubleBufferedAllocator(const DoubleBufferedAllocator& allocator) = delete;
	DoubleBufferedAllocator& operator=(const DoubleBufferedAllocator& allocator) = delete;

	// Constructs a DoubleBufferedAllocator with the size of each stacks
	explicit DoubleBufferedAllocator(const U32 stackSize_bytes);

	// Release memory back to OS
	~DoubleBufferedAllocator();

	// Swap to available stack allocator
	void swapBuffer();

	// Clear current stack allocator
	void clearCurrentBuffer();

	// allocate memory
	void* alloc(const U32 nBytes, const U32 alignment = 16);

	// No need to have a free methods because all allocated memory will be released by clear()
	void clearCurrentBuffer();

private:
	// Index of stack array, indicating current active stack
	U32 m_curStack;
	// Stack array
	StackAllocator* m_stack[2];
};

DoubleBufferedAllocator::DoubleBufferedAllocator(const U32 stackSize_bytes)
{
	m_curStack = 0;
	m_stack[0] = new StackAllocator(stackSize_bytes);
	m_stack[1] = new StackAllocator(stackSize_bytes);
}

inline DoubleBufferedAllocator::~DoubleBufferedAllocator()
{
	delete m_stack[0];
	delete m_stack[1];
}

inline void DoubleBufferedAllocator::swapBuffer()
{
	m_curStack = (U32)!m_curStack;
}

inline void * DoubleBufferedAllocator::alloc(const U32 nBytes, const U32 alignment)
{
	return m_stack[m_curStack]->alloc(nBytes, alignment);
}

inline void DoubleBufferedAllocator::clearCurrentBuffer()
{
	m_stack[m_curStack]->clear();
}