#include <vector>
#include <mutex>

#include "overlay.h"

#include "../../core/sdk.h"
#include "../../core/spoofcall/Invoker.h"

#include "../../sdk/entity.h"
#include "../../sdk/interfaces/iengineclient.h"
#include "../../sdk/interfaces/iglobalvars.h"
#include "../../sdk/interfaces/cgameentitysystem.h"
#include "../../sdk/interfaces/ccsgoinput.h"
#include "../../sdk/datatypes/utlvector.h"

#include "../../utilities/inputsystem.h"

#include "../../menu/menu.h"
#include "../../menu/widgets/widgets.h"

#include "../misc/misc.h"
#include "../rage/lagcomp.h"
#include "../rage/new_aim.hpp"
#include "../enginepred/pred.h"
#include "../configs/configs_var.h"
#include "../legit/legit_aim.h"

const char* weaponcur(C_CSWeaponBase* weapon)
{
	if (!weapon)
		return "";

	auto weapon_data = weapon->datawep();
	if (!weapon_data)
		return "";

	const char* szWeaponName = weapon_data->m_szName();
	const char* weaponPrefix = CXOR("weapon_");
	const char* weaponNameStart = strstr(szWeaponName, weaponPrefix);
	const char* extractedWeaponName = weaponNameStart ? weaponNameStart + strlen(weaponPrefix) : szWeaponName;

	return extractedWeaponName;
}

static ImDrawList* g_pBackgroundDrawList = nullptr;
static CCSPlayerController* pLocalController = nullptr;
static C_CSPlayerPawn* pLocal = nullptr;

void g_visuals_overlay::Render()
{
	widgets::render_log();

	g_legit_bot->drawfov();

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame())
		return;

	CCSPlayerController* pLocalController = CCSPlayerController::GetLocalPlayerController();
	if (!pLocalController)
		return;

	pLocal = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pLocalController->GetPawnHandle());
	if (!pLocal || !I::GlobalVars)
		return;

	RenderIndicators();

	const auto& screen_size = ImGui::GetIO().DisplaySize;
	const std::lock_guard<std::mutex> guard{ g_cachedEntitiesMutex };

	for (const auto& it : g_cachedEntities) 
	{
		if (!it.m_draw)
			continue;

		C_BaseEntity* pEntity = I::GameResourceService->pGameEntitySystem->Get(it.m_handle);
		if (pEntity == nullptr)
			continue;

		CBaseHandle hEntity = pEntity->GetRefEHandle();
		if (hEntity != it.m_handle) 
			continue;

		switch (it.m_type) {
		case CachedEntity_t::PLAYER_CONTROLLER:
			CCSPlayerController* pPlayer = I::GameResourceService->pGameEntitySystem->Get<CCSPlayerController>(hEntity);
			if (pPlayer == nullptr)
				break;

			render_esp(pPlayer, it.m_bbox);
			break;
		}
	}

	for (const auto& it : g_cachedEntities) 
	{
		C_BaseEntity* pEntity = I::GameResourceService->pGameEntitySystem->Get(it.m_handle);
		if (pEntity == nullptr)
			continue;

		CBaseHandle hEntity = pEntity->GetRefEHandle();
		if (hEntity != it.m_handle)
			continue;

		switch (it.m_type) {
		case CachedEntity_t::PLAYER_CONTROLLER:
			CCSPlayerController* pPlayer = I::GameResourceService->pGameEntitySystem->Get<CCSPlayerController>(hEntity);
			if (pPlayer == nullptr)
				break;

			RenderFOV(pPlayer->get_player_pawn(), pPlayer);
			break;
		}
	}
}

