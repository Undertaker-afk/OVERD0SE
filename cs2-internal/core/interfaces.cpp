#include <d3d11.h>
#include "interfaces.h"

#include "../utilities/memory.h"

#include "../sdk/interfaces/iswapchaindx11.h"
#include "hooks.h"

#include "../sdk/datatypes/resourceutils.h"
#include "../sdk/interfaces/events.h"
#include "../sdk/interfaces/imaterialsystem.h"
#include "../sdk/interfaces/inetworkclientservice.h"

using InstantiateInterfaceFn_t = void* (*)();

class CInterfaceRegister
{
public:
	InstantiateInterfaceFn_t fnCreate;
	const char* szName;
	CInterfaceRegister* pNext;
};

static const CInterfaceRegister* GetRegisterList(const wchar_t* wszModuleName)
{
	void* hModule = MEM::GetModuleBaseHandle(wszModuleName);
	if (hModule == nullptr)
		return nullptr;

	std::uint8_t* pCreateInterface = reinterpret_cast<std::uint8_t*>(MEM::GetExportAddress(hModule, CXOR("CreateInterface")));

	if (pCreateInterface == nullptr)
	{
		printf(CXOR("failed to get CreateInterface address\n"));
		return nullptr;
	}

	return *reinterpret_cast<CInterfaceRegister**>(MEM::ResolveRelativeAddress(pCreateInterface, 0x3, 0x7));
}

template <typename T = void*>
T* Capture(const CInterfaceRegister* pModuleRegister, const char* szInterfaceName)
{
	for (const CInterfaceRegister* pRegister = pModuleRegister; pRegister != nullptr; pRegister = pRegister->pNext)
	{
		if (const std::size_t nInterfaceNameLength = CRT::StringLength(szInterfaceName);
			// found needed interface
			CRT::StringCompareN(szInterfaceName, pRegister->szName, nInterfaceNameLength) == 0 &&
			// and we've given full name with hardcoded digits
			(CRT::StringLength(pRegister->szName) == nInterfaceNameLength ||
				// or it contains digits after name
				CRT::StringToInteger<int>(pRegister->szName + nInterfaceNameLength, nullptr, 10) > 0))
		{
			// capture our interface
			void* pInterface = pRegister->fnCreate();

			printf(CXOR("captured %s inteface\n"), pRegister->szName);

			return static_cast<T*>(pInterface);
		}
	}

	printf(CXOR("failed to find interface %s\n"), szInterfaceName);
	return nullptr;
}

CCSGOInput* I::Get_input()
{
	static auto get_func = *reinterpret_cast<CCSGOInput * (*)()>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, CXOR("48 8B 0D ? ? ? ? 4C 8B C6 8B 10 E8")), 0x3, 0x7));

	return get_func();
}

