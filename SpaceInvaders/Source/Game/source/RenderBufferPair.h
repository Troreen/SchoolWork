#pragma once
#include "RenderTypes.h"
#include <mutex>

class RenderBufferPair
{
public:
	RenderBufferPair();
	~RenderBufferPair();

	RenderWorld& Back();
	const RenderWorld& Front() const;

	void Swap();
private:
	RenderWorld myBuffers[2];
	int myBackIndex;
	std::mutex myMutex;

};