void g_visuals_overlay::RenderIndicators()
{
	const auto& screen_size = ImGui::GetIO().DisplaySize;
	const auto screen_center = screen_size / 2.f;

	auto draw_list = ImGui::GetForegroundDrawList();

	static auto x_size = 5;
	static auto pos_dmg = MATH::fnRandomInt(0, 2);

	const float time_delta = std::abs(I::GlobalVars->m_current_time - SDK::last_hurt_time);
	const float time_delta_shoot = std::abs(I::GlobalVars->m_current_time - SDK::shoot_time);

	static Vector_t vec_3d_local;
	static std::vector<Vector_t> m_vec_pos_local;

	if (SDK::bullets_es)
	{
		SDK::bullets_es = false;
		m_vec_pos_local.push_back(g_prediction->get_local_data()->m_eye_pos);
	}

	Vector_t m_vec_pos_hit = SDK::vec_scr;
	Vector_t vec_3d;

	//ImColor col = c::accent;

	MATH::world_to_screen(m_vec_pos_hit, vec_3d, screen_size);

	if (I::Engine->IsConnected() && I::Engine->IsInGame())
	{
		if (SDK::LocalPawn)
		{
			if (!SDK::LocalPawn->is_alive())
				return;

			if (SDK::LocalPawn->ActiveWeapon() && SDK::LocalPawn->ActiveWeapon()->datawep())
			{
				if (SDK::LocalPawn->ActiveWeapon()->datawep()->m_WeaponType() == WEAPONTYPE_SNIPER_RIFLE)
				{
					if (SDK::LocalPawn->ActiveWeapon()->m_zoomLevel() != 0)
					{
						ImColor colCustomScope1{ cfg_var::m_cfg->g_misc.colCustomScope1.colValue.GetU32() },
							colCustomScope2{ cfg_var::m_cfg->g_misc.colCustomScope2.colValue.GetU32() };

						const auto scope_thickness = cfg_var::m_cfg->g_misc.scope_thickness / 2 + 1 + cfg_var::m_cfg->g_misc.scope_gap;
						const auto arrow_thickness = cfg_var::m_cfg->g_misc.scope_thickness / 2.f;

						switch (cfg_var::m_cfg->g_misc.m_scopelocal)
						{
						case 1: // Default
							draw_list->AddRectFilled(ImVec2(screen_center.x, 0), ImVec2(screen_center.x + 1, screen_size.y), ImColor(0, 0, 0));
							draw_list->AddRectFilled(ImVec2(0, screen_center.y), ImVec2(screen_size.x, screen_center.y + 1), ImColor(0, 0, 0));
							break;
						case 2: // Custom
							draw_list->AddRectFilledMultiColor(
								{ screen_center.x - scope_thickness - cfg_var::m_cfg->g_misc.scope_size, screen_center.y - arrow_thickness },
								{ screen_center.x - scope_thickness, screen_center.y + arrow_thickness }, colCustomScope2, colCustomScope1, colCustomScope1, colCustomScope2);

							draw_list->AddRectFilledMultiColor(
								{ screen_center.x + scope_thickness + cfg_var::m_cfg->g_misc.scope_size, screen_center.y - arrow_thickness },
								{ screen_center.x + scope_thickness, screen_center.y + arrow_thickness }, colCustomScope2, colCustomScope1, colCustomScope1, colCustomScope2);

							draw_list->AddRectFilledMultiColor(
								{ screen_center.x - arrow_thickness, screen_center.y - cfg_var::m_cfg->g_misc.scope_size - scope_thickness },
								{ screen_center.x + arrow_thickness, screen_center.y - scope_thickness }, colCustomScope2, colCustomScope2, colCustomScope1, colCustomScope1);

							draw_list->AddRectFilledMultiColor(
								{ screen_center.x - arrow_thickness, screen_center.y + cfg_var::m_cfg->g_misc.scope_size + scope_thickness },
								{ screen_center.x + arrow_thickness, screen_center.y + scope_thickness }, colCustomScope2, colCustomScope2, colCustomScope1, colCustomScope1);
							break;
						default:
							break;
						}
					}
				}
			}

			//if (time_delta_shoot <= 3)
			//{
			//	for (auto pos : m_vec_pos_local)
			//	{
			//		if (MATH::world_to_screen(pos, vec_3d_local, screen_size))
			//			draw_list->AddLine(*reinterpret_cast<const ImVec2*>(&vec_3d_local),
			//				*reinterpret_cast<const ImVec2*>(&vec_3d), col, 0
			//			);
			//	}
			//}
			//else
			//	m_vec_pos_local.clear();

			//if (!pLocal)
			//	return;

			//if (pLocal->GetHealth() <= 0)
			//	return;

			//auto ActiveWeapon = pLocal->ActiveWeapon();
			//if (!ActiveWeapon)
			//	return;

			//auto data = ActiveWeapon->datawep();
			//if (!data)
			//	return;

			//if (!ActiveWeapon->IsWeapon())
			//	return;

			//const char* extractedWeaponName = weaponcur(ActiveWeapon);

			//bool has_awp = strcmp(extractedWeaponName, CXOR("awp")) == 0;
			//bool has_heavy_pistols = strcmp(extractedWeaponName, CXOR("revolver")) == 0 || strcmp(extractedWeaponName, CXOR("deagle")) == 0;
			//bool has_scout = strcmp(extractedWeaponName, CXOR("ssg08")) == 0;

			//int minimum_damage_over;
			//int minimum_damage_over_var;
			//int chan_over;
			//int chan_over_var;

			//if (has_awp) {
			//	minimum_damage_over = cfg_var::m_cfg->g_aim.weapon[6].is_mindmg_override;
			//	minimum_damage_over_var = cfg_var::m_cfg->g_aim.weapon[6].mindmg_override_val;
			//	chan_over = cfg_var::m_cfg->g_aim.weapon[6].is_hitchan_override;
			//	chan_over_var = cfg_var::m_cfg->g_aim.weapon[6].hitchan_override_val;
			//}
			//else if (has_scout) {
			//	minimum_damage_over = cfg_var::m_cfg->g_aim.weapon[5].is_mindmg_override;
			//	minimum_damage_over_var = cfg_var::m_cfg->g_aim.weapon[5].mindmg_override_val;
			//	chan_over = cfg_var::m_cfg->g_aim.weapon[5].is_hitchan_override;
			//	chan_over_var = cfg_var::m_cfg->g_aim.weapon[5].hitchan_override_val;
			//}
			//else if (has_heavy_pistols) {
			//	minimum_damage_over = cfg_var::m_cfg->g_aim.weapon[2].is_mindmg_override;
			//	minimum_damage_over_var = cfg_var::m_cfg->g_aim.weapon[2].mindmg_override_val;
			//	chan_over = cfg_var::m_cfg->g_aim.weapon[2].is_hitchan_override;
			//	chan_over_var = cfg_var::m_cfg->g_aim.weapon[2].hitchan_override_val;
			//}
			//else if (data->m_WeaponType() == WEAPONTYPE_PISTOL && !has_heavy_pistols) {
			//	minimum_damage_over = cfg_var::m_cfg->g_aim.weapon[1].is_mindmg_override;
			//	minimum_damage_over_var = cfg_var::m_cfg->g_aim.weapon[1].mindmg_override_val;
			//	chan_over = cfg_var::m_cfg->g_aim.weapon[1].is_hitchan_override;
			//	chan_over_var = cfg_var::m_cfg->g_aim.weapon[1].hitchan_override_val;
			//}
			//else if (data->m_WeaponType() == WEAPONTYPE_MACHINEGUN) {
			//	minimum_damage_over = cfg_var::m_cfg->g_aim.weapon[3].is_mindmg_override;
			//	minimum_damage_over_var = cfg_var::m_cfg->g_aim.weapon[3].mindmg_override_val;
			//	chan_over = cfg_var::m_cfg->g_aim.weapon[3].is_hitchan_override;
			//	chan_over_var = cfg_var::m_cfg->g_aim.weapon[3].hitchan_override_val;
			//}
			//else if (data->m_WeaponType() == WEAPONTYPE_RIFLE) {
			//	minimum_damage_over = cfg_var::m_cfg->g_aim.weapon[3].is_mindmg_override;
			//	minimum_damage_over_var = cfg_var::m_cfg->g_aim.weapon[3].mindmg_override_val;
			//	chan_over = cfg_var::m_cfg->g_aim.weapon[3].is_hitchan_override;
			//	chan_over_var = cfg_var::m_cfg->g_aim.weapon[3].hitchan_override_val;
			//}
			//else if (data->m_WeaponType() == WEAPONTYPE_SNIPER_RIFLE) {
			//	minimum_damage_over = cfg_var::m_cfg->g_aim.weapon[4].is_mindmg_override;
			//	minimum_damage_over_var = cfg_var::m_cfg->g_aim.weapon[4].mindmg_override_val;
			//	chan_over = cfg_var::m_cfg->g_aim.weapon[4].is_hitchan_override;
			//	chan_over_var = cfg_var::m_cfg->g_aim.weapon[4].hitchan_override_val;
			//}
			//else
			//{
			//	minimum_damage_over = 0;
			//	minimum_damage_over_var = 0;
			//	chan_over = 0;
			//	chan_over_var = 0;
			//}

			//auto dmg_val = CXOR("DMG: ") + std::to_string(minimum_damage_over_var);
			//auto chan_val = CXOR("CHAN: ") + std::to_string(chan_over_var);

			//if (SDK::LocalPawn->GetGameSceneNode()->GetSkeletonInstance()->GetModel().GetHitboxData()[hitbox_stomach].pos == Vector_t{ 0,0,0 })
			//	return;

			//Vector_t vec_3d_lp;
			//MATH::world_to_screen(SDK::LocalPawn->GetGameSceneNode()->GetSkeletonInstance()->GetModel().GetHitboxData()[hitbox_stomach].pos, vec_3d_lp, screen_size);

			//Vector_t indicator_pos{ vec_3d_lp.x + 150, vec_3d_lp.y, vec_3d_lp.z };
			//auto indicator_pos_2d = ImVec2(30.f, screen_center.y + screen_center.y / 3.f);

			//if (HIWORD(GetKeyState(minimum_damage_over)))
			//{
			//	draw_list->AddMultiColorText(FONT::indicators, 25, indicator_pos_2d, dmg_val.c_str(), col, col);
			//	indicator_pos_2d.y -= 30.f;
			//}

			//if (HIWORD(GetKeyState(chan_over)))
			//{
			//	draw_list->AddMultiColorText(FONT::indicators, 25, indicator_pos_2d, chan_val.c_str(), col, col);
			//	indicator_pos_2d.y -= 30.f;
			//}

			//if (LOWORD(GetKeyState(cfg_var::m_cfg->g_antiaim.left_anti)))
			//{
			//	draw_list->AddMultiColorText(FONT::indicators, 25, indicator_pos_2d, CXOR("MM LEFT"), col, col);
			//	indicator_pos_2d.y -= 30.f;
			//}

			//if (LOWORD(GetKeyState(cfg_var::m_cfg->g_antiaim.right_anti)))
			//{
			//	draw_list->AddMultiColorText(FONT::indicators, 25, indicator_pos_2d, CXOR("MM RIGHT"), col, col);
			//	indicator_pos_2d.y -= 30.f;
			//}
		}
	}
}

