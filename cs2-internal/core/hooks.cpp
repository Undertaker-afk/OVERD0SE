#include "hooks.h"

#include <DirectXMath.h>
#include <dxgi.h>
#include <d3d11.h>
#include <wrl/client.h>

#include "../sdk/interfaces/iswapchaindx11.h"
#include "../sdk/interfaces/iviewrender.h"
#include "../sdk/interfaces/cgameentitysystem.h"
#include "../sdk/interfaces/ccsgoinput.h"
#include "../sdk/interfaces/iinputsystem.h"
#include "../sdk/interfaces/iengineclient.h"
#include "../sdk/interfaces/ienginecvar.h"
#include "../sdk/interfaces/inetworkclientservice.h"
#include "../sdk/interfaces/iglobalvars.h"
#include "../sdk/interfaces/imaterialsystem.h"
#include "../sdk/datatypes/viewsetup.h"
#include "../sdk/entity.h"

#include "../core/memory/cmodule.hpp"

#include "../features/visuals/overlay.h"
#include "../features/visuals/visuals.h"
#include "../features/enginepred/pred.h"
#include "../features/rage/new_aim.hpp"
#include "../features/rage/anti_hit.hpp"
#include "../features/rage/lagcomp.h"
#include "../features/misc/misc.h"
#include "../features/legit/legit_aim.h"
#include "../features/skins/modelchanger.h"
#include "../features/configs/configs_var.h"

#include "../sdk/datatypes/viewsetup.h"
#include "../sdk/entity.h"
#include "../sdk/interfaces/events.h"

#include "../utilities/memory.h"
#include "../utilities/inputsystem.h"
#include "../utilities/draw.h"

#include "interfaces.h"
#include "sdk.h"
#include "spoofcall/invoker.h"
#include "convars.h"

#include "../menu/menu.h"
#include "../menu/widgets/widgets.h"

#define STB_OMIT_TESTS
#include "stb.hh"

#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include "../features/skins/inventorychanger.h"

using Microsoft::WRL::ComPtr;

#define SDK_SIG(sig) stb::simple_conversion::build<stb::fixed_string{sig}>::value

