#include <vector>
#include "../../../sdk/inventory/gcsystem.h"
#include "../../../sdk/inventory/inventory.h"
#include "inventorychanger.h"
#include <iostream>
#include <memoryapi.h>
#include <mutex>
#include <array>
#include "../../../sdk/interfaces/cgameentitysystem.h"
#include "../../sdk/interfaces/iengineclient.h"
#include "../../sdk/datatypes/utlstringtoken.h"
#include "../../sdk/interfaces/events.h"
inline uint32_t MurmurHash2(const void* key, int len, uint32_t seed)
{
	/* 'm' and 'r' are mixing constants generated offline.
	   They're not really 'magic', they just happen to work well.  */

	const uint32_t m = 0x5bd1e995;
	const int r = 24;

	/* Initialize the hash to a 'random' value */

	uint32_t h = seed ^ len;

	/* Mix 4 bytes at a time into the hash */

	const unsigned char* data = (const unsigned char*)key;

	while (len >= 4)
	{
		uint32_t k = *(uint32_t*)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	/* Handle the last few bytes of the input array  */

	switch (len)
	{
	case 3:
		h ^= data[2] << 16;
	case 2:
		h ^= data[1] << 8;
	case 1:
		h ^= data[0];
		h *= m;
	};

	/* Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.  */

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

#define TOLOWERU(c) ((uint32_t)(((c >= 'A') && (c <= 'Z')) ? c + 32 : c))

inline uint32_t MurmurHash2LowerCaseA1(const char* pString, int len, uint32_t nSeed)
{
	char* p = (char*)malloc(len + 1);
	for (int i = 0; i < len; i++)
	{
		p[i] = TOLOWERU(pString[i]);
	}
	return MurmurHash2(p, len, nSeed);
}
class CUtlStringToken1
{
public:
	std::uint32_t m_nHashCode;
#if DEBUG_STRINGTOKENS
	const char* m_pDebugName;
#endif

	CUtlStringToken1(const char* szString)
	{
		this->SetHashCode1(this->MakeStringToken1(szString));
	}

	bool operator==(const CUtlStringToken1& other) const
	{
		return (other.m_nHashCode == m_nHashCode);
	}

	bool operator!=(const CUtlStringToken1& other) const
	{
		return (other.m_nHashCode != m_nHashCode);
	}

	bool operator<(const CUtlStringToken1& other) const
	{
		return (m_nHashCode < other.m_nHashCode);
	}

	/// access to the hash code for people who need to store thse as 32-bits, regardless of the
	/// setting of DEBUG_STRINGTOKENS (for instance, for atomic operations).
	uint32_t GetHashCode1(void) const
	{
		return m_nHashCode;
	}

	void SetHashCode1(uint32_t nCode)
	{
		m_nHashCode = nCode;
	}

	__forceinline std::uint32_t MakeStringToken1(const char* szString, int nLen)
	{
		std::uint32_t nHashCode = MurmurHash2LowerCaseA1(szString, nLen, STRINGTOKEN_MURMURHASH_SEED);
		return nHashCode;
	}

	__forceinline std::uint32_t MakeStringToken1(const char* szString)
	{
		return MakeStringToken1(szString, (int)strlen(szString));
	}

	//__forceinline std::uint32_t MakeStringToken(CUtlString& str)
	//{
	//    return MakeStringToken(str.Get(), str.Length());
	//}

	CUtlStringToken1()
	{
		m_nHashCode = 0;
	}
};

void changers::OnFrameStageNotify(int frameStage)
{
	if (frameStage != 6 and frameStage != 3)
		return;

	CCSPlayerInventory* pInventory = GSF::pInventoryManager->GetLocalInventory();
	if (!pInventory)
		return;

	CCSPlayerController* pLocalPlayerController = CCSPlayerController::GetLocalPlayerController();
	if (!pLocalPlayerController)
		return;

	C_CSPlayerPawn* pLocalPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pLocalPlayerController->GetPawnHandle());
	if (!pLocalPawn)
		return;

	if (pLocalPawn->GetHealth() <= 0)
		return;

	CGameEntitySystem* pEntitySystem = I::GameResourceService->pGameEntitySystem;
	if (!pEntitySystem)
		return;

	const uint64_t steamID = pInventory->GetOwner().m_id;

	CCSPlayer_ViewModelServices* pViewModelServices = pLocalPawn->GetViewModelServices();
	if (!pViewModelServices)
		return;

	C_CSGOViewModel* pViewModel = I::GameResourceService->pGameEntitySystem->Get<C_CSGOViewModel>(pViewModelServices->m_hViewModel());
	if (!pViewModel)
		return;

	if (frameStage == 6)
		changers::GloveChanger(pLocalPawn);
	changers::AgentChanger(pLocalPawn);
	if (frameStage == 6)
		changers::KnifeChanger(pLocalPawn);

	int highestIndex = pEntitySystem->GetHighestEntityIndex();
	for (int i = 64 + 1; i <= highestIndex; ++i)
	{
		C_BaseEntity* pEntity = pEntitySystem->Get(i);
		if (!pEntity || !pEntity->IsWeapon())
			continue;

		C_CSWeaponBase* pWeapon = reinterpret_cast<C_CSWeaponBase*>(pEntity);
		if (pWeapon->GetOriginalOwnerXuid() != steamID)
			continue;

		CAttributeManager* pAttributeContainer = pWeapon->m_AttributeManager();
		if (!pAttributeContainer)
			continue;

		C_EconItemView* pWeaponItemView = pAttributeContainer->m_Item();
		if (!pWeaponItemView)
			continue;
		CEconItemDefinition* pWeaponDefinition =
			pWeaponItemView->GetStaticData();
		if (!pWeaponDefinition)
			continue;

		CGameSceneNode* pWeaponSceneNode = pWeapon->GetGameSceneNode();
		if (!pWeaponSceneNode)
			continue;

		// No idea how to check this faster with the new loadout system.
		C_EconItemView* pWeaponInLoadoutItemView = nullptr;

		if (pWeaponDefinition->IsWeapon())
		{
			for (int i = 0; i <= 56; ++i)
			{
				C_EconItemView* pItemView = pInventory->GetItemInLoadout(
					pWeapon->m_iOriginalTeamNumber(), i);
				if (!pItemView)
					continue;

				if (pItemView->m_iItemDefinitionIndex() ==
					pWeaponDefinition->m_nDefIndex)
				{
					pWeaponInLoadoutItemView = pItemView;
					break;
				}
			}
		}
		else
		{
			pWeaponInLoadoutItemView = pInventory->GetItemInLoadout(
				pWeapon->m_iOriginalTeamNumber(),
				pWeaponDefinition->GetLoadoutSlot());
		}

		if (!pWeaponInLoadoutItemView)
			continue;

		// Check if skin is added by us.
		auto it = std::find(g_vecAddedItemsIDs.cbegin(), g_vecAddedItemsIDs.cend(), pWeaponInLoadoutItemView->m_iItemID());
		if (it == g_vecAddedItemsIDs.cend())
			continue;

		CEconItemDefinition* pWeaponInLoadoutDefinition = pWeaponInLoadoutItemView->GetStaticData();
		if (!pWeaponInLoadoutDefinition)
			continue;

		// Example: Will not equip FiveSeven skin on CZ. Not applies for knives.
		const bool isKnife = pWeaponInLoadoutDefinition->IsKnife(false);
		pWeaponItemView->m_bDisallowSOC() = false;
		pWeaponInLoadoutItemView->m_bDisallowSOC() = false;

		pWeaponItemView->m_iItemID() = pWeaponInLoadoutItemView->m_iItemID();
		pWeaponItemView->m_iItemIDHigh() = pWeaponInLoadoutItemView->m_iItemIDHigh();
		pWeaponItemView->m_iItemIDLow() = pWeaponInLoadoutItemView->m_iItemIDLow();
		pWeaponItemView->m_iAccountID() = uint32_t(pInventory->GetOwner().m_id);
		pWeaponItemView->m_iItemDefinitionIndex() = pWeaponInLoadoutDefinition->m_nDefIndex;

		// pWeaponItemView->m_bIsStoreItem() = true;
		//pWeaponItemView->m_bIsTradeItem() = true;

		// Displays nametag and stattrak on the gun.
		// Found by: https://www.unknowncheats.me/forum/members/2377851.html
		/*   if (!pWeapon->m_bUIWeapon()) {
			pWeapon->AddStattrakEntity();
			pWeapon->AddNametagEntity();
		}*/
		CBaseHandle hWeapon = pWeapon->GetRefEHandle();
		if (isKnife)
		{
			if (pViewModel && (pViewModel->m_hWeapon().GetEntryIndex() == hWeapon.GetEntryIndex() || pViewModel->m_hWeapon() == hWeapon))
			{
				if (pWeapon->datawep()->m_WeaponType() == 9 or pWeapon->datawep()->m_WeaponType() == 7)
					continue;
				pWeaponItemView->m_iItemDefinitionIndex() = pWeaponInLoadoutDefinition->m_nDefIndex;

				const char* knifeModel = pWeaponInLoadoutDefinition->GetModelName();

				std::uint32_t Hash = CUtlStringToken1(std::to_string(pWeaponInLoadoutDefinition->m_nDefIndex).c_str()).GetHashCode1();
				pWeapon->m_nSubclassID() = Hash;
				pWeapon->UpdateSubClass();
				pWeapon->datawep()->m_szName() = pWeaponInLoadoutDefinition->GetSimpleWeaponName();
				pWeapon->UpdateVData();

				CGameSceneNode* pViewModelSceneNode = pViewModel->GetGameSceneNode();
				if (pViewModelSceneNode)
				{
					pWeaponSceneNode->SetMeshGroupMask(2);
					pViewModelSceneNode->SetMeshGroupMask(2);
				}
				pWeapon->SetModel(knifeModel);
				pViewModel->SetModel(knifeModel);
				pViewModel->pAnimationGraphInstance->pAnimGraphNetworkedVariables = nullptr;
			}
		}
		else
		{
			// Use legacy weapon models only for skins that require them.
			// Probably need to cache this if you really care that much about
			// performance
			//const char* model = pWeaponInLoadoutDefinition->GetModelName();
			//pWeapon->SetModel(model);
			//if (pViewModel && pViewModel->m_hWeapon() == hWeapon)
			//{
			//	pViewModel->SetModel(model);
			//}

			CEconItemSystem* EconItemSystem = I::Client->GetEconItemSystem();
			CEconItemSchema* pItemSchema = EconItemSystem->GetEconItemSchema();

			auto paint_kit = pItemSchema->GetPaintKits().FindByKey(pWeaponInLoadoutItemView->GetCustomPaintKitIndex());
			bool uses_old_model = paint_kit.has_value() && paint_kit.value()->UsesOldModel();

			pWeaponSceneNode->SetMeshGroupMask(1 + uses_old_model);
			if (pViewModel && pViewModel->m_hWeapon().GetEntryIndex() == hWeapon.GetEntryIndex())
			{
				CGameSceneNode* pViewModelSceneNode = pViewModel->GetGameSceneNode();

				pViewModelSceneNode->SetMeshGroupMask(1 + uses_old_model);
			}
		}
	}
}
struct GloveInfo
{
	int itemId;
	uint64_t itemHighId;
	uint64_t itemLowId;
	int itemDefId;
};

static GloveInfo addedGloves;
void changers::OnEquipItemInLoadout(int team, int slot, uint64_t itemID)
{
	auto it =
		std::find(g_vecAddedItemsIDs.begin(), g_vecAddedItemsIDs.end(), itemID);
	if (it == g_vecAddedItemsIDs.end())
		return;

	CCSInventoryManager* pInventoryManager = GSF::pInventoryManager;
	if (!pInventoryManager)
		return;

	CCSPlayerInventory* pInventory = pInventoryManager->GetLocalInventory();
	if (!pInventory)
		return;

	C_EconItemView* pItemViewToEquip = pInventory->GetItemViewForItem(*it);
	if (!pItemViewToEquip)
		return;

	C_EconItemView* pItemInLoadout = pInventory->GetItemInLoadout(team, slot);
	if (!pItemInLoadout)
		return;

	CEconItemDefinition* pItemInLoadoutStaticData = pItemInLoadout->GetStaticData();
	if (!pItemInLoadoutStaticData)
		return;

	// Equip default item. If you would have bought Deagle and you previously
	// had R8 equipped it will now give you a Deagle.
	const uint64_t defaultItemID = (std::uint64_t(0xF) << 60) | pItemViewToEquip->m_iItemDefinitionIndex();
	pInventoryManager->EquipItemInLoadout(team, slot, defaultItemID);

	CEconItem* pItemInLoadoutSOCData = pItemInLoadout->GetSOCData(nullptr);
	if (!pItemInLoadoutSOCData)
		return;

	CEconItemDefinition* toequipdata = pItemViewToEquip->GetStaticData();
	if (!toequipdata)
		return;

	if (toequipdata->IsWeapon() && !toequipdata->IsKnife(false) && !toequipdata->IsGlove(false))
	{
		pInventory->SOUpdated(pInventory->GetOwner(), (CSharedObject*)pItemInLoadoutSOCData, eSOCacheEvent_Incremental);
		return;
	}
	else if (toequipdata->IsGlove(false))
	{
		uint64_t steamID = pInventory->GetOwner().m_id;

		CCSPlayerController* pLocalPlayerController = CCSPlayerController::GetLocalPlayerController();
		if (!pLocalPlayerController)
			return;

		C_CSPlayerPawn* pLocalPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pLocalPlayerController->GetPawnHandle());
		if (!pLocalPawn)
			return;

		if (pLocalPawn->GetHealth() <= 0)
			return;

		CCSPlayer_ViewModelServices* pViewModelServices = pLocalPawn->GetViewModelServices();
		if (!pViewModelServices)
			return;

		C_BaseViewModel* pViewModel = I::GameResourceService->pGameEntitySystem->Get<C_BaseViewModel>(pViewModelServices->m_hViewModel());
		if (!pViewModel)
			return;

		// Create a struct to store glove information
		addedGloves.itemId = pItemViewToEquip->m_iItemID();
		addedGloves.itemHighId = pItemViewToEquip->m_iItemIDHigh();
		addedGloves.itemLowId = pItemViewToEquip->m_iItemIDLow();
		addedGloves.itemDefId = pItemViewToEquip->m_iItemDefinitionIndex();

		C_EconItemView* pGloves = &pLocalPawn->m_EconGloves();
		if (!pGloves)
			return;

		CEconItemDefinition* pGlovesDefinition = pGloves->GetStaticData();
		if (!pGlovesDefinition)
			return;
		pLocalPawn->m_bNeedToReApplyGloves() = true;
		pInventory->SOUpdated(pInventory->GetOwner(), (CSharedObject*)pItemInLoadoutSOCData, eSOCacheEvent_Incremental);
		return;
	}
	else if (toequipdata->IsKnife(false))
	{
		pInventory->SOUpdated(pInventory->GetOwner(), (CSharedObject*)pItemInLoadoutSOCData, eSOCacheEvent_Incremental);
		return;
	}
}

