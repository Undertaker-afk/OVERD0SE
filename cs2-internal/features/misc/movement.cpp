#include "movement.h"
#include <iostream>
#include <array>
#include <cstddef> 
#include <xcharconv.h>
#include <xiosbase>

#include "../../sdk/entity.h"
#include "../../sdk/datatypes/usercmd.h"
#include "../../sdk/interfaces/ccsgoinput.h"
#include "../../sdk/datatypes/qangle.h"
#include "../../sdk/interfaces/ienginecvar.h"
#include "../../sdk/interfaces/itrace.h"
#include "../../sdk/interfaces/cgameentitysystem.h"

#include "../../features/enginepred/pred.h"
#include "../../features/rage/anti_hit.hpp"
#include "../../features/configs/configs_var.h"

#include "../../core/sdk.h"
#include "../../core/convars.h"

void g_misc_movement::BunnyHop(CUserCmd* pCmd, CBaseUserCmdPB* pUserCmd, C_CSPlayerPawn* pLocalPawn)
{
	if (!cfg_var::m_cfg->g_misc.bunny || CONVAR::sv_autobunnyhopping->value.i1)
		return;

	if (!SDK::LocalController || !SDK::LocalPawn)
		return;

	if ((pLocalPawn->GetActualMoveType() == MOVETYPE_LADDER) || (pLocalPawn->GetActualMoveType() == MOVETYPE_NOCLIP))
		return;

	if (pLocalPawn->GetFlags() & FL_ONGROUND)
		pCmd->m_nButtons.m_nValue &= ~IN_JUMP;

	bool jumping = (pCmd->m_nButtons.m_nValue & IN_JUMP) || (pCmd->m_nButtons.m_nValueChanged & IN_JUMP);

	if (!jumping)
		return;

	bool move_left = SDK::Cmd->m_nButtons.m_nValue & IN_MOVELEFT, move_right = SDK::Cmd->m_nButtons.m_nValue & IN_MOVERIGHT;

	pCmd->m_nButtons.m_nValue &= ~IN_JUMP;
	pCmd->m_nButtons.m_nValueChanged &= ~IN_JUMP;
	pCmd->m_nButtons.m_nValueScroll &= ~IN_JUMP;

	if (pLocalPawn->GetFlags() & FL_ONGROUND) 
	{
		m_scrolls = 1;

		if (move_left || move_right)
			m_max_scrolls = 5;
		else
			m_max_scrolls = 3;
	}

	if (m_scrolls && m_scrolls < m_max_scrolls)
	{
		pCmd->m_nButtons.m_nValueScroll |= IN_JUMP;

		pCmd->ProcessSubTick(pCmd->add_subtick_move_step(pCmd), IN_JUMP, 1, false, true);
		pCmd->ProcessSubTick(pCmd->add_subtick_move_step(pCmd), IN_JUMP, 1, true, false);

		m_scrolls++;
	}
}

void g_misc_movement::AutoStrafe(CBaseUserCmdPB* pUserCmd, C_CSPlayerPawn* pLocalPawn)
{
	if (!cfg_var::m_cfg->g_misc.strafe || (pLocalPawn->GetFlags() & FL_ONGROUND))
		return;

	if (!pUserCmd || !pUserCmd->has_viewangles() || !SDK::LocalPawn || !SDK::LocalPawn->is_alive())
		return;

	if (pLocalPawn->GetActualMoveType() == MOVETYPE_LADDER || pLocalPawn->GetActualMoveType() == MOVETYPE_NOCLIP)
		return;

	pUserCmd->set_forwardmove(0.f);
	pUserCmd->set_leftmove(0.f);

	Vector_t view_angels = cheat->viewangles_vec;
	Vector_t velocity = pLocalPawn->m_vecVelocity();

	velocity.z = 0.f;

	float speed = velocity.Length2D();
	if (speed < 5.f)
	{
		pUserCmd->set_leftmove((pUserCmd->client_tick() % 2) ? 1.f : -1.f);
		return;
	}

	float ideal_angle = min(90.f, M_RAD2DEG(std::asin(30.f / speed)));
	float smooth = std::clamp(cfg_var::m_cfg->g_misc.strafesmootch / 100.f, 0.1f, 1.0f);

	ideal_angle *= smooth;

	float velocity_yaw = M_RAD2DEG(std::atan2(velocity.y, velocity.x));
	float delta = std::remainder(view_angels.y - velocity_yaw, 360.f);

	static bool dir_strafe = false;

	if (std::abs(delta) > ideal_angle * 1.5f)
		dir_strafe = !dir_strafe;

	if (dir_strafe)
	{
		pUserCmd->set_leftmove(1.f);
		view_angels.y -= ideal_angle;
	}
	else
	{
		pUserCmd->set_leftmove(-1.f);
		view_angels.y += ideal_angle;
	}

	float rotate = M_DEG2RAD(view_angels.y - velocity_yaw);

	float n_forward = -std::sin(rotate) * speed;
	float n_left = std::cos(rotate) * speed;

	pUserCmd->set_forwardmove(std::clamp(n_forward / 100.f, -1.f, 1.f));
	pUserCmd->set_leftmove(std::clamp(n_left / 100.f, -1.f, 1.f));
}