namespace sigs 
{
	CSigScan GetHitboxSet("C_BaseEntity::GetHitboxSet", "client.dll",
		{
			{SDK_SIG("E8 ? ? ? ? 48 85 C0 0F 85 ? ? ? ? 44 8D 48 07"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
			{SDK_SIG("41 8B D6 E8 ? ? ? ? 4C 8B F8"), [](CPointer& ptr) { ptr.Absolute(4, 0); }},
		});
	CSigScan GetBoneName("CModel::GetBoneName", "client.dll",
		{
			{SDK_SIG("48 8B CE E8 ? ? ? ? 48 8B 0F"), [](CPointer& ptr) { ptr.Offset(0x4); }},
		});
	CSigScan HitboxToWorldTransforms("C_BaseEntity::HitboxToWorldTransforms", "client.dll",
		{
			{SDK_SIG("E8 ? ? ? ? 4C 8B A3"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
		});

	CSigScan ComputeHitboxSurroundingBox("C_BaseEntity::ComputeHitboxSurroundingBox", "client.dll",
		{
			{SDK_SIG("E9 ? ? ? ? F6 43 5B FD"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
		});
}

bool H::Setup()
{
	MH_STATUS status = MH_Initialize();
	if (status != MH_OK) {
		printf("MH_Initialize failed: %d\n", status);
		return false;
	}
	printf(CXOR("Initializing ShadowVMT & SilentInlineHooking & Hooks\n"));

	CS_ASSERT(hkPresent.Create(MEM::GetVFunc(I::SwapChain->pDXGISwapChain, VTABLE::D3D::PRESENT), reinterpret_cast<void*>(&Present)));
	printf(CXOR("1\n"));
	CS_ASSERT(hkResizeBuffers.Create(MEM::GetVFunc(I::SwapChain->pDXGISwapChain, VTABLE::D3D::RESIZEBUFFERS), reinterpret_cast<void*>(&ResizeBuffers)));
	printf(CXOR("2\n"));

	ComPtr<IDXGIDevice> device;
	ComPtr<IDXGIAdapter> adapter;
	ComPtr<IDXGIFactory> factor;

	if (SUCCEEDED(I::Device->QueryInterface(IID_PPV_ARGS(&device)))) 
	{
		if (SUCCEEDED(device->GetAdapter(&adapter)))
			adapter->GetParent(IID_PPV_ARGS(&factor));
	}

	CS_ASSERT(hkCreateSwapChain.Create(MEM::GetVFunc(I::SwapChain->pIDXGIFactory, VTABLE::DXGI::CREATESWAPCHAIN), reinterpret_cast<void*>(&CreateSwapChain)));
	printf(CXOR("3\n"));

	SilentEntitySystem.Setup(I::GameResourceService->pGameEntitySystem);
	SilentEntitySystem.HookIndex(15, OnAddEntity);
	SilentEntitySystem.HookIndex(16, OnRemoveEntity);

	spoof_call<void>(_fake_addr, &EntCache::CacheCurrentEntities);

	CS_ASSERT(hkIsRelativeMouseMode.Create(MEM::GetVFunc(I::InputSystem, VTABLE::SDL::ISRELATIVEMOUSEMODE), reinterpret_cast<void*>(&IsRelativeMouseMode)));
	printf(CXOR("4\n"));
	CS_ASSERT(hkMouseInputEnabled.Create(MEM::GetVFunc(I::Input, VTABLE::CLIENT::MOUSEINPUTENABLED), reinterpret_cast<void*>(&MouseInputEnabled)));
	printf(CXOR("5\n"));

	CS_ASSERT(hkCreateMove.Create(MEM::FindPattern(CLIENT_DLL, CXOR("48 8B C4 4C 89 40 18 48 89 48 08 55 53 57")), reinterpret_cast<void*>(&CreateMove)));
	CS_ASSERT(hkDrawGlow.Create(MEM::FindPattern(CLIENT_DLL, CXOR("40 53 48 83 EC ? 48 8B 54 24")), reinterpret_cast<void*>(&DrawGlow)));
	CS_ASSERT(hkGetMatrixForView.Create(MEM::FindPattern(CLIENT_DLL, CXOR("40 53 48 81 EC ? ? ? ? 49 8B C1")), reinterpret_cast<void*>(&GetMatrixForView)));
	CS_ASSERT(hkSmokeModulatione.Create(MEM::FindPattern(CLIENT_DLL, CXOR("40 56 57 41 56 48 83 EC ? 8B 35")), reinterpret_cast<void*>(&SmokeUpdate)));
	CS_ASSERT(hkpvs.Create(MEM::FindPattern(CLIENT_DLL, CXOR("40 53 48 83 EC ? 48 8B 01 48 8B D9 FF 90 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 43")), (&Pvs)));
	CS_ASSERT(hkOverrideView.Create(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 48 8B FA E8")), (&OverrideView)));
	CS_ASSERT(hkShouldRender.Create(MEM::FindPattern(CLIENT_DLL, CXOR("40 53 48 83 EC ? 48 8B D9 83 FA ? 75 ? 48 8B 0D ? ? ? ? 48 8D 54 24 ? 48 8B 01 FF 90 ? ? ? ? 8B 10")), (&ShouldRender)));
	CS_ASSERT(hkRenderLegsFPV.Create(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 01")), reinterpret_cast<void*>(&RenderLegsFPV)));
	CS_ASSERT(hkGameEvents.Create(MEM::FindPattern(CLIENT_DLL, CXOR("40 55 53 56 57 41 55 41 57 48 8D 6C 24")), reinterpret_cast<void*>(&HandleGameEvents)));
	CS_ASSERT(hkHudUpdate.Create(MEM::FindPattern(CLIENT_DLL, CXOR("40 55 53 56 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 65 48 8B 04 25")), reinterpret_cast<void*>(&HudUpdate)));
	CS_ASSERT(hkDrawViewModel.Create(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8B EC 48 83 EC ? 4D 8B E0")), reinterpret_cast<void*>(&DrawViewModel)));
	CS_ASSERT(hkLevelInit.Create(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 ? 56 48 83 EC ? 48 8B 0D ? ? ? ? 48 8B F2")), reinterpret_cast<void*>(&LevelInit)));
	CS_ASSERT(hkDrawSceneObject.Create(MEM::FindPattern(SCENESYSTEM_DLL, CXOR("48 89 54 24 ? 55 57 41 55 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 49 63 F9")), reinterpret_cast<void*>(&DrawSceneObject)));
	CS_ASSERT(hkLightingModulate.Create(MEM::FindPattern(SCENESYSTEM_DLL, CXOR("48 89 54 24 ? 53 41 56 41 57")), reinterpret_cast<void*>(&LightingModulate)));
	CS_ASSERT(hkLevelShutdown.Create(MEM::FindPattern(CLIENT_DLL, CXOR("48 83 EC ? 48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 45 33 C9 45 33 C0 48 8B 01 FF 50 ? 48 85 C0 74 ? 48 8B 0D ? ? ? ? 48 8B D0 4C 8B 01 41 FF 50 ? 48 83 C4")), reinterpret_cast<void*>(&LevelShutdown)));
	CS_ASSERT(hkDrawObject.Create(MEM::FindPattern(SCENESYSTEM_DLL, CXOR("48 8B C4 48 89 50 10 53 41 55 41 56 48 81 EC ? ? ? ? 4D 63 F1")), reinterpret_cast<void*>(&DrawObject)));
	CS_ASSERT(hkUpdateScens.Create(MEM::FindPattern(SCENESYSTEM_DLL, CXOR("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 4C 8B F9")), reinterpret_cast<void*>(&UpdateScens)));
	CS_ASSERT(hkUpdatePostProcessing.Create(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 ? 57 48 83 EC ? 80 B9 ? ? ? ? ? 8B DA 48 8B F9")), reinterpret_cast<void*>(&UpdatePostProcessing)));
	CS_ASSERT(hkSetModel.Create(MEM::FindPattern(CLIENT_DLL, CXOR("40 53 48 83 EC 20 48 8B D9 4C 8B C2 48 8B 0D ? ? ? ? 48 8D 54 24")), reinterpret_cast<void*>(&SetModel)));
	CS_ASSERT(hkOnEquipItemInLoadout.Create(MEM::GetVFunc(GSF::pInventoryManager, 60u), reinterpret_cast<void*>(&OnEquipItemInLoadout)));
	
	CS_ASSERT(hkCameraInput.Create(MEM::GetVFunc(I::Input, VTABLE::CLIENT::CAMERA), reinterpret_cast<void*>(&CameraInput)));
	
	CS_ASSERT(hkFrameStageNotify.Create(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 10 56 48 83 EC 30 8B 05")), reinterpret_cast<void*>(&FrameStageNotify)));
	printf(CXOR("yes\n"));
	
	g_model_changer->update_player_models();

	return true;
}

void H::Destroy()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	MH_Uninitialize();

	SilentEntitySystem.UnhookAll();
}

void* CS_FASTCALL H::InputParser(CCSInputMessage* Input, CCSGOInputHistoryEntryPB* InputHistoryEntry, char a3, void* a4, void* a5, void* a6)
{
	auto original = hkInputParser.GetOriginal();
	auto result = original(Input, InputHistoryEntry, a3, a4, a5, a6);

	return result;
}

void __fastcall H::SetModel(void* pawn, const char* szModelName)
{
	changers::OnSetModel(static_cast<C_BaseModelEntity*>(pawn), szModelName);

	hkSetModel.GetOriginal()(pawn, szModelName);
}

bool H::OnEquipItemInLoadout(int nTeam, int nSlot, std::uint64_t nItemID)
{
	changers::OnEquipItemInLoadout(nTeam, nSlot, nItemID);

	return hkOnEquipItemInLoadout.GetOriginal()(nTeam, nSlot, nItemID);
}

void CS_FASTCALL H::CameraInput(CCSGOInput* Input, int a1) {
	auto horiginal = hkCameraInput.GetOriginal();

	if (!SDK::LocalPawn || !SDK::LocalPawn->is_alive() || !SDK::LocalController || !SDK::Cmd)
		return horiginal(Input, a1);

	auto backup = SDK::backup_VA = Input->GetViewAngles();

	horiginal(Input, a1);
	Input->SetViewAngle(backup);
}

void* CS_FASTCALL H::UpdateScens(CAggregateSceneObject* scn_obj, void* a2)
{
	const auto original = hkUpdateScens.GetOriginal();

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame() || !cfg_var::m_cfg->g_misc.nightmode)
		return original(scn_obj, a2);

	int count = scn_obj->count;

	for (int i = 0; i < count; i++) 
	{
		CAggregateSceneObjectData* pAggregateSceneObjectData = &scn_obj->array[i];

		pAggregateSceneObjectData->r = static_cast<unsigned char>(cfg_var::m_cfg->g_misc.colNightmode.colValue.r);
		pAggregateSceneObjectData->g = static_cast<unsigned char>(cfg_var::m_cfg->g_misc.colNightmode.colValue.g);
		pAggregateSceneObjectData->b = static_cast<unsigned char>(cfg_var::m_cfg->g_misc.colNightmode.colValue.b);
	}

	return original(scn_obj, a2);
}

int GetModelType(const std::string_view& name)
{
	if (name.find(CXOR("sun")) != std::string::npos
		|| name.find(CXOR("clouds")) != std::string::npos)
		return MODEL_SUN;

	if (name.find(CXOR("effects")) != std::string::npos)
		return MODEL_EFFECTS;

	return MODEL_OTHER;
}

void CS_FASTCALL H::DrawSceneObject(void* ptr, void* a2, CBaseSceneData* scene_data, int count, int a5, void* a6, void* a7, void* a8)
{
	const auto original = hkDrawSceneObject.GetOriginal();
	int type = GetModelType(scene_data->material->GetName());
	Color_t color;

	switch (type)
	{
	case MODEL_SUN:
	{
		color = cfg_var::m_cfg->g_misc.colSkyBox.colValue;
		break;
	}
	case MODEL_EFFECTS:
	case MODEL_OTHER:
	{
		color = cfg_var::m_cfg->g_misc.colNightmode.colValue;
		break;
	}
	}

	if (cfg_var::m_cfg->g_misc.nightmode)
	{
		for (int i = 0; i < count; ++i)
		{
			auto scene_ptr = &scene_data[i];
			if (scene_ptr)
			{
				scene_ptr->r = color.r;
				scene_ptr->g = color.g;
				scene_ptr->b = color.b;
				scene_ptr->a = color.a;
			}
		}
	}

	return original(ptr, a2, scene_data, count, a5, a6, a7, a8);
}

void* CS_FASTCALL H::DrawViewModel(float* a1, float* offsets, float* fov)
{
	const auto oDrawViewModel = hkDrawViewModel.GetOriginal();
	void* pReturn = oDrawViewModel(a1, offsets, fov);

	*offsets = cfg_var::m_cfg->g_misc.viewX;
	offsets[1] = cfg_var::m_cfg->g_misc.viewY;
	offsets[2] = cfg_var::m_cfg->g_misc.viewZ;
	*fov = cfg_var::m_cfg->g_misc.viewfov;

	return pReturn;
}

void* H::IsRelativeMouseMode(void* pThisptr, bool bActive)
{
	const auto oIsRelativeMouseMode = hkIsRelativeMouseMode.GetOriginal();

	MENU::bMainActive = bActive;

	if (cheat->menu_open)
		return oIsRelativeMouseMode(pThisptr, false);

	return oIsRelativeMouseMode(pThisptr, bActive);
}

void* CS_FASTCALL H::UpdatePostProcessing(C_PostProcessingVolume* pPostProcessingVolume, int a2)
{
	const auto original = hkUpdatePostProcessing.GetOriginal();

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame())
		return original(pPostProcessingVolume, a2);

	g_misc->map_exposure_correction(pPostProcessingVolume);

	return original(pPostProcessingVolume, a2);
}

void CS_FASTCALL H::LightingModulate(void* pLightBinnerGPU, CSceneLightObject* pSceneLightObject, __int64 a3)
{
	const auto original = hkLightingModulate.GetOriginal();

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame() || !cfg_var::m_cfg->g_misc.nightmode)
		return original(pLightBinnerGPU, pSceneLightObject, a3);

	g_misc->lighting(pSceneLightObject);

	return original(pLightBinnerGPU, pSceneLightObject, a3);
}

void CS_FASTCALL H::granade_cam(void* a1, int a2, char a3, int a4)
{
	const auto original = hkgranade_cam.GetOriginal();

	if (cfg_var::m_cfg->g_misc.smokeremove)
		return;
}

void* CS_FASTCALL H::SmokeUpdate(void* a)
{
	const auto original = hkSmokeModulatione.GetOriginal();

	if (!cfg_var::m_cfg->g_misc.smokeremove)
		return original(a);
}

void* CS_FASTCALL H::RenderLegsFPV(void* a1, void* a2, void* a3, void* a4, void* a5)
{
	const auto original = hkRenderLegsFPV.GetOriginal();

	if (!cfg_var::m_cfg->g_visuals.removelegs)
		return original(a1, a2, a3, a4, a5);
}

void* CS_FASTCALL H::ViewModelUpdate(void* a1, void* a2, void* a3, void* a4, bool a5)
{
	const auto original = hkViewModelUpdate.GetOriginal( );

	if ( cfg_var::m_cfg->g_misc.viewover )
	{
		static auto viewmodel_fov = I::Cvar->Find(FNV1A::HashConst(CXOR("viewmodel_fov")));
		static auto viewmodel_x = I::Cvar->Find(FNV1A::HashConst(CXOR("viewmodel_offset_x")));
		static auto viewmodel_y = I::Cvar->Find(FNV1A::HashConst(CXOR("viewmodel_offset_y")));
		static auto viewmodel_z = I::Cvar->Find(FNV1A::HashConst(CXOR("viewmodel_offset_z")));

		viewmodel_fov->set_float( cfg_var::m_cfg->g_misc.viewfov );
		viewmodel_x->set_float( cfg_var::m_cfg->g_misc.viewX );
		viewmodel_y->set_float( cfg_var::m_cfg->g_misc.viewY );
		viewmodel_z->set_float( cfg_var::m_cfg->g_misc.viewZ );
	}

	return original(a1, a2, a3, a4, a5);
}

bool CS_FASTCALL H::Pvs(C_CSPlayerPawn* a1)
{
	const auto original = hkpvs.GetOriginal();

	auto backup_PVS = a1->VisibleinPVS();

	a1->VisibleinPVS() = true;

	return 	original(a1);

	a1->VisibleinPVS() = backup_PVS;
}

bool CS_FASTCALL H::ShouldRender(C_CSPlayerPawn* a1, int splitscreen)
{
	const auto original = hkShouldRender.GetOriginal();

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame() || !SDK::LocalPawn)
		return 	original(a1, splitscreen);

	if (splitscreen != -1)
		return 	original(a1, splitscreen);

	if (a1 == SDK::LocalPawn)
		return false;

	return 	original(a1, splitscreen);
}

const char* m_groups[] = {
   ("body"),
   ("head"),
   ("chest"),
   ("body"),
   ("left arm"),
   ("right arm"),
   ("left leg"),
   ("right leg"),
   ("neck"),
   ("unknown"),
   ("gear")
};

void CS_FASTCALL H::HandleGameEvents(void* rcx, IGameEvent* const ev)
{
	const auto original = hkGameEvents.GetOriginal();

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame() || !SDK::LocalPawn)
		return original(rcx, ev);

	changers::OnPreFireEvent(ev);

	switch ( const char* event{ ev->GetName( ) }; FNV1A::Hash( event ) ) {
	case FNV1A::HashConst( CXOR( "player_hurt" ) ): 
	{
		if ( !SDK::LocalController )
			break;

		const auto attacker = ev->get_player_controller( CXOR( "attacker" ) );
		if ( attacker && attacker == SDK::LocalController )
		{
			const auto user_ = ev->get_player_controller( CXOR( "userid" ) );
			if ( user_ && user_ != SDK::LocalController )
			{
				C_CSPlayerPawn* pawn = I::GameResourceService->pGameEntitySystem->Get< C_CSPlayerPawn >( user_->GetPawnHandle( ) );
				if ( !pawn || !pawn->GetGameSceneNode( ) )
					return;

				const auto dmg_health{ ev->get_int( CXOR( "dmg_health" ) ) };
				const auto health{ ev->get_int( CXOR( "health" ) ) };
				const auto hit_group{ ev->get_int( CXOR( "hitgroup" ) ) };
				const auto name{ reinterpret_cast< CCSPlayerController* >( user_ )->GetPlayerName( ) };

				if ( health > 1 ) {
					if ( !cfg_var::m_cfg->g_misc.eventlogs[ 0 ] )
						return;

					widgets::add_log( std::format( "Hit {} in the {} for {} damage ({} remaining)", name, m_groups[ hit_group ], dmg_health, health ).c_str( ) );
				}
				else if ( health < 1 ) {
					if ( !cfg_var::m_cfg->g_misc.eventlogs[ 1 ] )
						return;

					widgets::add_log( std::format( "Kill {} in the {} for {} damage ({} remaining)", name, m_groups[ hit_group ], dmg_health, health ).c_str( ) );
				}
				else if ( dmg_health == 0 ) {
					if ( !cfg_var::m_cfg->g_misc.eventlogs[ 2 ] )
						return;

					widgets::add_log( std::format( "Missed shot to {}", name ).c_str( ) );
				}

				if ( dmg_health > 1 )
					SDK::last_hurt_time = I::GlobalVars->m_current_time;

				SDK::last_hp = health;
				SDK::last_dmg = dmg_health;
				SDK::hitted_hitbox = hit_group;
			}
		}
	} break;

	case FNV1A::HashConst( CXOR( "round_freeze_end" ) ): 
		SDK::freeze_time = false; 
		break;

	case FNV1A::HashConst( CXOR( "round_start" ) ): 
	{
		SDK::freeze_time = true;

		g_misc->buy_bot( );

		if ( !SDK::LocalController )
			break;

	} break;

	default:
		break;
	}

	original(rcx, ev);
}

HRESULT CS_FASTCALL H::ResizeBuffers(IDXGISwapChain* pSwapChain, std::uint32_t nBufferCount, std::uint32_t nWidth, std::uint32_t nHeight, DXGI_FORMAT newFormat, std::uint32_t nFlags)
{
	ImGui_ImplDX11_InvalidateDeviceObjects();

	const auto oResizeBuffer = hkResizeBuffers.GetOriginal();
	auto hResult = oResizeBuffer(pSwapChain, nBufferCount, nWidth, nHeight, newFormat, nFlags);

	if (SUCCEEDED(hResult))
		I::CreateRenderTarget(pSwapChain);

	return hResult;
}

HRESULT CS_STDCALL H::CreateSwapChain(IDXGIFactory* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain)
{
	const auto oCreateSwapChain = hkCreateSwapChain.GetOriginal();

	I::DestroyRenderTarget();

	return oCreateSwapChain(pFactory, pDevice, pDesc, ppSwapChain);
}

long H::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (D::OnWndProc(hWnd, uMsg, wParam, lParam))
		return 1L;

	return ::CallWindowProcW(IPT::pOldWndProc, hWnd, uMsg, wParam, lParam);
}

void* CS_FASTCALL H::OnAddEntity(void* rcx, CEntityInstance* pInstance, CBaseHandle hHandle) 
{
	spoof_call<void>(_fake_addr, EntCache::OnAddEntity, pInstance, hHandle);
	return SilentEntitySystem.GetOg<decltype(&OnAddEntity)>(15)(rcx, pInstance, hHandle);
}

void* CS_FASTCALL H::DrawSky(void* a1, void* a2)
{
	void* lastSky = nullptr;
	Vector_t lastColor = { 1.f, 1.f, 1.f };

	const auto oDrawSky = hkDrawSky.GetOriginal();

	if (a1 != lastSky) 
	{
		lastColor = *(Vector_t*)((__int64)a1 + 0xF8);
		lastSky = a1;
	}

	if (cfg_var::m_cfg->g_misc.nightmode)
		*(Vector_t*)((__int64)a1 + 0xF8) = { cfg_var::m_cfg->g_misc.colSkyBox.colValue.GetVec4().x, cfg_var::m_cfg->g_misc.colSkyBox.colValue.GetVec4().y, cfg_var::m_cfg->g_misc.colSkyBox.colValue.GetVec4().z };
	else
		*(Vector_t*)((__int64)a1 + 0xF8) = lastColor;

	return oDrawSky(a1, a2);
}

ViewMatrix_t* CS_FASTCALL H::GetMatrixForView(CRenderGameSystem* pRenderGameSystem, IViewRender* pViewRender, ViewMatrix_t* pOutWorldToView, ViewMatrix_t* pOutViewToProjection, ViewMatrix_t* pOutWorldToProjection, ViewMatrix_t* pOutWorldToPixels)
{
	const auto oGetMatrixForView = hkGetMatrixForView.GetOriginal();
	ViewMatrix_t* matResult = oGetMatrixForView(pRenderGameSystem, pViewRender, pOutWorldToView, pOutViewToProjection, pOutWorldToProjection, pOutWorldToPixels);

	SDK::ViewMatrix = *pOutWorldToProjection;
	SDK::CameraPosition = pViewRender->vecOrigin;

	if (I::Engine->IsConnected() && I::Engine->IsInGame())
		spoof_call<void>(_fake_addr, g_visuals_overlay::CalculateBoundingBoxes);

	return matResult;
}

void* CS_FASTCALL H::OnRemoveEntity(void* rcx, CEntityInstance* pInstance, CBaseHandle hHandle) {
	spoof_call<void>(_fake_addr, EntCache::OnRemoveEntity, pInstance, hHandle);

	return SilentEntitySystem.GetOg<decltype(&OnRemoveEntity)>(16)(rcx, pInstance, hHandle);
}

int64_t CS_FASTCALL H::SendNetInputMessage(CNetInputMessage* a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6)
{
	const auto bres = hkSendInputMessage.GetOriginal();
	return bres(a1, a2, a3, a4, a5, a6);
}

void* CS_FASTCALL H::HudUpdate(void* a1)
{
	const auto original = hkHudUpdate.GetOriginal();

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame() || !SDK::LocalPawn || !SDK::LocalPawn->GetHealth())
		return original(a1);

	if (cfg_var::m_cfg->g_misc.m_scopelocal <= 0)
		return original(a1);

	if (!SDK::LocalPawn->IsScoped())
		return original(a1);

	auto is_scoped = SDK::LocalPawn->IsScoped();

	SDK::LocalPawn->IsScoped() = false;

	return original(a1);

	SDK::LocalPawn->IsScoped() = is_scoped;
}

float smooth_angle_transition(float current_angle, float target_angle, float step)
{
	float delta = fmod(target_angle - current_angle + 180.0f, 360.0f) - 180.0f;

	if (std::abs(delta) > step)
		delta = (delta > 0) ? step : -step;

	return current_angle + delta;
}

float calculate_target_angle(bool move_left, bool move_right, bool move_backward, float current_angle) 
{
	float target_angle = current_angle;

	if (move_left)
		target_angle += move_backward ? 90 : -90;
	if (move_right)
		target_angle += move_backward ? -90 : 90;
	if (move_backward)
		target_angle -= 180;

	return target_angle;
}

bool CS_FASTCALL H::CreateMove(CCSGOInput* pInput, int nSlot, CUserCmd* pCmd)
{
	const auto oCreateMove = hkCreateMove.GetOriginal();
	const bool bResult = oCreateMove(pInput, nSlot, pCmd);

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame())
		return bResult;

	auto local_controller = SDK::LocalController = CCSPlayerController::GetLocalPlayerController();
	if (local_controller == nullptr)
		return bResult;

	auto pawn = SDK::LocalPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(local_controller->m_hPawn());
	if (pawn == nullptr)
		return bResult;

	CUserCmd* user_cmd = SDK::Cmd = pInput->GetUserCmd();
	if (user_cmd == nullptr)
		return  bResult;

	CBaseUserCmdPB* pBaseCmd = SDK::Cmd->m_csgoUserCmd.mutable_base();
	if (pBaseCmd == nullptr)
		return bResult;

	auto ActiveWeapon = SDK::LocalPawn->ActiveWeapon();
	if (!ActiveWeapon)
		return bResult;

	cheat->alive = SDK::LocalPawn->GetHealth() > 0 && SDK::LocalPawn->GetLifeState() != ELifeState::LIFE_DEAD;
	cheat->onground = (SDK::LocalPawn->GetFlags() & FL_ONGROUND);

	if (I::NetworkClientService) 
	{
		auto network_client = I::NetworkClientService->GetNetworkClient();

		if (network_client)
			cheat->lerp = network_client->GetClientInterpAmount();
	}

	if (SDK::LocalPawn->is_alive()) 
	{
		static auto draw_view_model = I::Cvar->Find(FNV1A::HashConst(CXOR("r_drawviewmodel")));
		static auto draw_grenade = I::Cvar->Find(FNV1A::HashConst(CXOR("sv_grenade_trajectory_prac_pipreview")));

		if (cfg_var::m_cfg->g_misc.granade_cam) 
		{
			draw_grenade->set_bool(true);
			I::Cvar->Find(FNV1A::HashConst(CXOR("sv_grenade_trajectory_prac_trailtime")))->set_float(15);

			SDK::LocalPawn->GetFlashDuration() = 0;
		} 
		else // disabled
			draw_grenade->set_bool(false);

		if ((SDK::LocalPawn->ActiveWeapon()->m_zoomLevel() != 0) && (SDK::LocalPawn->ActiveWeapon()->datawep()->m_WeaponType() == WEAPONTYPE_SNIPER_RIFLE))
			draw_view_model->set_bool(false);
		else
			draw_view_model->set_bool(true);

		I::Cvar->Find(FNV1A::HashConst(CXOR("sv_cheats")))->set_bool(true);
		I::Cvar->Find(FNV1A::HashConst(CXOR("fov_cs_debug")))->set_float(cfg_var::m_cfg->g_misc.ifov);

		cheat->viewangles = { user_cmd->m_csgoUserCmd.mutable_base()->viewangles().x(), user_cmd->m_csgoUserCmd.mutable_base()->viewangles().y() ,user_cmd->m_csgoUserCmd.mutable_base()->viewangles().z() };
		cheat->viewangles_vec = { user_cmd->m_csgoUserCmd.mutable_base()->viewangles().x(), user_cmd->m_csgoUserCmd.mutable_base()->viewangles().y() ,user_cmd->m_csgoUserCmd.mutable_base()->viewangles().z() };

		bool move_forward = SDK::Cmd->m_nButtons.m_nValue & IN_FORWARD, move_backward = SDK::Cmd->m_nButtons.m_nValue & IN_BACK;
		bool move_left = SDK::Cmd->m_nButtons.m_nValue & IN_MOVELEFT, move_right = SDK::Cmd->m_nButtons.m_nValue & IN_MOVERIGHT;

		float target_angle = calculate_target_angle(move_left, move_right, move_backward, cheat->viewangles_vec.y);
		float step = 200;

		cheat->viewangles_vec.y = smooth_angle_transition(cheat->viewangles_vec.y, target_angle, step);

		if (SDK::LocalPawn->IsValidMoveType()) 
		{
			g_misc_movement::BunnyHop(user_cmd, user_cmd->m_csgoUserCmd.mutable_base(), SDK::LocalPawn);
			g_misc_movement::AutoStrafe(user_cmd->m_csgoUserCmd.mutable_base(), SDK::LocalPawn);
			g_misc_movement::AutoStrafeDir(user_cmd->m_csgoUserCmd.mutable_base(), SDK::LocalPawn);
		}

		g_misc->update_camera(pInput);
		g_misc->skybox_changer();

		g_prediction->run();

		g_rage_bot->build_seed_table();

		g_rage_bot->automatic_stop(SDK::LocalPawn, ActiveWeapon, SDK::Cmd);

		g_rage_bot->calculate_max_accuracy();
		g_rage_bot->on_create_move();

		g_anti_hit->create_move(user_cmd);

		g_legit_bot->on_create_move(pBaseCmd, local_controller, SDK::LocalPawn);

		g_prediction->end();

		if (SDK::LocalPawn->IsValidMoveType())
			g_misc_movement::MovementFix(user_cmd, cheat->viewangles.y);
	}

	return bResult;
}

HRESULT CS_STDCALL H::Present(IDXGISwapChain* pSwapChain, UINT uSyncInterval, UINT uFlags)
{
	const auto oPresent = hkPresent.GetOriginal();

	static bool init{ false }, success_init{ false };

	static std::chrono::steady_clock::time_point init_time;

	// hook
	if (!init && I::RenderTargetView == nullptr)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&I::Device)))
		{
			I::Device->GetImmediateContext(&I::DeviceContext);
			DXGI_SWAP_CHAIN_DESC sd;

			pSwapChain->GetDesc(&sd);
			IPT::hWindow = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;

			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			I::Device->CreateRenderTargetView(pBackBuffer, NULL, &I::RenderTargetView);
			pBackBuffer->Release();

			init_time = std::chrono::steady_clock::now();
			init = true;
		}
		else
			return oPresent(I::SwapChain->pDXGISwapChain, uSyncInterval, uFlags);
	}

	// imgui & menu
	if (init)
	{
		if (!success_init)
		{
			D::InitImGui();

			success_init = true;
		}

		// init time
		float init_elp = std::chrono::duration<float>(std::chrono::steady_clock::now() - init_time).count();
		float remaining = min(init_elp / 1.5f, 1.0f);

		D::Render(remaining, remaining < 1.0f);
	}

	return oPresent(I::SwapChain->pDXGISwapChain, uSyncInterval, uFlags);
}

bool CS_FASTCALL H::FireEventClientSide(void* rcx, IGameEvent* event, bool bServerOnly)
{
	const auto oPreFire = hkPreFireEvent.GetOriginal();

	return oPreFire(rcx, event, bServerOnly);
}

bool CS_FASTCALL H::MouseInputEnabled(void* pThisptr)
{
	const auto oMouseInputEnabled = hkMouseInputEnabled.GetOriginal();
	return cheat->menu_open ? false : oMouseInputEnabled(pThisptr);
}

void CS_FASTCALL H::FrameStageNotify(void* rcx, int nFrameStage)
{
	const auto oFrameStageNotify = hkFrameStageNotify.GetOriginal();

	if (!I::Engine->IsConnected() && !I::Engine->IsInGame())
		return oFrameStageNotify(rcx, nFrameStage);

	g_misc->map_color_correction(SDK::LocalPawn);
	changers::OnFrameStageNotify(nFrameStage);
	//static auto vis = I::Cvar->Find(FNV1A::HashConst(CXOR("sc_no_vis")));
	//vis->set_bool(true);

	switch (nFrameStage)
	{
	case FRAME_START:
	case FRAME_NET_UPDATE_START:
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
	case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
	case FRAME_NET_UPDATE_END:
	case FRAME_RENDER_START:
		break;

	case FRAME_RENDER_END:
		if (cfg_var::m_cfg->g_skins.model_changer)
			g_model_changer->set_player_model();

		g_rage_bot->store_records();
		break;

	case FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE:
		break;
	default:
		break;
	}

	return oFrameStageNotify(rcx, nFrameStage);
}

__int64* CS_FASTCALL H::LevelInit(void* pClientModeShared, const char* szNewMap)
{
	const auto oLevelInit = hkLevelInit.GetOriginal();

	if (I::GlobalVars == nullptr)
		I::GlobalVars = *reinterpret_cast<IGlobalVars**>(MEM::ResolveRelativeAddress(MEM::FindPattern(CLIENT_DLL, CXOR("48 8B 0D ? ? ? ? 4C 8D 05 ? ? ? ? 48 85 D2")), 0x3, 0x7));

	return oLevelInit(pClientModeShared, szNewMap);
}

__int64 CS_FASTCALL H::LevelShutdown(void* pClientModeShared)
{
	const auto oLevelShutdown = hkLevelShutdown.GetOriginal();
	I::GlobalVars = nullptr;

	return oLevelShutdown(pClientModeShared);
}

void CS_FASTCALL H::OverrideView(void* pClientModeCSNormal, CViewSetup* pSetup)
{
	const auto oOverrideView = hkOverrideView.GetOriginal();

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame())
		return hkOverrideView.GetOriginal()(pClientModeCSNormal, pSetup);

	if (!SDK::LocalController->IsPawnAlive() || !SDK::LocalController)
		return hkOverrideView.GetOriginal()(pClientModeCSNormal, pSetup);

	// @credits: imhartash
	if (cfg_var::m_cfg->g_misc.enable_aspectratio) 
	{
		pSetup->nSomeFlags |= 2;
		pSetup->flAspectRatio = cfg_var::m_cfg->g_misc.aspectratio;
	} 
	else
		pSetup->nSomeFlags &= ~2;

	return oOverrideView(pClientModeCSNormal, pSetup);
}

void CS_FASTCALL H::DrawGlow(CGlowProperty* glow_property)
{
	const auto oDrawGlow = hkDrawGlow.GetOriginal();

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame() || !SDK::LocalPawn)
		return 	oDrawGlow(glow_property);

	auto owner = glow_property->m_hOwner;

	SchemaClassInfoData_t* pClassInfo = nullptr;
	owner->GetSchemaClassInfo(&pClassInfo);

	if (pClassInfo == nullptr)
		return 	oDrawGlow(glow_property);

	const FNV1A_t uHashedName = FNV1A::Hash(pClassInfo->szName);

	if (uHashedName != FNV1A::HashConst(CXOR("C_CSPlayerPawn")))
		return oDrawGlow(glow_property);

	C_CSPlayerPawn* pawn = owner->As<C_CSPlayerPawn>();

	if (!pawn->is_alive())
		return oDrawGlow(glow_property);

	if (SDK::LocalPawn && SDK::LocalPawn == pawn && LOWORD(GetKeyState(cfg_var::m_cfg->g_misc.thirdperson)))
	{
		glow_property->m_glowColorOverride() = cfg_var::m_cfg->g_visuals.chams_local.colLocalGlow.colValue;

		if (cfg_var::m_cfg->g_visuals.chams_local.LocalGlow)
			glow_property->m_bGlowing() = true;
	}

	if (!pawn->GetTeam())
		return;

	if (SDK::LocalPawn && (SDK::LocalPawn == pawn || SDK::LocalPawn->GetTeam() == pawn->GetTeam()))
		return 	oDrawGlow(glow_property);
}

void CS_FASTCALL H::DrawObject(void* pAnimatableSceneObjectDesc, void* pDx11, material_data_t* arrMeshDraw, int nDataCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2)
{
	const auto oDrawObject = hkDrawObject.GetOriginal();
	if (!I::Engine->IsConnected() || !I::Engine->IsInGame())
		return oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);

	if (SDK::LocalController == nullptr || SDK::LocalPawn == nullptr)
		return oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);

	if (!F::VISUALS::OnDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2))
		oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
}