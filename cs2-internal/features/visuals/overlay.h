#pragma once

#include "../../utilities/common.h"
#include "../../utilities/draw.h"

#include "../../sdk/datatypes/vector.h"
#include "../../sdk/datatypes/transform.h"

#include <memory>
#include <unordered_map>

class CCSPlayerController;
class C_BaseEntity;
class C_CSPlayerPawn;
class CBaseHandle;
class CEntityInstance;

namespace g_visuals_overlay
{
	void Render();
	void RenderIndicators();
	void CalculateBoundingBoxes();
	void RenderFOV(C_CSPlayerPawn* player, CCSPlayerController* pPlayerController);
	void CalculateSkeleton(CCSPlayerController* pPlayerController, C_CSPlayerPawn* pPlayer, const ImVec4& out);

	bool IsValid(CCSPlayerController* pEntity);

	void render_esp(CCSPlayerController* pEntity, const ImVec4& out);
}