bool I::Setup()
{
	bool bSuccess = true;

	const auto pTier0Handle = MEM::GetModuleBaseHandle(TIER0_DLL);
	if (pTier0Handle == nullptr)
		return false;

	MemAlloc = *reinterpret_cast<IMemAlloc**>(MEM::GetExportAddress(pTier0Handle, CXOR("g_pMemAlloc")));
	bSuccess &= (MemAlloc != nullptr);

	m_random_float = reinterpret_cast<decltype(m_random_float)>(MEM::GetExportAddress(pTier0Handle, CXOR("RandomFloat")));

	m_random_seed = reinterpret_cast<decltype(m_random_seed)>(MEM::GetExportAddress(pTier0Handle, CXOR("RandomSeed")));

	const auto pSchemaSystemRegisterList = GetRegisterList(SCHEMASYSTEM_DLL);
	if (pSchemaSystemRegisterList == nullptr)
		return false;

	SchemaSystem = Capture<ISchemaSystem>(pSchemaSystemRegisterList, SCHEMA_SYSTEM);
	bSuccess &= (SchemaSystem != nullptr);

	const auto pInputSystemRegisterList = GetRegisterList(INPUTSYSTEM_DLL);
	if (pInputSystemRegisterList == nullptr)
		return false;

	InputSystem = Capture<IInputSystem>(pInputSystemRegisterList, INPUT_SYSTEM_VERSION);
	bSuccess &= (InputSystem != nullptr);

	const auto pEngineRegisterList = GetRegisterList(ENGINE2_DLL);
	if (pEngineRegisterList == nullptr)
		return false;

	GameResourceService = Capture<IGameResourceService>(pEngineRegisterList, GAME_RESOURCE_SERVICE_CLIENT);
	bSuccess &= (GameResourceService != nullptr);

	Engine = Capture<IEngineClient>(pEngineRegisterList, SOURCE2_ENGINE_TO_CLIENT);
	bSuccess &= (Engine != nullptr);

	NetworkClientService = Capture<INetworkClientService>(pEngineRegisterList, NETWORK_CLIENT_SERVICE);
	bSuccess &= (NetworkClientService != nullptr);

	m_network_game_client = NetworkClientService->GetNetworkClient();
	bSuccess &= (m_network_game_client != nullptr);

	const auto pTier0RegisterList = GetRegisterList(TIER0_DLL);
	if (pTier0RegisterList == nullptr)
		return false;

	Cvar = Capture<IEngineCVar>(pTier0RegisterList, ENGINE_CVAR);
	bSuccess &= (Cvar != nullptr);

	convars = Capture<ccvar>(pTier0RegisterList, ENGINE_CVAR);
	bSuccess &= (convars != nullptr);

	const auto pClientRegister = GetRegisterList(CLIENT_DLL);
	if (pClientRegister == nullptr)
		return false;

	Client = Capture<ISource2Client>(pClientRegister, SOURCE2_CLIENT);
	bSuccess &= (Client != nullptr);

	const auto pLocalizeRegisterList = GetRegisterList(LOCALIZE_DLL);
	if (pLocalizeRegisterList == nullptr)
		return false;

	Localize = Capture<CLocalize>(pLocalizeRegisterList, LOCALIZE);
	bSuccess &= (Localize != nullptr);

	/* material sys */
	const auto pMaterialSystem2Register = GetRegisterList(MATERIAL_SYSTEM2_DLL);
	if (pMaterialSystem2Register == nullptr)
		return false;

	MaterialSystem2 = Capture<material_system_t>(pMaterialSystem2Register, MATERIAL_SYSTEM2);
	bSuccess &= (MaterialSystem2 != nullptr);

	const auto pResourceSystemRegisterList = GetRegisterList(RESOURCESYSTEM_DLL);
	if (pResourceSystemRegisterList == nullptr)
		return false;

	ResourceSystem = Capture<IResourceSystem>(pResourceSystemRegisterList, RESOURCE_SYSTEM);
	bSuccess &= (ResourceSystem != nullptr);

	if (ResourceSystem != nullptr)
	{
		ResourceHandleUtils = reinterpret_cast<CResourceHandleUtils*>(ResourceSystem->QueryInterface(RESOURCE_HANDLE_UTILS));
		bSuccess &= (ResourceHandleUtils != nullptr);
	}

	const auto particles = GetRegisterList(PARTICLES_DLL);
	if (particles == nullptr)
		return false;

	ParticleManager = Capture<CParticleManager>(particles, PARTICLE_SYSTEM);
	bSuccess &= (ParticleManager != nullptr);

	Trace = *reinterpret_cast<i_trace**>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, CXOR("4C 8B 35 ? ? ? ? 24")), 0x3));
	bSuccess &= (Trace != nullptr);

	GameEvent = *reinterpret_cast<IGameEventManager2**>(MEM::ResolveRelativeAddress(MEM::GetVFunc<std::uint8_t*>(Client, 14U) + 0x3E, 0x3, 0x7));
	bSuccess &= (GameEvent != nullptr);

	SwapChain = **reinterpret_cast<ISwapChainDx11***>(MEM::ResolveRelativeAddress(MEM::FindPattern(RENDERSYSTEM_DLL, CXOR("66 0F 7F 0D 83 C9 43 ? 48 8B F7 66 0F 7F 05 88 C9 43 ?")), 0x4, 0x8));
	bSuccess &= (SwapChain != nullptr);

	if (SwapChain != nullptr)
	{
		if (FAILED(SwapChain->pDXGISwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&Device)))
		{
			printf(CXOR("failed to get device from swapchain\n"));

			CS_ASSERT(false);

			return false;
		}
		else
			Device->GetImmediateContext(&DeviceContext);
	}

	bSuccess &= (Device != nullptr && DeviceContext != nullptr);
	// Using hardcoded offsets from cs2-dumper instead of pattern scanning
	const void* hClientDll = MEM::GetModuleBaseHandle(CLIENT_DLL);
	if (hClientDll == nullptr)
		return false;
	
	Input = *reinterpret_cast<CCSGOInput**>(reinterpret_cast<std::uintptr_t>(hClientDll) + Offsets::dwCSGOInput);
	bSuccess &= (Input != nullptr);
	
	GlobalVars = *reinterpret_cast<IGlobalVars**>(reinterpret_cast<std::uintptr_t>(hClientDll) + Offsets::dwGlobalVars);
	bSuccess &= (GlobalVars != nullptr);
	
	// Note: MGameParticleManagerSystem still uses pattern scan as it's not in cs2-dumper offsets
	MGameParticleManagerSystem = reinterpret_cast<CGameParticleManagerSystem*>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, CXOR("49 63 81 ?? ?? ?? ?? 85 C0")), 0x3));
	bSuccess &= (MGameParticleManagerSystem != nullptr);

	return bSuccess && SetupBaseSystems();
}

