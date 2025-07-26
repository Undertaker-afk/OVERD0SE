#include "misc.h"

#include <fstream>
#include <iostream>

#include "../features/enginepred/pred.h"
#include "../features/configs/configs_var.h"

#include "../../core/sdk.h"

#include "../../sdk/interfaces/ccsgoinput.h"
#include "../../sdk/interfaces/ienginecvar.h"
#include "../../sdk/interfaces/iengineclient.h"
#include "../../sdk/interfaces/cgameentitysystem.h"
#include "../../sdk/entity.h"

void c_misc::update_camera(CCSGOInput* thisptr)
{
	if (!I::Engine->IsConnected() || !I::Engine->IsInGame())
		return;

	static auto cam_collision = I::Cvar->Find( FNV1A::HashConst( CXOR( "cam_collision" ) ) );
	static auto cam_idealdist = I::Cvar->Find( FNV1A::HashConst( CXOR( "cam_idealdist" ) ) );

	cam_collision->set_int( 0 );
	cam_idealdist->set_float( cfg_var::m_cfg->g_misc.thirdpersondist );

	bool bThirdPerson = LOWORD( GetKeyState( cfg_var::m_cfg->g_misc.thirdperson ) );

	// fix camera
	if ( thisptr->bInThirdPerson != bThirdPerson )
	{
		Vector_t view_angles{ I::Input->GetViewAngles( ).x, I::Input->GetViewAngles( ).y, I::Input->GetViewAngles( ).z };

		if ( bThirdPerson )
			thisptr->vecThirdPersonAngles = view_angles;

		thisptr->bInThirdPerson = bThirdPerson;
	}

	thisptr->vecThirdPersonAngles.z = cfg_var::m_cfg->g_misc.thirdpersondist;
}

void c_misc::skybox_changer()
{
	if (!I::Engine->IsConnected() || !I::Engine->IsInGame())
		return;

	for (int i = 64; i < I::GameResourceService->pGameEntitySystem->GetHighestEntityIndex(); i++) {

		CEntityInstance* cEnt = (CEntityInstance*)I::GameResourceService->pGameEntitySystem->Get(i);

		if (cEnt == nullptr)
			continue;

		CEntityIdentity* pEntity = cEnt->GetIdentity();

		if (!pEntity)
			continue;

		if (!pEntity->GetDesignerName())
			continue;

		if (pEntity->GetDesignerName() == CXOR("env_sky"))
		{
			C_EnvSky* SkyEntity = I::GameResourceService->pGameEntitySystem->Get<C_EnvSky>(i);

			SkyEntity->m_bEnabled() = true;
			SkyEntity->m_vTintColorLightingOnly() = cfg_var::m_cfg->g_misc.colLighting.colValue;
			SkyEntity->m_vTintColor() = cfg_var::m_cfg->g_misc.colLighting.colValue;
			SkyEntity->m_flFogMaxStart() = SkyEntity->m_flFogMinStart() = cfg_var::m_cfg->g_misc.map_tone;
			SkyEntity->m_flFogMinEnd() = SkyEntity->m_flFogMaxEnd() = cfg_var::m_cfg->g_misc.map_tone;
			//SkyEntity->m_nFogType() = 2;
		}
	}
}

void c_misc::map_exposure_correction(C_PostProcessingVolume* pPostProcessingVolume)
{
	if (!SDK::LocalPawn)
		return;

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame())
		return;

	pPostProcessingVolume->m_bExposureControl() = true;
	pPostProcessingVolume->m_flExposureFadeSpeedDown() = pPostProcessingVolume->m_flExposureFadeSpeedUp() = 0;
	pPostProcessingVolume->m_flMinExposure() = pPostProcessingVolume->m_flMaxExposure() = cfg_var::m_cfg->g_misc.map_tone * 0.01f;
}

void c_misc::map_color_correction(C_CSPlayerPawn* localPlayerPawn)
{
	using UpdateCameraServices_t = void(CS_FASTCALL*)(CPlayer_CameraServices*, int);
	static UpdateCameraServices_t UpdateCameraServices = reinterpret_cast<UpdateCameraServices_t>(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 08 57 48 83 EC 20 8B FA 48 8B D9 E8 ?? ?? ?? ?? 84 C0 0F 84")));

	static float previousExposure = -1;
	float exposure = static_cast<float>(cfg_var::m_cfg->g_misc.map_tone);

	if (previousExposure == exposure)
		return;

	if (!localPlayerPawn)
		return;

	for (int i = 1; i <= I::GameResourceService->pGameEntitySystem->GetHighestEntityIndex(); i++)
	{
		auto entity = I::GameResourceService->pGameEntitySystem->Get< C_BaseEntity >(i);
		if (entity == nullptr)
			continue;

		SchemaClassInfoData_t* pClassInfo = nullptr;
		entity->GetSchemaClassInfo(&pClassInfo);

		if (pClassInfo == nullptr)
			continue;

		const FNV1A_t uHashedName = FNV1A::Hash(pClassInfo->szName);
		if (uHashedName != FNV1A::HashConst(CXOR("C_PostProcessingVolume")))
			continue;

		C_PostProcessingVolume* postProcessingVolume = reinterpret_cast<C_PostProcessingVolume*>(entity);
		if (postProcessingVolume == nullptr)
			continue;

		previousExposure = exposure;

		if (cfg_var::m_cfg->g_misc.nightmode)
		{
			map_exposure_correction(postProcessingVolume);
			UpdateCameraServices(localPlayerPawn->GetCameraServices(), 0);
		}
	}
}

void c_misc::buy_bot()
{
	if (cfg_var::m_cfg->g_misc.m_buy_bot_enable)
	{
		std::string buy;

		switch (cfg_var::m_cfg->g_misc.m_buybot1)
		{
		case 1:
			buy += CXOR("buy g3sg1; buy scar20; ");
			break;
		case 2:
			buy += CXOR("buy awp; ");
			break;
		case 3:
			buy += CXOR("buy ssg08; ");
			break;
		}

		switch (cfg_var::m_cfg->g_misc.m_buybot2)
		{
		case 1:
			buy += CXOR("buy elite; ");
			break;
		case 2:
			buy += CXOR("buy fiveseven; buy tec9; ");
			break;
		case 3:
			buy += CXOR("buy deagle; buy revolver; ");
			break;
		}

		if (cfg_var::m_cfg->g_misc.m_buybot3[0])
			buy += CXOR("buy vesthelm; buy vest; ");

		if (cfg_var::m_cfg->g_misc.m_buybot3[1])
			buy += CXOR("buy taser; ");

		if (cfg_var::m_cfg->g_misc.m_buybot3[2])
			buy += CXOR("buy molotov; buy hegrenade; buy smokegrenade; ");

		if (cfg_var::m_cfg->g_misc.m_buybot3[3])
			buy += CXOR("buy defuser; ");

		I::Engine->unrestricted_cmd(buy.c_str());
	}
}

void c_misc::lighting(CSceneLightObject* pSceneLightObject)
{
	pSceneLightObject->r = cfg_var::m_cfg->g_misc.colLighting.colValue.GetVec4().x;
	pSceneLightObject->g = cfg_var::m_cfg->g_misc.colLighting.colValue.GetVec4().y;
	pSceneLightObject->b = cfg_var::m_cfg->g_misc.colLighting.colValue.GetVec4().z;
}