#include <windows.h>

#include "config.h"

#include "../utilities/crt.h"
#include "../utilities/memory.h"

#include <filesystem>

#if defined(CS_CONFIGURATION_BINARY)
#include "../libs/json/binary.h"
#elif defined(CS_CONFIGURATION_JSON)
#include "../libs/json/json.h"
#elif defined(CS_CONFIGURATION_TOML)
#include "../libs/json/toml.h"
#endif

std::size_t C::UserDataType_t::GetSerializationSize() const
{
	std::size_t nTotalDataSize = 0U;

	for (const UserDataMember_t& member : vecMembers)
		nTotalDataSize += sizeof(FNV1A_t[2]) + member.nDataSize;

	return nTotalDataSize;
}

void C::VariableObject_t::SetStorage(const void* pValue)
{
	// check is available to store value in the local storage
	if (this->nStorageSize <= sizeof(this->storage.uLocal))
	{
		CRT::MemorySet(&this->storage.uLocal, 0U, sizeof(this->storage.uLocal));
		CRT::MemoryCopy(&this->storage.uLocal, pValue, this->nStorageSize);
	}
	// otherwise use heap memory to store it
	else
	{
		CS_ASSERT(this->storage.pHeap != nullptr); // tried to access non allocated storage

		CRT::MemorySet(this->storage.pHeap, 0U, this->nStorageSize);
		CRT::MemoryCopy(this->storage.pHeap, pValue, this->nStorageSize);
	}
}

std::size_t C::VariableObject_t::GetSerializationSize() const
{
	std::size_t nSerializationSize = this->nStorageSize;
	switch (this->uTypeHash)
	{
	case FNV1A::HashConst("bool[]"):
	case FNV1A::HashConst("int[]"):
	case FNV1A::HashConst("unsigned int[]"):
	case FNV1A::HashConst("float[]"):
	case FNV1A::HashConst("char[][]"):
		nSerializationSize += sizeof(std::size_t);
		break;
	default:
	{
		for (const UserDataType_t& userType : vecUserTypes)
		{
			if (userType.uTypeHash == this->uTypeHash)
			{
				nSerializationSize = sizeof(std::size_t) + userType.GetSerializationSize();
				break;
			}
		}
		break;
	}
	}

	return nSerializationSize;
}

bool C::Setup()
{
	AddUserType(FNV1A::HashConst("ColorPickerVar_t"),
	{
		UserDataMember_t{ FNV1A::HashConst("colPrimary"), FNV1A::HashConst("Color_t"), &ColorPickerVar_t::colValue },
	});

	AddUserType(FNV1A::HashConst("TextOverlayVar_t"), 
	{ 
		UserDataMember_t{ FNV1A::HashConst("bEnable"), FNV1A::HashConst("bool"), &TextOverlayVar_t::bEnable },
		UserDataMember_t{ FNV1A::HashConst("colPrimary"), FNV1A::HashConst("Color_t"), &TextOverlayVar_t::colPrimary },
		UserDataMember_t{ FNV1A::HashConst("colOutline"), FNV1A::HashConst("Color_t"), &TextOverlayVar_t::colOutline }
	});

	AddUserType(FNV1A::HashConst("FrameOverlayVar_t"),
	{
		UserDataMember_t{ FNV1A::HashConst("bEnable"), FNV1A::HashConst("bool"), &FrameOverlayVar_t::bEnable },
		UserDataMember_t{ FNV1A::HashConst("flThickness"), FNV1A::HashConst("float"), &FrameOverlayVar_t::flThickness },
		UserDataMember_t{ FNV1A::HashConst("flRounding"), FNV1A::HashConst("float"), &FrameOverlayVar_t::flRounding },
		UserDataMember_t{ FNV1A::HashConst("colPrimary"), FNV1A::HashConst("Color_t"), &FrameOverlayVar_t::colPrimary },
		UserDataMember_t{ FNV1A::HashConst("colOutline"), FNV1A::HashConst("Color_t"), &FrameOverlayVar_t::colOutline } 
	});

	AddUserType(FNV1A::HashConst("BarOverlayVar_t"),
	{ 
		UserDataMember_t{ FNV1A::HashConst("bEnable"), FNV1A::HashConst("bool"), &BarOverlayVar_t::bEnable },
		UserDataMember_t{ FNV1A::HashConst("bUseFactorColor"), FNV1A::HashConst("bool"), &BarOverlayVar_t::bUseFactorColor },
		UserDataMember_t{ FNV1A::HashConst("flThickness"), FNV1A::HashConst("float"), &BarOverlayVar_t::flThickness },
		UserDataMember_t{ FNV1A::HashConst("colPrimary"), FNV1A::HashConst("Color_t"), &BarOverlayVar_t::colPrimary },
		UserDataMember_t{ FNV1A::HashConst("colBackground"), FNV1A::HashConst("Color_t"), &BarOverlayVar_t::colBackground },
		UserDataMember_t{ FNV1A::HashConst("colOutline"), FNV1A::HashConst("Color_t"), &BarOverlayVar_t::colOutline } 
	});

	return true;
}

void C::AddUserType(const FNV1A_t uTypeHash, const std::initializer_list<UserDataMember_t> vecUserMembers)
{
	if (vecUserMembers.size() == 0U)
		return;

	UserDataType_t userDataType;
	userDataType.uTypeHash = uTypeHash;

	for (const auto& userDataMember : vecUserMembers)
		userDataType.vecMembers.push_back(userDataMember);

	vecUserTypes.emplace_back(CRT::Move(userDataType));
}

std::size_t C::GetVariableIndex(const FNV1A_t uNameHash)
{
	for (std::size_t i = 0U; i < vecVariables.size(); i++)
	{
		if (vecVariables[i].uNameHash == uNameHash)
			return i;
	}

	return C_INVALID_VARIABLE;
}