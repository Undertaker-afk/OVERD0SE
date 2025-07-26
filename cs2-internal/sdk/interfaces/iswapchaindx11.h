#pragma once

#include "../../utilities/memory.h"

struct IDXGISwapChain;

class ISwapChainDx11
{
	MEM_PAD(0x170);
	IDXGISwapChain* pDXGISwapChain;
	IDXGIFactory* pIDXGIFactory;
};