void g_visuals_overlay::CalculateBoundingBoxes()
{
	if (!I::Engine->IsConnected() || !I::Engine->IsInGame()) 
		return;

	if (!pLocal)
		return;

	if (!I::GameResourceService->pGameEntitySystem)
		return;

	const std::lock_guard<std::mutex> guard{ g_cachedEntitiesMutex };

	for (auto& it : g_cachedEntities)
	{
		C_BaseEntity* pEntity = I::GameResourceService->pGameEntitySystem->Get(it.m_handle);
		if (!pEntity)
			continue;

		CBaseHandle hEntity = pEntity->GetRefEHandle();
		if (hEntity != it.m_handle)
			continue;

		switch (it.m_type)
		{
		case CachedEntity_t::PLAYER_CONTROLLER:
			CCSPlayerController* pPlayer = I::GameResourceService->pGameEntitySystem->Get<CCSPlayerController>(hEntity);
			if (pPlayer == nullptr)
				break;

			C_CSPlayerPawn* pPlayerPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pPlayer->GetPawnHandle());
			if (pPlayerPawn == nullptr)
				break;

			it.m_draw = pPlayerPawn->CalculateBoundingBox(it.m_bbox, false);
			break;
		}
	}
}

void g_visuals_overlay::RenderFOV(C_CSPlayerPawn* player, CCSPlayerController* pPlayerController)
{
	if (!player || !SDK::LocalPawn || !cfg_var::m_cfg->g_visuals.enablevisarrows)
		return;

	if (!player->is_alive() || !SDK::LocalPawn->is_alive() || !cheat->alive || SDK::Cmd == nullptr)
		return;

	if (!SDK::LocalPawn->IsOtherEnemy(player))
		return;

	auto isOnScreen = [](Vector_t origin, Vector_t& screen) -> bool
		{
			const auto& screen_size = ImGui::GetIO().DisplaySize;
			if (!MATH::world_to_screen(origin, screen, screen_size))
				return false;

			static int iScreenWidth, iScreenHeight;
			iScreenWidth = screen_size.x;
			iScreenHeight = screen_size.y;

			auto xOk = iScreenWidth > screen.x;
			auto yOk = iScreenHeight > screen.y;

			return xOk && yOk;
		};

	const auto& screen_size = ImGui::GetIO().DisplaySize;
	auto local_data = g_prediction->get_local_data();
	auto draw_list = ImGui::GetForegroundDrawList();

	Vector_t screenPos;

	if (isOnScreen(player->GetGameSceneNode()->m_vecOrigin(), screenPos))
		return;

	Vector_t viewAngles{ I::Input->GetViewAngles().x, I::Input->GetViewAngles().y, I::Input->GetViewAngles().z };

	static int width, height;
	width = screen_size.x;
	height = screen_size.y;

	auto screenCenter = screen_size / 2.f;;
	auto angleYawRad = M_DEG2RAD(viewAngles.y - MATH::new_calculate_angle(SDK::LocalPawn->GetGameSceneNode()->m_vecOrigin(), player->GetGameSceneNode()->m_vecOrigin()).y - 90.0f);

	auto radius = 50;
	auto size = 10;

	auto newPointX = screenCenter.x + ((((width - (size * 3)) * 0.5f) * (radius / 100.0f)) * cos(angleYawRad)) + (int)(6.0f * (((float)size - 4.0f) / 16.0f));
	auto newPointY = screenCenter.y + ((((height - (size * 3)) * 0.5f) * (radius / 100.0f)) * sin(angleYawRad));

	std::array <ImVec2, 3> points
	{
		ImVec2(newPointX - size, newPointY - size),
			ImVec2(newPointX + size, newPointY),
			ImVec2(newPointX - size, newPointY + size)
	};

	MATH::rotate_triangle(points, viewAngles.y - MATH::new_calculate_angle(SDK::LocalPawn->GetGameSceneNode()->m_vecOrigin(), player->GetGameSceneNode()->m_vecOrigin()).y - 90.0f);
	draw_list->AddTriangleFilled(points.at(0), points.at(1), points.at(2), cfg_var::m_cfg->g_visuals.arrows_color.colValue.GetU32());
}