void changers::OnSetModel(C_BaseModelEntity* pEntity, const char*& model)
{
	// When you're lagging you may see the default knife for one second and this
	// function fixes that.
	if (!I::Engine->IsConnected() || !I::Engine->IsInGame())
		return;

	CCSPlayerController* pLocalPlayerController = CCSPlayerController::GetLocalPlayerController();
	if (!pLocalPlayerController)
		return;

	C_CSPlayerPawn* pLocalPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pLocalPlayerController->GetPawnHandle());
	if (!pLocalPawn)
		return;

	if (pLocalPawn->GetHealth() <= 0)
		return;

	if (!pEntity)
		return;

	C_BaseViewModel* pViewModel = (C_BaseViewModel*)pEntity;
	if (!pViewModel)
		return;

	CCSPlayerInventory* pInventory = GSF::pInventoryManager->GetLocalInventory();
	if (!pInventory)
		return;

	const uint64_t steamID = pInventory->GetOwner().m_id;

	C_CSWeaponBase* pWeapon = I::GameResourceService->pGameEntitySystem->Get<C_CSWeaponBase>(pViewModel->m_hWeapon());
	if (!pWeapon || !pWeapon->IsBasePlayerWeapon())
		return;

	if (!pWeapon->IsWeapon() ||
		pWeapon->GetOriginalOwnerXuid() != steamID)
		return;

	if (pWeapon->datawep()->m_WeaponType() == WEAPONTYPE_KNIFE == WEAPONTYPE_C4 == WEAPONTYPE_GRENADE)
		return;

	CAttributeManager* pAttributeContainer = pWeapon->m_AttributeManager();
	if (!pAttributeContainer)
		return;

	C_EconItemView* pWeaponItemView = pAttributeContainer->m_Item();
	if (!pWeaponItemView)
		return;

	CEconItemDefinition* pWeaponDefinition = pWeaponItemView->GetStaticData();
	if (!pWeaponDefinition)
		return;

	C_EconItemView* pWeaponInLoadoutItemView = pInventory->GetItemInLoadout(
		pWeapon->m_iOriginalTeamNumber(), pWeaponDefinition->GetLoadoutSlot());
	if (!pWeaponInLoadoutItemView)
		return;

	// Check if skin is added by us.
	auto it = std::find(g_vecAddedItemsIDs.cbegin(), g_vecAddedItemsIDs.cend(),
		pWeaponInLoadoutItemView->m_iItemID());
	if (it == g_vecAddedItemsIDs.cend())
		return;
	if (pWeapon->datawep()->m_WeaponType() == 9 or pWeapon->datawep()->m_WeaponType() == 7)
		return;
	CEconItemDefinition* pWeaponInLoadoutDefinition =
		pWeaponInLoadoutItemView->GetStaticData();

	if (!pWeaponInLoadoutDefinition) // ||
		return;

	model = pWeaponInLoadoutDefinition->GetModelName();
}

