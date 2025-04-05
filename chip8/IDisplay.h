#pragma once

class IDisplay
{
	public:
		virtual void DrawInstruction(unsigned char* graphicsPtr) = 0;
};
