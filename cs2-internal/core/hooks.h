#pragma once

#include <d3d11.h>
#include <dxgi1_2.h>

#include "../utilities/detourhook.h"

#include "../sdk/entity_handle.h"
#include "../sdk/datatypes/matrix.h"
#include "../sdk/entity.h"

#include "../core/sdk.h"
#include "../core/silentvmt/ShadowVMT.h"
#include "../core/silentvmt/InlineHook.h"

namespace VTABLE
{
	namespace D3D
	{
		enum {
			PRESENT = 8U,
			RESIZEBUFFERS = 13U,
			RESIZEBUFFERS_CSTYLE = 39U,
		};
	}

	namespace DXGI
	{
		enum {
			CREATESWAPCHAIN = 10U,
		};
	}

	namespace CLIENT
	{
		enum {
			CREATEMOVE = 5U,
			CAMERA = 7U,
			MOUSEINPUTENABLED = 19U,
			PREDICTION = 21U,
			FRAMESTAGENOTIFY = 36U,
		};
	}

	namespace INVENTORY
	{
		enum {
			EQUIPITEMLOADOUT = 54U,
		};
	}

	namespace SDL
	{
		enum {
			ISRELATIVEMOUSEMODE = 76U,
		};
	}
}

class CNetInputMessage;
class CRenderGameSystem;
class IViewRender;
class CCSGOInput;
class CViewSetup;
class CGlowProperty;
class IGameEvent;
class C_BaseModelEntity;
class material_data_t;
class CGCClientSystem;
class PostProcessingTonemapParameters_t;
class CGCClientSharedObjectTypeCache;
class CCSGOInputHistoryEntryPB;
class CCSInputMessage;
class CAggregateSceneObject;
class C_PostProcessingVolume;
class CSceneLightObject;
class CBaseSceneData;

namespace sigs 
{
	extern CSigScan GetHitboxSet;
	extern CSigScan HitboxToWorldTransforms;
	extern CSigScan ComputeHitboxSurroundingBox;
	extern CSigScan GetBoneName;
}

namespace H
{

	bool Setup();
	void Destroy();

	HRESULT WINAPI Present(IDXGISwapChain* pSwapChain, UINT uSyncInterval, UINT uFlags);
	HRESULT CS_FASTCALL ResizeBuffers(IDXGISwapChain* pSwapChain, std::uint32_t nBufferCount, std::uint32_t nWidth, std::uint32_t nHeight, DXGI_FORMAT newFormat, std::uint32_t nFlags);
	HRESULT WINAPI CreateSwapChain(IDXGIFactory* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain);
	
	long CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	ViewMatrix_t* CS_FASTCALL GetMatrixForView(CRenderGameSystem* pRenderGameSystem, IViewRender* pViewRender, ViewMatrix_t* pOutWorldToView, ViewMatrix_t* pOutViewToProjection, ViewMatrix_t* pOutWorldToProjection, ViewMatrix_t* pOutWorldToPixels);
	
	bool CS_FASTCALL CreateMove(CCSGOInput* pInput, int nSlot, CUserCmd* pCmd);

	void* CS_FASTCALL DrawViewModel(float* a1, float* offsets, float* fov);
	void* IsRelativeMouseMode(void* pThisptr, bool bActive);

	bool CS_FASTCALL FireEventClientSide(void* rcx, IGameEvent* event, bool bServerOnly);
	bool CS_FASTCALL MouseInputEnabled(void* pThisptr);
	
	void* CS_FASTCALL OnAddEntity(void* rcx, CEntityInstance* pInstance, CBaseHandle hHandle);
	void* CS_FASTCALL DrawSky(void* a1, void* a2);
	void* CS_FASTCALL OnRemoveEntity(void* rcx, CEntityInstance* pInstance, CBaseHandle hHandle);
	void CS_FASTCALL FrameStageNotify(void* rcx, int nFrameStage);
	
	__int64* CS_FASTCALL LevelInit(void* pClientModeShared, const char* szNewMap);
	__int64 CS_FASTCALL LevelShutdown(void* pClientModeShared);
	
	void CS_FASTCALL OverrideView(void* pClientModeCSNormal, CViewSetup* pSetup);
	void CS_FASTCALL DrawGlow(CGlowProperty* glow_property);
	void CS_FASTCALL DrawObject(void* pAnimatableSceneObjectDesc, void* pDx11, material_data_t* arrMeshDraw, int nDataCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2);
	void CS_FASTCALL HandleGameEvents(void* rcx, IGameEvent* const event);
	
	int64_t CS_FASTCALL	SendNetInputMessage(CNetInputMessage* a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);
	