void changers::AddEconItemToList(CEconItem* pItem)
{
	g_vecAddedItemsIDs.emplace_back(pItem->m_ulID);
}

void changers::Shutdown()
{
	CCSPlayerInventory* pInventory = GSF::pInventoryManager->GetLocalInventory();
	if (!pInventory)
		return;

	for (uint64_t id : g_vecAddedItemsIDs)
	{
		pInventory->RemoveEconItem(pInventory->GetSOCDataForItem(id));
	}
}

//void skin_changer::OnRoundReset(IGameEvent* pEvent)
//{
//	if (!pEvent || g_vecAddedItemsIDs.empty())
//		return;
//
//	const char* eventName = pEvent->GetName();
//	if (!eventName)
//		return;
//
//	CCSPlayerController* pLocalPlayerController = CCSPlayerController::GetLocalPlayerController();
//
//	if (!pLocalPlayerController)
//		return;
//
//	C_CSPlayerPawn* pLocalPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pLocalPlayerController->GetPawnHandle());
//	if (!pLocalPawn)
//		return;
//}

void changers::OnPreFireEvent(IGameEvent* pEvent)
{
	if (!pEvent)
		return;
	std::string eventName = pEvent->GetName();

	if (eventName.find("player_death") == std::string::npos)
		return;

	const auto pControllerWhoKilled = pEvent->get_player_controller("attacker");
	if (pControllerWhoKilled == nullptr)
		return;

	const auto pControllerWhoDied = pEvent->get_player_controller("userid");
	if (pControllerWhoDied == nullptr)
		return;

	CCSPlayerController* pLocalPlayerController = CCSPlayerController::GetLocalPlayerController();
	if (!pLocalPlayerController || pControllerWhoKilled->GetIdentity()->GetIndex() != pLocalPlayerController->GetIdentity()->GetIndex())
		return;

	C_CSPlayerPawn* pLocalPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pLocalPlayerController->GetPawnHandle());
	if (!pLocalPawn)
		return;

	CPlayer_WeaponServices* pWeaponServices = pLocalPawn->GetWeaponServices();
	if (!pWeaponServices)
		return;

	C_CSWeaponBase* pActiveWeapon = I::GameResourceService->pGameEntitySystem->Get<C_CSWeaponBase>(pWeaponServices->m_hActiveWeapon());
	if (!pActiveWeapon)
		return;

	CAttributeManager* pAttributeContainer = pActiveWeapon->m_AttributeManager();
	if (!pAttributeContainer)
		return;

	C_EconItemView* pWeaponItemView = pAttributeContainer->m_Item();
	if (!pWeaponItemView)
		return;

	CEconItemDefinition* pWeaponDefinition = pWeaponItemView->GetStaticData();

	if (!pWeaponDefinition || !pWeaponDefinition->IsKnife(false))
		return;

	pEvent->SetString(CUtlStringToken("weapon"), pWeaponDefinition->GetSimpleWeaponName());
}




