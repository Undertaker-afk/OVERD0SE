#pragma once
#include <d3d11.h>
#include <shlobj_core.h>

#include "../utilities/common.h"
#include "../utilities/memory.h"
#include "Offsets.hpp"

#pragma region sdk_definitons
#define GAME_RESOURCE_SERVICE_CLIENT CXOR("GameResourceServiceClientV00")
#define SOURCE2_CLIENT CXOR("Source2Client00")
#define SCHEMA_SYSTEM CXOR("SchemaSystem_00")
#define INPUT_SYSTEM_VERSION CXOR("InputSystemVersion00")
#define SOURCE2_ENGINE_TO_CLIENT CXOR("Source2EngineToClient00")
#define ENGINE_CVAR CXOR("VEngineCvar00")
#define LOCALIZE CXOR("Localize_00")
#define NETWORK_CLIENT_SERVICE CXOR("NetworkClientService_001")
#define MATERIAL_SYSTEM2 CXOR("VMaterialSystem2_00")
#define RENDERSYS_SYSTEM CXOR("RenderGameSystem_00")
#define RESOURCE_SYSTEM CXOR("ResourceSystem013")
#define PARTICLE_SYSTEM CXOR("ParticleSystemMgr003")
#include "../sdk/interfaces/game_rule.h"
#define RESOURCE_HANDLE_UTILS CXOR("ResourceHandleUtils001")

#define TICK_INTERVAL (0.015625f)
#define TIME_TO_TICKS(TIME) (static_cast<int>(0.5f + static_cast<float>(TIME) / TICK_INTERVAL))
#define TICKS_TO_TIME(TICKS) (TICK_INTERVAL * static_cast<float>(TICKS))
#define ROUND_TO_TICKS(TIME) (TICK_INTERVAL * TIME_TO_TICKS(TIME))
#define TICK_NEVER_THINK (-1)
#pragma endregion

class ISwapChainDx11;
class IMemAlloc;
class CCSGOInput;
class ISchemaSystem;
class IGlobalVars;
class IInputSystem;
class IGameResourceService;
class ISource2Client;
class IEngineClient;
class IEngineCVar;
class ccvar;
class INetworkClientService;
class material_system_t;
class CLocalize;
class IResourceSystem;
class CResourceHandleUtils;
class i_trace;
class CNetworkGameClient;
class IGameEvent;
class IGameEventManager2;
class CGCClientSystem;
class CGameParticleManagerSystem;
class CParticleManager;

class CCSInventoryManager;
class CCSGCClientSystem;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

class CLocalize {
public:
	auto FindSafe(const char* tokenName) {
		return MEM::CallVFunc<const char*, 17U>(this, tokenName);
	}
};

namespace I
{
	bool Setup();
	bool SetupBaseSystems();

	bool CreateRenderTarget(IDXGISwapChain* pSwapChain);
	void DestroyRenderTarget();

	CCSGOInput* Get_input();

	inline IGameEventManager2* GameEvent = nullptr;
	inline i_trace* Trace = nullptr;
	inline IMemAlloc* MemAlloc = nullptr;
	inline ISwapChainDx11* SwapChain = nullptr;
	inline ID3D11Device* Device = nullptr;
	inline ID3D11DeviceContext* DeviceContext = nullptr;
	inline ID3D11RenderTargetView* RenderTargetView = nullptr;
	inline CCSGOInput* Input = nullptr;
	inline ISchemaSystem* SchemaSystem = nullptr;
	inline IGlobalVars* GlobalVars = nullptr;
	inline IInputSystem* InputSystem = nullptr;
	inline IGameResourceService* GameResourceService = nullptr;
	inline ISource2Client* Client = nullptr;
	inline IEngineClient* Engine = nullptr;
	inline IEngineCVar* Cvar = nullptr;
	inline ccvar* convars = nullptr;
	inline CNetworkGameClient* m_network_game_client = nullptr;
	inline CGameParticleManagerSystem* MGameParticleManagerSystem = nullptr;
	inline CParticleManager* ParticleManager = nullptr;
	inline INetworkClientService* NetworkClientService = nullptr;
	inline material_system_t* MaterialSystem2 = nullptr;
	inline CLocalize* Localize = nullptr;
	inline CResourceHandleUtils* ResourceHandleUtils = nullptr;
	inline IResourceSystem* ResourceSystem = nullptr;
	inline float(CS_CDECL* m_random_float)(float min, float max) = nullptr;
	inline int(CS_CDECL* m_random_seed)(int seed) = nullptr;
}

namespace GSF {
	inline CCSInventoryManager* pInventoryManager = nullptr;
	inline CCSGCClientSystem* pGCClientSystem = nullptr;
}