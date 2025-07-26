#pragma once

class CUserCmd;
class CBaseUserCmdPB;
class CCSGOInputHistoryEntryPB;
class CBasePlayerController;
class CCSPlayerController;
class C_CSPlayerPawn;
class CSubtickMoveStep;
class QAngle_t;
class C_CSWeaponBase;

namespace g_misc_movement
{
	void BunnyHop(CUserCmd* pCmd, CBaseUserCmdPB* pUserCmd, C_CSPlayerPawn* pLocalPawn);
	void AutoStrafe(CBaseUserCmdPB* pUserCmd, C_CSPlayerPawn* pLocalPawn);
	void AutoStrafeDir(CBaseUserCmdPB* pUserCmd, C_CSPlayerPawn* pLocalPawn);
	void ClampMovement(CUserCmd* pCmd, float speed);
	void MovementFix(CUserCmd* pCmd, float angle);

	inline int m_scrolls{ };
	inline int m_max_scrolls{ };
}