CEconItem* CEconItem::CreateInstance()
{
	// ida: // #STR: "Update(CEconItem)", "CEconItem", "Create(CEconItem)", "BuildCacheSubscribed(CEconItem)", "Update(CEconEquipSlot)", "CEconEquipSlot", "Create(CEconEquipSlot)", "BuildCacheSubscribed(CEconEquipSlot)", "Update(CEconPersonaDataPublic)", "CEconPersonaDataPublic"
	// ida:  sub_E0F420(
	//	1,
	//	(unsigned int)CEconItem::CreateInstance,
	//	0,
	//	(unsigned int)"CEconItem",
	//	(__int64)"BuildCacheSubscribed(CEconItem)",
	//	(__int64)"Create(CEconItem)",
	//	(__int64)"Update(CEconItem)");

	using fnCreateSharedObjectSubclassEconItem = CEconItem * (__cdecl*)();
	static fnCreateSharedObjectSubclassEconItem oCreateSharedObjectSubclassEconItem = reinterpret_cast<fnCreateSharedObjectSubclassEconItem>(MEM::FindPattern(CLIENT_DLL, ("48 83 EC 28 B9 48 00 00 00 E8 ? ? ? ? 48 85")));

#ifdef CS_PARANOID
	CS_ASSERT(oCreateSharedObjectSubclassEconItem != nullptr);
#endif

	return oCreateSharedObjectSubclassEconItem();
}

