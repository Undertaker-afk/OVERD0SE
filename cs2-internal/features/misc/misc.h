#pragma once

#include "../../sdk/interfaces/events.h"

#include <memory>
#include <unordered_map>

class CUserCmd;
class CBaseUserCmdPB;
class CCSGOInputHistoryEntryPB;
class CBasePlayerController;
class CCSPlayerController;
class C_CSPlayerPawn;
class CSubtickMoveStep;
class QAngle_t;
class CCSGOInput;
class CViewSetup;
class C_PostProcessingVolume;
class C_EnvSky;
class CSceneLightObject;
class CGameParticleManagerSystem;
class c_particle_snapshot;

class c_misc {

public:
	void update_camera(CCSGOInput* thisptr);
	void skybox_changer();
	void map_exposure_correction(C_PostProcessingVolume* pPostProcessingVolume);
	void map_color_correction(C_CSPlayerPawn* localPlayerPawn);
	void buy_bot();
	void lighting(CSceneLightObject* pSceneLightObject);
};

const auto g_misc = std::make_unique<c_misc>();