//namespace WeaponsIcons 
//{
//	std::map<std::string, const wchar_t*> gunIcons = {
//		{"weapon_p90", L"P"},
//		{"weapon_mp9", L"O"},
//		{"weapon_mp5sd", L"O"},
//		{"weapon_m4a4", L"M"},
//		{"weapon_knife", L"]"},
//		{"weapon_knife_ct", L"]"},
//		{"weapon_knife_t", L"]"},
//		{"weapon_deagle", L"A"},
//		{"weapon_elite", L"B"},
//		{"weapon_fiveseven", L"C"},
//		{"weapon_glock", L"D"},
//		{"weapon_revolver", L"J"},
//		{"weapon_hkp2000", L"E"},
//		{"weapon_p250", L"F"},
//		{"weapon_usp_silencer", L"G"},
//		{"weapon_tec9", L"H"},
//		{"weapon_cz75a", L"I"},
//		{"weapon_mac10", L"K"},
//		{"weapon_ump45", L"L"},
//		{"weapon_bizon", L"M"},
//		{"weapon_mp7", L"N"},
//		{"weapon_galilar", L"Q"},
//		{"weapon_famas", L"R"},
//		{"weapon_m4a1_silencer", L"T"},
//		{"weapon_m4a1", L"S"},
//		{"weapon_aug", L"U"},
//		{"weapon_sg556", L"V"},
//		{"weapon_ak47", L"W"},
//		{"weapon_g3sg1", L"X"},
//		{"weapon_scar20", L"Y"},
//		{"weapon_awp", L"Z"},
//		{"weapon_ssg08", L"a"},
//		{"weapon_xm1014", L"b"},
//		{"weapon_sawedoff", L"c"},
//		{"weapon_mag7", L"d"},
//		{"weapon_nova", L"e"},
//		{"weapon_negev", L"f"},
//		{"weapon_m249", L"g"},
//		{"weapon_taser", L"h"},
//		{"weapon_flashbang", L"i"},
//		{"weapon_hegrenade", L"j"},
//		{"weapon_smokegrenade", L"k"},
//		{"weapon_molotov", L"l"},
//		{"weapon_decoy", L"m"},
//		{"weapon_incgrenade", L"n"},
//		{"weapon_c4", L"o"},
//		{"weapon_bayonet", L"]"},
//		{"weapon_knife_survival_bowie", L"]"},
//		{"weapon_knife_butterfly", L"]"},
//		{"weapon_knife_canis", L"]"},
//		{"weapon_knife_cord", L"]"},
//		{"weapon_knife_css", L"]"},
//		{"weapon_knife_falchion", L"]"},
//		{"weapon_knife_flip", L"]"},
//		{"weapon_knife_gut", L"]"},
//		{"weapon_knife_karambit", L"]"},
//		{"weapon_knife_twinblade", L"]"},
//		{"weapon_knife_kukri", L"]"},
//		{"weapon_knife_m9_bayonet", L"]"},
//		{"weapon_knife_outdoor", L"]"},
//		{"weapon_knife_push", L"]"},
//		{"weapon_knife_skeleton", L"]"},
//		{"weapon_knife_stiletto", L"]"},
//		{"weapon_knife_tactical", L"]"},
//		{"weapon_knife_widowmaker", L"]"},
//		{"weapon_knife_ursus", L"]"}
//	};
//
//	wchar_t get(std::string designerName) {
//		try {
//			return *gunIcons.at(designerName);
//		}
//		catch (std::out_of_range& const e) {
//			return NULL;
//		}
//	}
//};

