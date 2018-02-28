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

class SingleFrameAllocator: StackAllocator 
{
	SingleFrameAllocator(const size_type stackSize_bytes) : StackAllocator(stackSize_bytes) {}
};