void g_misc_movement::AutoStrafeDir(CBaseUserCmdPB* pUserCmd, C_CSPlayerPawn* pLocalPawn)
{
	if (!cfg_var::m_cfg->g_misc.strafe || (pLocalPawn->GetFlags() & FL_ONGROUND))
		return;

	if (pLocalPawn->GetActualMoveType() == MOVETYPE_LADDER || pLocalPawn->GetActualMoveType() == MOVETYPE_NOCLIP)
		return;

	if (!pUserCmd || !pUserCmd->has_viewangles())
		return;

	const auto velocity = SDK::LocalPawn->m_vecVelocity();
	const auto speed2D = velocity.Length2D();

	if (speed2D < 2.f && !pUserCmd->has_forwardmove() && !pUserCmd->has_leftmove())
		return;

	const auto ideal_rotate = min(M_RAD2DEG(std::asinf(15.f / speed2D)), 90.f);
	const auto sign = (pUserCmd->client_tick() % 2) ? 1.f : -1.f;

	bool move_forward = SDK::Cmd->m_nButtons.m_nValue & IN_FORWARD;
	bool move_backward = SDK::Cmd->m_nButtons.m_nValue & IN_BACK;
	bool move_left = SDK::Cmd->m_nButtons.m_nValue & IN_MOVELEFT;
	bool move_right = SDK::Cmd->m_nButtons.m_nValue & IN_MOVERIGHT;

	pUserCmd->set_forwardmove(speed2D > 0.1f ? 0.f : 1.f);

	Vector_t move_angle = { pUserCmd->viewangles().x(), pUserCmd->viewangles().y(), pUserCmd->viewangles().z() };

	if (move_forward)
		move_angle.y += move_left ? 45.f : move_right ? -45.f : 0.f;
	else if (move_backward)
		move_angle.y += move_left ? 135.f : move_right ? -135.f : 180.f;
	else if (move_left || move_right)
		move_angle.y += move_left ? 90.f : -90.f;

	static float old_yaw = 0.f;

	float yaw_delta = std::remainder(move_angle.y - old_yaw, 360.f);
	float abs_yaw_delta = std::abs(yaw_delta);

	old_yaw = move_angle.y;

	pUserCmd->set_leftmove(yaw_delta > 0.f ? -1.f : 1.f);

	if (abs_yaw_delta <= ideal_rotate || abs_yaw_delta >= 30.f) 
	{
		const auto vel_ang = M_RAD2DEG(std::atan2(velocity.y, velocity.x));
		const auto vel_delta = std::remainder(move_angle.y - vel_ang, 360.f);

		float retrack_speed = ideal_rotate * ((cfg_var::m_cfg->g_misc.strafesmootch / 100.f) * 3);

		if (vel_delta <= retrack_speed || speed2D <= 15.f) 
		{
			if (-retrack_speed <= vel_delta || speed2D <= 15.f) 
			{
				move_angle.y += ideal_rotate * sign;
				pUserCmd->set_leftmove(sign);
			}
			else 
			{
				move_angle.y = vel_ang - retrack_speed;
				pUserCmd->set_leftmove(1.f);
			}
		}
		else 
		{
			move_angle.y = vel_ang + retrack_speed;
			pUserCmd->set_leftmove(-1.f);
		}
	}

	const float rotate = M_DEG2RAD(pUserCmd->viewangles().y() - move_angle.y);

	float n_forward = std::cos(rotate) * pUserCmd->forwardmove() - std::sin(rotate) * pUserCmd->leftmove();
	float n_left = std::sin(rotate) * pUserCmd->forwardmove() + std::cos(rotate) * pUserCmd->leftmove();

	pUserCmd->set_forwardmove(std::clamp(n_forward, -1.f, 1.f));
	pUserCmd->set_leftmove(std::clamp(n_left * -1.f, -1.f, 1.f));
}