void g_visuals_overlay::CalculateSkeleton(CCSPlayerController* pPlayerController, C_CSPlayerPawn* player, const ImVec4& out)
{
	if (!pLocal)
		return;

	auto game_scene_node = player->GetGameSceneNode();
	if (!game_scene_node)
		return;

	auto skeleton = game_scene_node->GetSkeletonInstance();
	if (!skeleton)
		return;

	auto model_state = &skeleton->GetModel();

	CStrongHandle<CModel> model = model_state->m_hModel();

	auto model_skelet = &model->m_modelSkeleton();

	if (!model_skelet)
		return;

	skeleton->calc_world_space_bones(0, bone_flags::FLAG_HITBOX);

	auto bones = model_state->GetHitboxData();

	for (uint32_t i = 0; i < model->GetHitboxesNum(); i++)
	{
		if (!(model->GetHitboxFlags(i) & bone_flags::FLAG_HITBOX))
			continue;

		auto parent_index = model->GetHitboxParent(i);
		if (parent_index == -1)
			continue;

		ImVec2 start_scr, end_scr;
		if (!D::WorldToScreen(bones[i].pos, start_scr) || !D::WorldToScreen(bones[parent_index].pos, end_scr))
			continue;

		if (cfg_var::m_cfg->g_visuals.skelet_sets.outlined)
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(start_scr.x, start_scr.y), ImVec2(end_scr.x, end_scr.y), ImColor(0, 0, 0, 255), 2.f);

		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(start_scr.x, start_scr.y), ImVec2(end_scr.x, end_scr.y), cfg_var::m_cfg->g_visuals.skelet_sets.col.colValue.GetU32(), 1.f);
	}
}