inline void* (CS_FASTCALL* fnSetDynamicAttributeValueUint)(void*, void*,
	void*);
void CEconItem::SetDynamicAttributeValue(int index, void* value)
{
	fnSetDynamicAttributeValueUint = reinterpret_cast<decltype(fnSetDynamicAttributeValueUint)>(FindPattern(CLIENT_DLL, CXOR("48 89 6C 24 20 57 41 56 41 57 48 81 EC 90 00 00 00 48 8B FA C7 44 24 20 00 00 00 00 4D 8B F8 4C 8D 0D ?? ?? ?? ?? 48 8B E9 4C 8D 05 ?? ?? ?? ?? 33 D2 48 8B 4F 18 E8 ?? ?? ?? ?? 4C 8B F0 48 85 C0 0F 84 9A")));

	CEconItemSchema* pItemSchema =
		I::Client->GetEconItemSystem()->GetEconItemSchema();
	if (!pItemSchema)
		return;

	void* pAttributeDefinitionInterface =
		pItemSchema->GetAttributeDefinitionInterface(index);
	if (!pAttributeDefinitionInterface)
		return;

	if (!fnSetDynamicAttributeValueUint)
		return;

	fnSetDynamicAttributeValueUint(this, pAttributeDefinitionInterface, value);
}

void CEconItem::SetDynamicAttributeValueString(int index, const char* value)
{
	// CS2FIXME: Function got inlined and cannot be sigscanned.
}



