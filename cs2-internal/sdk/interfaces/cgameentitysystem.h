#pragma once

// used: schema field
#include "../../utilities/memory.h"

#include "../entity_handle.h"

#define MAX_ENTITIES_IN_LIST 512
#define MAX_ENTITY_LISTS 64 // 0x3F
#define MAX_TOTAL_ENTITIES MAX_ENTITIES_IN_LIST* MAX_ENTITY_LISTS

class C_BaseEntity;

class CGameEntitySystem
{
public:
	/// GetClientEntity
	template <typename T = C_BaseEntity>
	T* Get(int nIndex)
	{
		return reinterpret_cast<T*>(this->GetEntityByIndex(nIndex));
	}

	/// GetClientEntityFromHandle
	template <typename T = C_BaseEntity>
	T* Get(const CBaseHandle hHandle)
	{
		if (!hHandle.IsValid())
			return nullptr;

		return reinterpret_cast<T*>(this->GetEntityByIndex(hHandle.GetEntryIndex()));
	}

	int GetHighestEntityIndex()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + 0x20F0);
	}

private:
	void* GetEntityByIndex(int nIndex)
	{
		//@ida: #STR: "(missing),", "(missing)", "Ent %3d: %s class %s name %s\n" | or find "cl_showents" cvar -> look for callback
		//	do { pEntity = GetBaseEntityByIndex(g_pGameEntitySystem, nCurrentIndex); ... }
		using fnGetBaseEntity = void*(CS_THISCALL*)(void*, int);
		static auto GetBaseEntity = reinterpret_cast<fnGetBaseEntity>(MEM::FindPattern(CLIENT_DLL, CXOR("81 FA FE 7F 00 00 77 36")));
		return GetBaseEntity(this, nIndex);
	}
};

enum CSWeaponID {
	GLOCK = 1,
	USP_S = 2,
	P2000 = 3,
	DUAL_BERETTAS = 4,
	P250 = 5,
	TEC9 = 6,
	FIVE_SEVEN = 7,
	DESERT_EAGLE = 8,

	MAC10 = 17,
	MP9 = 18,
	MP7 = 19,
	UMP45 = 24,
	P90 = 26,

	GALIL_AR = 13,
	FAMAS = 14,
	AK47 = 10,
	M4A4 = 16,
	M4A1_S = 20,
	AUG = 23,
	SG553 = 27,

	AWP = 9,
	G3SG1 = 11,
	SCAR20 = 38,

	M249 = 28,
	NEGEV = 35
};