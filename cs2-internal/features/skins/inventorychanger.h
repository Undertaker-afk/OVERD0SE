#pragma once

#include <cstdint>
#include <vector>

class C_CSPlayerPawn;
class IGameEvent;
class C_BaseModelEntity;
class CEconItem;



static std::vector<uint64_t> g_vecAddedItemsIDs;

inline bool force_update = false;

namespace changers {
	void AgentChanger(C_CSPlayerPawn* pPlayerPawn);
	void GloveChanger(C_CSPlayerPawn* pPlayerPawn);
	void KnifeChanger(C_CSPlayerPawn* pPlayerPawn);
	

	void OnFrameStageNotify(int nStage);
	void OnEquipItemInLoadout(int nTeam, int nSlot, std::uint64_t nItemID);
	void OnSetModel(C_BaseModelEntity* pEntity, const char*& szModel);
	void OnPreFireEvent(IGameEvent* pEvent);


	void AddEconItemToList(CEconItem* pItem);
	void Shutdown();





	enum material_magic_number : uint32_t
	{
		material_magic_number__gloves = 0xf143b82a,
		material_magic_number__unknown1 = 0x1b52829c,
		material_magic_number__unknown2 = 0xa6ebe9b9,
		material_magic_number__unknown3 = 0x423b2ed4,
		material_magic_number__unknown4 = 0xc8d7255e
	};

	struct material_record
	{
		uint32_t ui32_unknwonstatic;
		material_magic_number identifer;
		uint32_t ui32_handle;
		uint32_t ui32_type_index;
	};
	inline bool dumpitems;
	inline std::uint64_t hash_agent;
}