bool g_visuals_overlay::IsValid(CCSPlayerController* player)
{
	if (!player)
		return false;

	C_CSPlayerPawn* pPlayerPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(player->GetPawnHandle());
	if (!pPlayerPawn)
		return false;

	float mAlpha[65];
	auto idx = player->GetIdentity()->GetEntryIndex();

	if (pPlayerPawn->GetHealth() <= 0) 
	{
		static constexpr auto ALPHA_FREQUENCY = 255 / 70.f;
		mAlpha[idx] -= ALPHA_FREQUENCY * I::GlobalVars->m_frame_time;
	}
	else 
	{
		if (player->GetGameSceneNode()->IsDormant()) 
		{
			if (mAlpha[idx] < 0.6f) {
				mAlpha[idx] -= (1.0f / 1.0f) * I::GlobalVars->m_frame_time;
				mAlpha[idx] = std::clamp(mAlpha[idx], 0.f, 0.6f);
			}
			else
				mAlpha[idx] -= (1.0f / 20.f) * I::GlobalVars->m_frame_time;
		}
		else 
		{
			mAlpha[idx] += (1.0f / 0.2f) * I::GlobalVars->m_frame_time;
			mAlpha[idx] = std::clamp(mAlpha[idx], 0.f, 1.f);
		}
	}

	return (mAlpha[idx] > 0.f);
}