	void CS_FASTCALL CameraInput(CCSGOInput* Input, int a1);
	void* CS_FASTCALL UpdateScens(CAggregateSceneObject* scn_obj, void* a2);
	void CS_FASTCALL DrawSceneObject(void* ptr, void* a2, CBaseSceneData* scene_data, int count, int a5, void* a6, void* a7, void* a8);
	void* CS_FASTCALL UpdatePostProcessing(C_PostProcessingVolume* pPostProcessingVolume, int a2);
	void CS_FASTCALL LightingModulate(void* pLightBinnerGPU, CSceneLightObject* pSceneLightObject, __int64 a3);
	void CS_FASTCALL granade_cam(void* a1, int a2, char a3, int a4);
	void* CS_FASTCALL SmokeUpdate(void* a);
	void* CS_FASTCALL RenderLegsFPV(void* a1, void* a2, void* a3, void* a4, void* a5);
	void* CS_FASTCALL ViewModelUpdate(void* a1, void* a2, void* a3, void* a4, bool a5);
	
	bool CS_FASTCALL Pvs(C_CSPlayerPawn* a1);
	bool CS_FASTCALL ShouldRender(C_CSPlayerPawn* a1, int splitscreen);
	
	void* CS_FASTCALL HudUpdate(void* a1);
	void* CS_FASTCALL InputParser(CCSInputMessage* a1, CCSGOInputHistoryEntryPB* a2, char a3, void* a4, void* a5, void* a6);
	void __fastcall SetModel(void* pawn, const char* szModelName);
	inline ShadowVMT SilentEntitySystem{ };
	bool OnEquipItemInLoadout(int nTeam, int nSlot, std::uint64_t nItemID);
	inline CBaseHookObject<decltype(&Present)> hkPresent = {};
	inline CBaseHookObject<decltype(&ResizeBuffers)> hkResizeBuffers = {};
	inline CBaseHookObject<decltype(&GetMatrixForView)> hkGetMatrixForView = {};
	inline CBaseHookObject<decltype(&HandleGameEvents)> hkGameEvents = {};
	inline CBaseHookObject<decltype(&LevelInit)> hkLevelInit = {};
	inline CBaseHookObject<decltype(&DrawObject)> hkDrawObject = {};
	inline CBaseHookObject<decltype(&CreateMove)> hkCreateMove = {};
	inline CBaseHookObject<decltype(&LevelShutdown)> hkLevelShutdown = {};
	inline CBaseHookObject<decltype(&FireEventClientSide)> hkPreFireEvent = {};
	inline CBaseHookObject<decltype(&FrameStageNotify)> hkFrameStageNotify = {};
	inline CBaseHookObject<decltype(&granade_cam)> hkgranade_cam = {};
	inline CBaseHookObject<decltype(&SendNetInputMessage)> hkSendInputMessage = {};
	inline CBaseHookObject<decltype(&CameraInput)> hkCameraInput = {};
	inline CBaseHookObject<decltype(&InputParser)> hkInputParser = {};
	inline CBaseHookObject<decltype(&SmokeUpdate)> hkSmokeModulatione = {};
	inline CBaseHookObject<decltype(&ViewModelUpdate)> hkViewModelUpdate = {};
	inline CBaseHookObject<decltype(&RenderLegsFPV)> hkRenderLegsFPV = {};
	inline CBaseHookObject<decltype(&ShouldRender)> hkShouldRender = {};
	inline CBaseHookObject<decltype(&DrawGlow)> hkDrawGlow = {};
	inline CBaseHookObject<decltype(&OverrideView)> hkOverrideView = {};
	inline CBaseHookObject<decltype(&LightingModulate)> hkLightingModulate = {};
	inline CBaseHookObject<decltype(&DrawSceneObject)> hkDrawSceneObject = {};
	inline CBaseHookObject<decltype(&Pvs)> hkpvs = {};
	inline CBaseHookObject<decltype(&UpdatePostProcessing)> hkUpdatePostProcessing = {};
	inline CBaseHookObject<decltype(&UpdateScens)> hkUpdateScens = {};
	inline CBaseHookObject<decltype(&HudUpdate)> hkHudUpdate = {};
	inline CBaseHookObject<decltype(&DrawSky)> hkDrawSky = {};
	inline CBaseHookObject<decltype(&CreateSwapChain)> hkCreateSwapChain = {};
	inline CBaseHookObject<decltype(&IsRelativeMouseMode)> hkIsRelativeMouseMode = {};
	inline CBaseHookObject<decltype(&MouseInputEnabled)> hkMouseInputEnabled = {};
	inline CBaseHookObject<decltype(&DrawViewModel)> hkDrawViewModel = {};
	inline CBaseHookObject<decltype(&SetModel)> hkSetModel = {};
	inline CBaseHookObject<decltype(&OnEquipItemInLoadout)> hkOnEquipItemInLoadout = {};

}