void changers::AgentChanger(C_CSPlayerPawn* pPlayerPawn) {
	CCSPlayerInventory* pInventory = GSF::pInventoryManager->GetLocalInventory();
	if (!pInventory)
		return;

	if (!pPlayerPawn)
		return;

	CEconItemDefinition* item_view_loadout = nullptr;
	for (int i = 0; i <= 56; ++i)
	{
		C_EconItemView* item_view_ = pInventory->GetItemInLoadout(pPlayerPawn->GetTeam(), i);
		if (!item_view_)
			continue;

		auto p = item_view_->GetStaticData();

		if (p->is_custom_player())
		{
			item_view_loadout = p;
			break;
		}
	}

	if (!item_view_loadout)
		return;

	if (FNV1A::Hash(item_view_loadout->GetModelName()) == changers::hash_agent)
		return;

	changers::hash_agent = FNV1A::Hash(item_view_loadout->GetModelName());

	pPlayerPawn->SetModel(item_view_loadout->GetModelName());
}

static int glove_frame = 0;

// Define a struct to hold glove information

struct material_info
{
	changers::material_record* p_mat_records;
	uint32_t ui32_count;
};

void invalidate_glove_material(C_BaseViewModel* viewmodel)
{
	using function_t = void(__fastcall*)(void*, uintptr_t);
	static function_t fn = reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, CXOR("89 54 24 10 48 83 EC 28 48 81 C1 ? ? ? ? 85 D2 75 09 48 83 C4 28 E9")));
	CS_ASSERT(fn != nullptr);
	return fn(viewmodel, changers::material_magic_number::material_magic_number__gloves);
}

void changers::GloveChanger(C_CSPlayerPawn* pPlayerPawn) {
	CCSPlayerInventory* pInventory = GSF::pInventoryManager->GetLocalInventory();
	if (!pInventory)
		return;


	auto steamID = pInventory->GetOwner().m_id;
	C_CSGOViewModel* pViewModel = (C_CSGOViewModel*)I::GameResourceService->pGameEntitySystem->Get(pPlayerPawn->GetViewModelServices()->m_hViewModel().GetEntryIndex());

	C_EconItemView* pGloveItemView = &pPlayerPawn->m_EconGloves();
	if (!pGloveItemView)
		return;

	auto pItemViewLoadout = pInventory->GetItemInLoadout(pPlayerPawn->GetTeam(), 41);
	if (!pItemViewLoadout)
		return;

	CEconItemDefinition* pGloveDefinition = pGloveItemView->GetStaticData();
	if (!pGloveDefinition)
		return;

	static std::uint8_t uUpdateFrames = 0U;
	/* Detect if we have to change gloves */
	if (pGloveItemView->m_iItemID() != pItemViewLoadout->m_iItemID() || force_update)
	{
		uUpdateFrames = 3U;
		pGloveItemView->m_iItemDefinitionIndex() = pItemViewLoadout->m_iItemDefinitionIndex();
		pGloveItemView->m_iItemID() = pItemViewLoadout->m_iItemID();
		pGloveItemView->m_iItemIDHigh() = pItemViewLoadout->m_iItemIDHigh();
		pGloveItemView->m_iItemIDLow() = pItemViewLoadout->m_iItemIDLow();
		pGloveItemView->m_iAccountID() = pItemViewLoadout->m_iAccountID();
		pGloveItemView->m_bDisallowSOC() = false;
		force_update = false;
	}
	/* Detect if we have to change pGloveItemView */
	if (uUpdateFrames)
	{
		invalidate_glove_material(pViewModel);

		pGloveItemView->m_bInitialized() = true;
		pPlayerPawn->m_bNeedToReApplyGloves() = true;

		uUpdateFrames--;
	}
}




