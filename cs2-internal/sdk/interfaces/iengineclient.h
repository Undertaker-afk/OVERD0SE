#pragma once

// used: callvfunc
#include "../../utilities/memory.h"

inline constexpr uint64_t Helper_GetAlternateIconKeyForWeaponPaintWearItem(
	uint16_t nDefIdx, uint32_t nPaintId, uint32_t nWear) {
	return (nDefIdx << 16) + (nPaintId << 2) + nWear;
}

struct AlternateIconData_t {
	const char* sSimpleName;
	const char* sLargeSimpleName;

private:
	char pad0[0x8];  // no idea
	char pad1[0x8];  // no idea

};
class CPaintKit {
public:
	char pad_0x0000[0xE0]; //0x0000

	int64_t PaintKitId() {
		return *reinterpret_cast<int64_t*>((uintptr_t)(this));
	}

	const char* PaintKitName() {
		return *reinterpret_cast<const char**>((uintptr_t)(this) + 0x8);
	}

	const char* PaintKitDescriptionString() {
		return *reinterpret_cast<const char**>((uintptr_t)(this) + 0x10);
	}

	const char* PaintKitDescriptionTag() {
		return *reinterpret_cast<const char**>((uintptr_t)(this) + 0x18);
	}

	int32_t PaintKitRarity() {
		return *reinterpret_cast<int32_t*>((uintptr_t)(this) + 0x44);
	}

	bool UsesOldModel() {
		return *reinterpret_cast<bool*>((uintptr_t)(this) + 0xB2);
	}
};

#include "../datatypes/utlmap.h"
class CEconItemDefinition;
class CEconItemSchema {
public:
	auto GetAttributeDefinitionInterface(int iAttribIndex)
	{
		return MEM::CallVFunc<void*, 4U>(this, iAttribIndex);
	}

	auto& GetSortedItemDefinitionMap()
	{
		return *reinterpret_cast<CUtlMap2<int, CEconItemDefinition*>*>(
			(uintptr_t)(this) + 0x128);
	}

	auto& GetAlternateIconsMap()
	{
		return *reinterpret_cast<CUtlMap2<uint64_t, AlternateIconData_t>*>(
			(uintptr_t)(this) + 0x278);
	}

	auto& GetPaintKits()
	{
		return *reinterpret_cast<CUtlMap2<int, CPaintKit*>*>((uintptr_t)(this) +
			0x2F0);
	}
};

class CEconItemSystem {
public:
	auto GetEconItemSchema() {
		return *reinterpret_cast<CEconItemSchema**>((uintptr_t)(this) + 0x8);
	}
};

enum flow : int {
	FLOW_OUTGOING = 0,
	FLOW_INCOMING = 1,
};

class INetChannelInfo {
public:
	float get_latency(flow flow)
	{
		return MEM::CallVFunc<int, 10U>(this, flow);
	}
};

#include "SceneDebugOverlay.hpp"

class ISource2Client {
public:
	auto GetEconItemSystem() {
		static auto fnGetEconItemSystem = *reinterpret_cast<CEconItemSystem**>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, CXOR("48 8B 05 ?? ?? ?? ?? 48 85 C0 75 6B"))));

		return fnGetEconItemSystem;
	}
	float GetLerpTime() {
		return MEM::CallVFunc<float, 173U>(this);
	}
	c_scene_debug_overlay* get_scene_debug_overlay()
	{
		//return MEM::CallVFunc<c_scene_debug_overlay, 157U>(this);
		using function_t = c_scene_debug_overlay * (CS_THISCALL*)(ISource2Client*);
		return (*reinterpret_cast<function_t**>(std::uintptr_t(this)))[163](this);
	}
};

class c_networked_client_info {
	std::byte pad_001[0x4];
public:
	int m_render_tick;
	float m_render_tick_fraction;
	int m_player_tick_count;
	float m_player_tick_fraction;
private:
	std::byte pad_002[0x4];
public:
	struct {
	private:
		std::byte pad_022[0xC];
	public:
		Vector_t m_eye_pos;
	} *m_local_data;
private:
	std::byte pad_003[0x8];
};

class IEngineClient
{
public:
	int GetMaxClients()
	{
		return MEM::CallVFunc<int, 34U>(this);
	}

	bool IsInGame()
	{
		return MEM::CallVFunc<bool, 35U>(this);
	}

	bool IsConnected()
	{
		return MEM::CallVFunc<bool, 36U> (this);
	}
	//gusta
	INetChannelInfo* GetNetChannelInfo(int split_screen_slot) {
		return MEM::CallVFunc<INetChannelInfo*, 37U>(this, split_screen_slot);
	}
	// return CBaseHandle index
	int GetLocalPlayer()
	{
		int nIndex = -1;

		MEM::CallVFunc<void, 49U>(this, std::ref(nIndex), 0);

		return nIndex + 1;
	}

	void unrestricted_cmd(const char* szCmd) {
		return MEM::CallVFunc<void, 45>(this, 0, szCmd, 0x7FFEF001);
	}

	[[nodiscard]] const char* GetLevelName()
	{
		return MEM::CallVFunc<const char*, 56U>(this);
	}
	 
	[[nodiscard]] const char* GetLevelNameShort()
	{
		return MEM::CallVFunc<const char*, 57U>(this);
	}

	[[nodiscard]] const char* GetProductVersionString()
	{
		return MEM::CallVFunc<const char*, 82U>(this);
	}

	c_networked_client_info* get_networked_client_info() {
		c_networked_client_info client_info;
		MEM::CallVFunc<void*, 178>(this, &client_info);
		return &client_info;
	}
};

enum EClientFrameStage : int
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	// a network packet is being received
	FRAME_NET_UPDATE_START,
	// data has been received and we are going to start calling postdataupdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// data has been received and called postdataupdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// received all packets, we can now do interpolation, prediction, etc
	FRAME_NET_UPDATE_END,
	// start rendering the scene
	FRAME_RENDER_START,
	// finished rendering the scene
	FRAME_RENDER_END,
	FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE
};