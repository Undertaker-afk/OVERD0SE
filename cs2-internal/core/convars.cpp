#include <vector>
#include <algorithm>

#include "convars.h"
#include "interfaces.h"

#include "../sdk/interfaces/ienginecvar.h"

bool CONVAR::Setup()
{
	bool bSuccess = true;

	m_pitch = I::Cvar->Find(FNV1A::HashConst(CXOR("m_pitch")));
	bSuccess &= m_pitch != nullptr;

	m_yaw = I::Cvar->Find(FNV1A::HashConst(CXOR("m_yaw")));
	bSuccess &= m_yaw != nullptr;

	sensitivity = I::Cvar->Find(FNV1A::HashConst(CXOR("sensitivity")));
	bSuccess &= sensitivity != nullptr;

	game_type = I::Cvar->Find(FNV1A::HashConst(CXOR("game_type")));
	bSuccess &= game_type != nullptr;

	game_mode = I::Cvar->Find(FNV1A::HashConst(CXOR("game_mode")));
	bSuccess &= game_mode != nullptr;

	mp_teammates_are_enemies = I::Cvar->Find(FNV1A::HashConst(CXOR("mp_teammates_are_enemies")));
	bSuccess &= mp_teammates_are_enemies != nullptr;

	sv_autobunnyhopping = I::Cvar->Find(FNV1A::HashConst(CXOR("sv_autobunnyhopping")));
	bSuccess &= sv_autobunnyhopping != nullptr;

	cam_idealdist = I::Cvar->Find(FNV1A::HashConst(CXOR("cam_idealdist")));
	bSuccess &= cam_idealdist != nullptr;

	cam_collision = I::Cvar->Find(FNV1A::HashConst(CXOR("cam_collision")));
	bSuccess &= cam_collision != nullptr;

	cam_snapto = I::Cvar->Find(FNV1A::HashConst(CXOR("cam_snapto")));
	bSuccess &= cam_snapto != nullptr;

	c_thirdpersonshoulder = I::Cvar->Find(FNV1A::HashConst(CXOR("c_thirdpersonshoulder")));
	bSuccess &= c_thirdpersonshoulder != nullptr;

	c_thirdpersonshoulderaimdist = I::Cvar->Find(FNV1A::HashConst(CXOR("c_thirdpersonshoulderaimdist")));
	bSuccess &= c_thirdpersonshoulderaimdist != nullptr;

	c_thirdpersonshoulderdist = I::Cvar->Find(FNV1A::HashConst(CXOR("c_thirdpersonshoulderdist")));
	bSuccess &= c_thirdpersonshoulderdist != nullptr;

	c_thirdpersonshoulderheight = I::Cvar->Find(FNV1A::HashConst(CXOR("c_thirdpersonshoulderheight")));
	bSuccess &= c_thirdpersonshoulderheight != nullptr;

	c_thirdpersonshoulderoffset = I::Cvar->Find(FNV1A::HashConst(CXOR("c_thirdpersonshoulderoffset")));
	bSuccess &= c_thirdpersonshoulderoffset != nullptr;

	cl_interpolate = I::Cvar->Find(FNV1A::HashConst(CXOR("cl_interpolate")));
	bSuccess &= cl_interpolate != nullptr;

	cl_interp_ratio = I::Cvar->Find(FNV1A::HashConst(CXOR("cl_interp_ratio")));
	bSuccess &= cl_interp_ratio != nullptr;

	return bSuccess;
}