void changers::KnifeChanger(C_CSPlayerPawn* pPlayerPawn) {
	CCSPlayerInventory* pInventory = GSF::pInventoryManager->GetLocalInventory();
	if (!pInventory)
		return;

	auto steam_id = pInventory->GetOwner().m_id;

	auto pViewModelServices = pPlayerPawn->GetViewModelServices();
	if (!pViewModelServices)
		return;

	C_CSGOViewModel* pViewModel = reinterpret_cast<C_CSGOViewModel*>(I::GameResourceService->pGameEntitySystem->Get(pViewModelServices->m_hViewModel().GetEntryIndex()));

	int highestIndex = I::GameResourceService->pGameEntitySystem->GetHighestEntityIndex();

	for (int i = 64 + 1; i <= highestIndex; ++i)
	{
		C_BaseEntity* pEntity = I::GameResourceService->pGameEntitySystem->Get<C_BaseEntity>(i);
		if (!pEntity || !pEntity->IsWeapon())
			continue;

		C_CSWeaponBase* pWeapon = reinterpret_cast<C_CSWeaponBase*>(pEntity);

		if (pWeapon->datawep()->m_WeaponType() == WEAPONTYPE_KNIFE == WEAPONTYPE_C4 == WEAPONTYPE_GRENADE)
			return;
		if (pWeapon->GetOriginalOwnerXuid() != steam_id)
			continue;

		if (pWeapon->datawep() && pWeapon->datawep()->m_WeaponType() != WEAPONTYPE_KNIFE)
		{
			continue;
		}

		CAttributeManager* pAttributeContainer = pWeapon->m_AttributeManager();
		if (!pAttributeContainer)
			continue;

		C_EconItemView* pWeaponItemView = pAttributeContainer->m_Item();
		if (!pWeaponItemView)
			continue;

		CEconItemDefinition* pWeaponDefinition = pWeaponItemView->GetStaticData();
		if (!pWeaponDefinition)
			continue;

		CGameSceneNode* pWeaponSceneNode = pWeapon->GetGameSceneNode();
		if (!pWeaponSceneNode)
			continue;

		C_EconItemView* pWeaponInLoadoutItemView = nullptr;

		if (pWeaponDefinition->GetStickersSupportedCount() >= 4)
		{
			for (int i = 0; i <= 56; ++i)
			{
				C_EconItemView* pItemView = pInventory->GetItemInLoadout(
					pWeapon->m_iOriginalTeamNumber(), i);
				if (!pItemView)
					continue;

				if (pItemView->m_iItemDefinitionIndex() == pWeaponDefinition->m_nDefIndex)
				{
					pWeaponInLoadoutItemView = pItemView;
					break;
				}
			}
		}
		else
		{
			pWeaponInLoadoutItemView = pInventory->GetItemInLoadout(
				pWeapon->m_iOriginalTeamNumber(),
				pWeaponDefinition->GetLoadoutSlot());
		}
		if (!pWeaponInLoadoutItemView)
			continue;

		auto it = std::find(g_vecAddedItemsIDs.cbegin(), g_vecAddedItemsIDs.cend(), pWeaponInLoadoutItemView->m_iItemID());
		if (it == g_vecAddedItemsIDs.cend())
			continue;

		CEconItemDefinition* pWeaponInLoadoutDefinition = pWeaponInLoadoutItemView->GetStaticData();
		if (!pWeaponInLoadoutDefinition)
			continue;

		if (!pWeaponInLoadoutDefinition->IsKnife(false) && pWeaponInLoadoutDefinition->m_nDefIndex != pWeaponDefinition->m_nDefIndex)
			continue;

		pWeaponItemView->m_bDisallowSOC() = false;
		pWeaponItemView->m_iItemID() = pWeaponInLoadoutItemView->m_iItemID();
		pWeaponItemView->m_iItemIDHigh() = pWeaponInLoadoutItemView->m_iItemIDHigh();
		pWeaponItemView->m_iItemIDLow() = pWeaponInLoadoutItemView->m_iItemIDLow();
		pWeaponItemView->m_iAccountID() = uint32_t(steam_id);
		pWeaponItemView->m_iItemDefinitionIndex() = pWeaponInLoadoutDefinition->m_nDefIndex;
		// *(bool*)(std::uintptr_t(pWeaponItemView) + 0x1e9) = false;

		std::uint32_t hash_ = CUtlStringToken1(std::to_string(pWeaponInLoadoutDefinition->m_nDefIndex).c_str()).GetHashCode1();
		pWeapon->m_nSubclassID() = hash_;
		pWeapon->UpdateSubClass();

		pWeapon->datawep()->m_szName() = pWeaponInLoadoutDefinition->GetSimpleWeaponName();
		pWeapon->UpdateVData();

		const char* knifeModel = pWeaponInLoadoutDefinition->GetModelName();
		pWeapon->SetModel(knifeModel);
		CBaseHandle hWeapon = pWeapon->GetRefEHandle();

		if (pViewModel && (pViewModel->m_hWeapon().GetEntryIndex() == hWeapon.GetEntryIndex()))
		{
			pViewModel->SetModel(knifeModel);
			pViewModel->pAnimationGraphInstance->pAnimGraphNetworkedVariables = nullptr;
		}
	}
}


bool CEconItemDefinition::is_custom_player()
{
	auto CSGO_Type_Agents = FNV1A::Hash("#Type_CustomPlayer");

	if (FNV1A::Hash(this->m_pszItemBaseName) != CSGO_Type_Agents)
		return false;

	if (m_nDefIndex >= 5036 && m_nDefIndex < 5106)
		return false;

	if (m_nDefIndex >= 5200 && m_nDefIndex <= 5205)
		return false;

	if (m_nDefIndex >= 5300 && m_nDefIndex <= 5305)
		return false;

	if (m_nDefIndex == 5600)
		return false;

	return true;
}

bool CEconItemDefinition::IsWeapon()
{
	// Every gun supports at least 4 stickers.
	return GetStickersSupportedCount() >= 4;
}

bool CEconItemDefinition::IsKnife(bool excludeDefault)
{
	auto CSGO_Type_Knife = FNV1A::Hash("#CSGO_Type_Knife");

	if (FNV1A::Hash(this->m_pszItemBaseName) != CSGO_Type_Knife)
		return false;

	return excludeDefault ? m_nDefIndex >= 500 : true;
}