void set_button_state(CUserCmd* pCmd, std::uint64_t button, int call_type)
{
	auto v7 = ~button;

	if (call_type == 1)
	{
		auto butt_mut = pCmd->m_csgoUserCmd.mutable_base()->buttons_pb().buttonstate1();
		auto butt = pCmd->m_nButtons.m_nValue;

		butt_mut = butt_mut & v7 | button;
		butt = butt & v7 | button;
	}
}

void g_misc_movement::ClampMovement(CUserCmd* pCmd, float speed)
{
	if (!SDK::LocalPawn && !cheat->alive)
		return;

	set_button_state(pCmd, IN_SPRINT, 1);

	auto f = pCmd->m_csgoUserCmd.mutable_base()->forwardmove() * 250.f;
	auto f2 = pCmd->m_csgoUserCmd.mutable_base()->leftmove() * 250.f;

	float calced_sqrtf = std::sqrtf((f * f) + (f2 * f2));

	if (calced_sqrtf > speed) 
	{
		float stops = calced_sqrtf / speed;

		pCmd->m_csgoUserCmd.mutable_base()->set_forwardmove(pCmd->m_csgoUserCmd.mutable_base()->forwardmove() / stops);
		pCmd->m_csgoUserCmd.mutable_base()->set_leftmove(pCmd->m_csgoUserCmd.mutable_base()->leftmove() / stops);
	}
}

void ClampMovement2(CUserCmd* pCmd, const float oldMovementLength)
{
	const auto curLength{ (pCmd->m_csgoUserCmd.mutable_base()->forwardmove() * pCmd->m_csgoUserCmd.mutable_base()->forwardmove() + pCmd->m_csgoUserCmd.mutable_base()->leftmove() * pCmd->m_csgoUserCmd.mutable_base()->leftmove()) };
	if (curLength)
	{
		float scale = oldMovementLength / curLength;
		if (scale != 1.f)
		{
			pCmd->m_csgoUserCmd.mutable_base()->set_forwardmove(pCmd->m_csgoUserCmd.mutable_base()->forwardmove() * scale);
			pCmd->m_csgoUserCmd.mutable_base()->set_leftmove(pCmd->m_csgoUserCmd.mutable_base()->leftmove() * scale);
		}
	}
}

void g_misc_movement::MovementFix(CUserCmd* pCmd, float angle)
{
	if (!pCmd->m_csgoUserCmd.mutable_base()->has_forwardmove() && !pCmd->m_csgoUserCmd.mutable_base()->has_leftmove())
		return;

	if (pCmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->y() == angle)
		return;

	const auto oldMovementLength{ pCmd->m_csgoUserCmd.mutable_base()->forwardmove() * pCmd->m_csgoUserCmd.mutable_base()->forwardmove() + pCmd->m_csgoUserCmd.mutable_base()->leftmove() * pCmd->m_csgoUserCmd.mutable_base()->leftmove() };
	angle = std::remainderf(angle, 360.f);

	float cur_angles{ pCmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->y() };
	pCmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->set_y(std::remainderf(cur_angles, 360.f));

	float rot{ std::fabs(cur_angles - angle) };
	if (angle <= cur_angles)
		rot = 360.f - std::fabs(angle - cur_angles);

	const auto forward_m{ pCmd->m_csgoUserCmd.mutable_base()->forwardmove() };
	const auto left_m{ pCmd->m_csgoUserCmd.mutable_base()->leftmove() };

	auto v57 = M_DEG2RAD(360.f - rot);
	auto v58 = M_DEG2RAD((360.f - rot) + 90.f);

	auto v59 = std::cos(v57) * forward_m;
	pCmd->m_csgoUserCmd.mutable_base()->set_forwardmove(v59 - (std::cos(v58) * left_m));

	auto v60 = std::sin(v58) * left_m;
	pCmd->m_csgoUserCmd.mutable_base()->set_leftmove(v60 - (std::sin(v57) * forward_m));

	ClampMovement2(pCmd, oldMovementLength);
}