// factory
class CBaseGameSystemFactory;
CBaseGameSystemFactory* pFactoryList;
class CBaseGameSystemFactory {
public:
	void* vTable;
	CBaseGameSystemFactory* pNext;
	const char* szName;
	void* pGameSystem;
};

template<typename T>
T GetFactory(const char* szFactoryName) {
	for (CBaseGameSystemFactory* pFactory = pFactoryList; pFactory != nullptr; pFactory = pFactory->pNext) {
		if (strcmp(szFactoryName, pFactory->szName) != 0)
			continue;

		printf(CXOR("factory %s found\n"), pFactory->szName);
		return reinterpret_cast<T>(pFactory->pGameSystem);
	}
}

bool I::SetupBaseSystems() {
	bool bSuccess = true;

	// get factory list
	pFactoryList = *reinterpret_cast<CBaseGameSystemFactory**>(MEM::ResolveRelativeAddress(MEM::FindPattern(CLIENT_DLL, CXOR("48 8B 3D ? ? ? ? 48 85 FF 0F 84 ? ? ? ? BE")), 0x3, 0x7));
	bSuccess &= (pFactoryList != nullptr);

	// get factories
	GSF::pInventoryManager = GetFactory<CCSInventoryManager*>("CCSInventoryManager");
	bSuccess &= (GSF::pInventoryManager != nullptr);

	GSF::pGCClientSystem = GetFactory<CCSGCClientSystem*>("CCSGCClientSystem");
	bSuccess &= (GSF::pGCClientSystem != nullptr);

	return bSuccess;
}

// end shit

bool I::CreateRenderTarget(IDXGISwapChain* pSwapChain) 
{
	SwapChain->pDXGISwapChain = pSwapChain;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	if (FAILED(SwapChain->pDXGISwapChain->GetDesc(&swapChainDesc))) 
	{
		printf(CXOR("Failed to get swap chain description.\n"));
		return false;
	}

	if (FAILED(SwapChain->pDXGISwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<PVOID*>(&Device)))) 
	{
		printf(CXOR("Failed to get device from swap chain.\n"));
		return false;
	}

	Device->GetImmediateContext(&DeviceContext);

	ID3D11Texture2D* back_buffer;

	if (FAILED(SwapChain->pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<PVOID*>(&back_buffer)))) 
	{
		printf(CXOR("Failed to get buffer from swap chain.\n"));
		return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC desc;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

	if (FAILED(Device->CreateRenderTargetView(back_buffer, &desc, &RenderTargetView))) 
	{
		back_buffer->Release();

		printf(CXOR("Failed to create render target view.\n"));

		return false;
	}

	back_buffer->Release();

	return true;
}

void I::DestroyRenderTarget()
{
	if (RenderTargetView != nullptr)
	{
		RenderTargetView->Release();
		RenderTargetView = nullptr;
	}
}