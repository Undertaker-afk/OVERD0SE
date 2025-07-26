#pragma once
class CGCClient;
class CCSGCClientSystem
{
public:
	CGCClient* GetCGCClient()
	{
		return reinterpret_cast<CGCClient*>((uintptr_t)(this) + 0xB8);
	};
};