void g_visuals_overlay::render_esp(CCSPlayerController* player, const ImVec4& color)
{
	if (!pLocal || !player || !cfg_var::m_cfg->g_visuals.enablevis)
		return;

	C_CSPlayerPawn* pPlayerPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(player->GetPawnHandle());
	if (!pPlayerPawn || pPlayerPawn->GetHealth() <= 0 || !player->GetIdentity())
		return;

	bool bIsEnemy = pLocal->IsOtherEnemy(pPlayerPawn);
	if (!bIsEnemy /*&& !cfg_var::m_cfg->g_visuals.showteammates*/)
		return;

	ImVec4 bbox;
	if (!pPlayerPawn->CalculateBoundingBox(bbox, false))
		return;

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	ImVec2 min = ImVec2(bbox.x, bbox.y);
	ImVec2 max = ImVec2(bbox.z, bbox.w);

	ImVec2 box_size = ImVec2(max.x - min.x, max.y - min.y);
	ImVec2 center = ImVec2((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f);

	// boxes
	if (cfg_var::m_cfg->g_visuals.box)
	{
	    float outline_size = cfg_var::m_cfg->g_visuals.box_sets.thickness * 0.3f + 1.0f;

		// black line
		if (cfg_var::m_cfg->g_visuals.box_sets.outlined)
			drawList->AddRect(
				min - ImVec2(outline_size, outline_size), max + ImVec2(outline_size, outline_size),
				ImColor(0, 0, 0, 255), cfg_var::m_cfg->g_visuals.box_sets.rounding + outline_size * 0.3f, 0, outline_size
			);

		// original
		drawList->AddRect(min, max,
			cfg_var::m_cfg->g_visuals.box_sets.col.colValue.GetU32(),
			cfg_var::m_cfg->g_visuals.box_sets.rounding, 0, cfg_var::m_cfg->g_visuals.box_sets.thickness
		);
	}

	// name
	if (cfg_var::m_cfg->g_visuals.name && player->GetPlayerName())
	{
		const char* p_name = player->GetPlayerName();

		ImVec2 p_size = FONT::pEspName->CalcTextSizeA(FONT::pEspName->FontSize, FLT_MAX, 0.0f, p_name);
		ImVec2 p_text = ImVec2(center.x - p_size.x * 0.5f, min.y - p_size.y - 5);

		// add black text effect (8 of)
		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				if (x == 0 && y == 0)
					continue;

				drawList->AddText(FONT::pEspName, FONT::pEspName->FontSize, ImVec2(p_text.x + x, p_text.y + y),
					ImColor(0, 0, 0, cfg_var::m_cfg->g_visuals.name_sets.col.colValue.a / 2), p_name);
			}
		}

		// original
		drawList->AddText(FONT::pEspName, FONT::pEspName->FontSize, p_text, cfg_var::m_cfg->g_visuals.name_sets.col.colValue.GetU32(), p_name);
	}

	// health bar
	if (cfg_var::m_cfg->g_visuals.hbar)
	{
		float hb_val = pPlayerPawn->GetHealth();

		ImVec2 hb_pos = ImVec2(min.x - 5.f, min.y);

		// black fill
		drawList->AddRectFilled(hb_pos, ImVec2(hb_pos.x - cfg_var::m_cfg->g_visuals.hbar_sets.thickness, max.y), ImColor(0, 0, 0, 140));

		// original
		drawList->AddRectFilled(
			ImVec2(hb_pos.x, hb_pos.y + box_size.y - box_size.y * hb_val / 100.f),
			ImVec2(hb_pos.x - cfg_var::m_cfg->g_visuals.hbar_sets.thickness, hb_pos.y + box_size.y),
			cfg_var::m_cfg->g_visuals.hbar_sets.col.colValue.GetU32()
		);

		// black line
		if (cfg_var::m_cfg->g_visuals.hbar_sets.outlined)
			drawList->AddRect(hb_pos, ImVec2(hb_pos.x - cfg_var::m_cfg->g_visuals.hbar_sets.thickness, max.y), ImColor(0, 0, 0, 255));
	}

	// weapon info
	CPlayer_WeaponServices* weapon_service = pPlayerPawn->GetWeaponServices();
	if (weapon_service)
	{
		auto active_weapon = I::GameResourceService->pGameEntitySystem->Get<C_CSWeaponBase>(weapon_service->m_hActiveWeapon());
		if (active_weapon)
		{
			auto data = active_weapon->datawep();
			if (data && strcmp(data->m_szName(), CXOR("NULL")) != 0)
			{
				const char* w_name = data->m_szName();

				if (const char* p_pos = strstr(w_name, CXOR("weapon_")))
					w_name = p_pos + 7;

				float weapon_offseet = 0.f;

				// ammo bar
				if (cfg_var::m_cfg->g_visuals.abar)
				{
					auto v_data = active_weapon->GetVData();
					if (v_data && v_data->m_iMaxClip1() > 0)
					{
						float a_stage = static_cast<float>(active_weapon->clip1()) / v_data->m_iMaxClip1();

						ImVec2 ab_min = ImVec2(min.x, max.y + 5.f);
						ImVec2 ab_max = ImVec2(max.x, max.y + 6.f);

						// black fill
						drawList->AddRectFilled(ImVec2(ab_min.x, ab_min.y), ImVec2(ab_max.x, ab_max.y + cfg_var::m_cfg->g_visuals.abar_sets.thickness), ImColor(30, 30, 30, 150));

						// original
						drawList->AddRectFilled(ab_min, ImVec2(min.x + box_size.x * a_stage, ab_max.y + cfg_var::m_cfg->g_visuals.abar_sets.thickness),
							cfg_var::m_cfg->g_visuals.abar_sets.col.colValue.GetU32());

						// black line
						if (cfg_var::m_cfg->g_visuals.abar_sets.outlined)
							drawList->AddRect(ImVec2(ab_min.x - 1.f, ab_min.y - 1.f), ImVec2(ab_max.x + 1.f, ab_max.y + cfg_var::m_cfg->g_visuals.abar_sets.thickness), ImColor(0, 0, 0, 255));

						// calculate
						weapon_offseet = 5.f + cfg_var::m_cfg->g_visuals.abar_sets.thickness;
					}
				}

				// weapon text
				if (cfg_var::m_cfg->g_visuals.textweap)
				{
					ImVec2 p_size = FONT::pEspName->CalcTextSizeA(FONT::pEspName->FontSize, FLT_MAX, 0.0f, w_name);
					ImVec2 p_text = ImVec2(center.x - p_size.x * 0.5f, max.y + weapon_offseet);

					// add black text effect (8 of)
					for (int x = -1; x <= 1; x++)
					{
						for (int y = -1; y <= 1; y++)
						{
							if (x == 0 && y == 0)
								continue;

							drawList->AddText(FONT::pEspWepName, FONT::pEspWepName->FontSize, ImVec2(p_text.x + x, p_text.y + y),
								ImColor(0, 0, 0, cfg_var::m_cfg->g_visuals.weap_sets.col.colValue.a / 2), w_name);
						}
					}

					// original
					drawList->AddText(FONT::pEspWepName, FONT::pEspWepName->FontSize, p_text, cfg_var::m_cfg->g_visuals.weap_sets.col.colValue.GetU32(), w_name);
				}
			}
		}
	}

	// skeleton
	if (cfg_var::m_cfg->g_visuals.skeleton)
		CalculateSkeleton(player, pPlayerPawn, color);

	// flags
	std::map<std::string, Color_t> p_flags;

	if (cfg_var::m_cfg->g_visuals.Flags[0] && pPlayerPawn->GetArmorValue() > 0)
		p_flags.insert({ player->m_bPawnHasHelmet() ? CXOR("HK") : CXOR("K"), Color_t(255, 255, 255, 255) });

	if (cfg_var::m_cfg->g_visuals.Flags[1] && player->m_bPawnHasDefuser())
		p_flags.insert({ CXOR("Defuser"), Color_t(255, 255, 255, 255) });

	if (cfg_var::m_cfg->g_visuals.Flags[2] && pPlayerPawn->IsScoped())
		p_flags.insert({ CXOR("Scoped"), Color_t(65, 230, 255, 255) });

	if (cfg_var::m_cfg->g_visuals.Flags[3] && pPlayerPawn->IsDefusing())
		p_flags.insert({ CXOR("Defusing."), Color_t(255, 146, 44, 255) });

	if (cfg_var::m_cfg->g_visuals.Flags[4] && pPlayerPawn->IsGrabbingHostage())
		p_flags.insert({ CXOR("GBB Hostage."), Color_t(44, 132, 255, 255) });

	if (!p_flags.empty())
	{
		ImVec2 p_text = ImVec2(max.x + 4.f, min.y - 4.f);

		for (const auto& flag : p_flags)
		{
			// add black text effect (8 of)
			for (int x = -1; x <= 1; x++)
			{
				for (int y = -1; y <= 1; y++)
				{
					if (x == 0 && y == 0)
						continue;

					drawList->AddText(FONT::pEspFlagsName, FONT::pEspFlagsName->FontSize, ImVec2(p_text.x + x, p_text.y + y), ImColor(0, 0, 0, flag.second.a / 2), flag.first.c_str());
				}
			}

			// original
			drawList->AddText(FONT::pEspFlagsName, FONT::pEspFlagsName->FontSize, p_text, flag.second.GetU32(), flag.first.c_str());

			p_text.y += FONT::pEspFlagsName->FontSize + 1.f;
		}
	}
}