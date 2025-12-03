#include "RenderBufferPair.h"

RenderBufferPair::RenderBufferPair()
	: myBackIndex(0)
{
}

RenderBufferPair::~RenderBufferPair()
{
}

RenderWorld& RenderBufferPair::Back()
{
	return myBuffers[myBackIndex];
}

const RenderWorld& RenderBufferPair::Front() const
{
	return myBuffers[1 - myBackIndex];
}

void RenderBufferPair::Swap()
{
	std::lock_guard<std::mutex> lock(myMutex);
	myBackIndex = 1 - myBackIndex;
}

