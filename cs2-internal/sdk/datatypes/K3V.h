#pragma once
#include <tchar.h>
#include <wchar.h>
#include <stdint.h>
#include <cstddef>

#include "../../utilities/common.h"

class CKeyValues32
{
public:
	uint64_t uKey;
	void* pValue;

	std::byte pad[0x8];
};

struct KV32IVD_t
{
	const char* szName;
	uint64_t unk0;
	uint64_t unk1;
};