bool CEconItemDefinition::IsGlove(bool excludeDefault)
{
	auto Type_Hands = FNV1A::Hash("#Type_Hands");

	bool valid = FNV1A::Hash(this->m_pszItemBaseName) == Type_Hands;
	bool defaultGlove = valid && m_nDefIndex == 5028 || m_nDefIndex == 5029;

	return excludeDefault ? !defaultGlove : valid;
}

bool CEconItemDefinition::IsAgent()
{
	auto Type_CustomPlayer = FNV1A::Hash("#Type_CustomPlayer");

	bool valid = FNV1A::Hash(this->m_pszItemBaseName) == Type_CustomPlayer;

	if (valid || m_nDefIndex >= 5036 && m_nDefIndex < 5106)
		return false;

	if (valid || m_nDefIndex >= 5200 && m_nDefIndex <= 5205)
		return false;

	if (valid || m_nDefIndex >= 5300 && m_nDefIndex <= 5305)
		return false;

	if (valid || m_nDefIndex == 5600)
		return false;

	return true;
}

static CGCClientSharedObjectTypeCache* CreateBaseTypeCache(
	CCSPlayerInventory* pInventory)
{
	CGCClient* pGCClient = GSF::pGCClientSystem->GetCGCClient();
	if (!pGCClient)
		return nullptr;

	SOID_t id = pInventory->GetOwner();

	CGCClientSharedObjectCache* pSOCache = pGCClient->FindSOCache(id);
	if (!pSOCache)
		return nullptr;

	return pSOCache->CreateBaseTypeCache(k_EEconTypeItem);
}

bool CCSPlayerInventory::AddEconItem(CEconItem* pItem)
{
	// Helper function to aid in adding items.
	if (!pItem)
		return false;

	CGCClientSharedObjectTypeCache* pSOTypeCache = ::CreateBaseTypeCache(this);
	if (!pSOTypeCache || !pSOTypeCache->AddObject((CSharedObject*)pItem))
		return false;

	SOCreated(GetOwner(), (CSharedObject*)pItem, eSOCacheEvent_Incremental);
	return true;
}

void CCSPlayerInventory::RemoveEconItem(CEconItem* pItem)
{
	// Helper function to aid in removing items.
	if (!pItem)
		return;

	CGCClientSharedObjectTypeCache* pSOTypeCache = ::CreateBaseTypeCache(this);
	if (!pSOTypeCache)
		return;

	const CUtlVector<CEconItem*>& pSharedObjects =
		pSOTypeCache->GetVecObjects<CEconItem*>();
	if (!pSharedObjects.Find(pItem))
		return;

	SODestroyed(GetOwner(), (CSharedObject*)pItem, eSOCacheEvent_Incremental);
	pSOTypeCache->RemoveObject((CSharedObject*)pItem);

	pItem->Destruct();
}

std::pair<uint64_t, uint32_t> CCSPlayerInventory::GetHighestIDs()
{
	uint64_t maxItemID = 0;
	uint32_t maxInventoryID = 0;

	CGCClientSharedObjectTypeCache* pSOTypeCache = ::CreateBaseTypeCache(this);
	if (pSOTypeCache)
	{
		const CUtlVector<CEconItem*>& vecItems =
			pSOTypeCache->GetVecObjects<CEconItem*>();

		for (CEconItem* it : vecItems)
		{
			if (!it)
				continue;

			// Checks if item is default.
			if ((it->m_ulID & 0xF000000000000000) != 0)
				continue;

			maxItemID = std::max(maxItemID, it->m_ulID);
			maxInventoryID = std::max(maxInventoryID, it->m_unInventory);
		}
	}

	return std::make_pair(maxItemID, maxInventoryID);
}

C_EconItemView* CCSPlayerInventory::GetItemViewForItem(uint64_t itemID)
{
	C_EconItemView* pEconItemView = nullptr;

	const CUtlVector<C_EconItemView*>& pItems = GetItemVector();
	for (C_EconItemView* it : pItems)
	{
		if (it && it->m_iItemID() == itemID)
		{
			pEconItemView = it;
			break;
		}
	}

	return pEconItemView;
}

CEconItem* CCSPlayerInventory::GetSOCDataForItem(uint64_t itemID)
{
	CEconItem* pSOCData = nullptr;

	CGCClientSharedObjectTypeCache* pSOTypeCache = ::CreateBaseTypeCache(this);
	if (pSOTypeCache)
	{
		const CUtlVector<CEconItem*>& vecItems =
			pSOTypeCache->GetVecObjects<CEconItem*>();

		for (CEconItem* it : vecItems)
		{
			if (it && (int)it->m_ulID == (int)itemID)
			{
				pSOCData = it;
				return pSOCData;

				break;
			}
		}
	}

	return pSOCData;
}
