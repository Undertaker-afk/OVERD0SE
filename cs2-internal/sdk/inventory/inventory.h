#pragma once
#include "../../utilities/memory.h"
#include "../entity.h"


class CCSInventoryManager {
public:
	bool EquipItemInLoadout(int nTeam, int nSlot, std::uint64_t nItemID) {
		return MEM::CallVFunc<bool, 60u>(this, nTeam, nSlot, nItemID);
	}

	CCSPlayerInventory* GetLocalInventory() {
		return MEM::CallVFunc<CCSPlayerInventory*, 63u>(this);
	}
};





class CSharedObject;










class CEconItem
{
	void SetDynamicAttributeValue(int index, void* value);
	void SetDynamicAttributeValueString(int index, const char* value);

public:
	static CEconItem* CreateInstance();

	void Destruct()
	{
		return MEM::CallVFunc<void, 1U>(this);
	}

	void SetPaintKit(float kit)
	{
		SetDynamicAttributeValue(6, &kit);
	}

	void SetPaintSeed(float seed)
	{
		SetDynamicAttributeValue(7, &seed);
	}

	void SetPaintWear(float wear)
	{
		SetDynamicAttributeValue(8, &wear);
	}

	void SetStatTrak(int count)
	{
		SetDynamicAttributeValue(80, &count);
	}

	void SetStatTrakType(int type)
	{
		SetDynamicAttributeValue(81, &type);
	}

	void SetCustomName(const char* pName)
	{
		SetDynamicAttributeValueString(111, pName);
	}
	void SetKeychains(int id) { SetDynamicAttributeValue(299, &id); }
	char pad0[0x10]; // 2 vtables
	uint64_t m_ulID;
	uint64_t m_ulOriginalID;
	void* m_pCustomDataOptimizedObject;
	uint32_t m_unAccountID;
	uint32_t m_unInventory;
	uint16_t m_unDefIndex;
	uint16_t m_unOrigin : 5;
	uint16_t m_nQuality : 4;
	uint16_t m_unLevel : 2;
	uint16_t m_nRarity : 4;
	uint16_t m_dirtybitInUse : 1;
	int16_t m_iItemSet;
	int m_bSOUpdateFrame;
	uint8_t m_unFlags;
};

class CCSPlayerInventory {
public:
	auto SOCreated(SOID_t owner, CSharedObject* pObject, int eEvent) {
		return MEM::CallVFunc<void, 0u>(this, owner, pObject, eEvent);
	}

	auto SOUpdated(SOID_t owner, CSharedObject* pObject, int eEvent) {
		return MEM::CallVFunc<void, 1u>(this, owner, pObject, eEvent);
	}

	auto SODestroyed(SOID_t owner, CSharedObject* pObject, int eEvent) {
		return MEM::CallVFunc<void, 2u>(this, owner, pObject, eEvent);
	}

	auto GetItemInLoadout(int iClass, int iSlot) {
		return MEM::CallVFunc<C_EconItemView*, 8u>(this, iClass, iSlot);
	}

	bool AddEconItem(CEconItem* pItem);
	void RemoveEconItem(CEconItem* pItem);
	std::pair<uint64_t, uint32_t> GetHighestIDs();
	C_EconItemView* GetItemViewForItem(uint64_t itemID);
	CEconItem* GetSOCDataForItem(uint64_t itemID);

	auto GetOwner()
	{
		return *reinterpret_cast<SOID_t*>((uintptr_t)(this) + 0x10);
	}

	auto& GetItemVector()
	{
		return *reinterpret_cast<CUtlVector<C_EconItemView*>*>(
			(uintptr_t)(this) + 0x20);
	}
};






class CEconItemDefinition
{
public:
	bool is_custom_player();
	bool IsWeapon();
	bool IsKnife(bool excludeDefault);
	bool IsGlove(bool excludeDefault);
	bool IsAgent();

	uint8_t GetRarity()
	{
		return *reinterpret_cast<uint8_t*>((uintptr_t)(this) + 0x42);
	}

	auto GetModelName()
	{
		return *reinterpret_cast<const char**>((uintptr_t)(this) + 0xD8);
	}

	auto GetStickersSupportedCount()
	{
		return *reinterpret_cast<int*>((uintptr_t)(this) + 0x100); // 0x118
	}

	auto GetSimpleWeaponName()
	{
		return *reinterpret_cast<const char**>((uintptr_t)(this) + 0x210);
	}

	auto GetName()
	{
		return *reinterpret_cast<const char**>((uintptr_t)(this) + 0x70);
	}

	auto GetLoadoutSlot()
	{
		return *reinterpret_cast<int*>((uintptr_t)(this) + 0x2E8);
	}

	char pad0[0x8]; // vtable
	void* m_pKVItem;
	uint16_t m_nDefIndex;
	CUtlVector<uint16_t> m_nAssociatedItemsDefIndexes;
	bool m_bEnabled;
	const char* m_szPrefab;
	uint8_t m_unMinItemLevel;
	uint8_t m_unMaxItemLevel;
	uint8_t m_nItemRarity;
	uint8_t m_nItemQuality;
	uint8_t m_nForcedItemQuality;
	uint8_t m_nDefaultDropItemQuality;
	uint8_t m_nDefaultDropQuantity;
	CUtlVector<void*> m_vecStaticAttributes;
	uint8_t m_nPopularitySeed;
	void* m_pPortraitsKV;
	const char* m_pszItemBaseName;
	bool m_bProperName;
	const char* m_pszItemTypeName;
	uint32_t m_unItemTypeID;
	const char* m_pszItemDesc;
};