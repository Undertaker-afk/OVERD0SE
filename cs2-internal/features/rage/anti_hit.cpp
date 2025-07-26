#include "anti_hit.hpp"

#include "../../features/configs/configs_var.h"
#include "../../features/misc/movement.h"

#include <random>

static std::mt19937 random_int(std::random_device{}());

float get_fov(const Vector_t& view_angle, const Vector_t& aim_angle)
{
    Vector_t delta = aim_angle - view_angle;

    MATH::normalize_angles(delta);

    return min(sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f)), 180.0f);
}

void anti_hit::create_move(CUserCmd* m_pcmd)
{
    if (!cfg_var::m_cfg->g_aim.enablerage)
        return;

    C_BaseCSGrenade* pGrenade = reinterpret_cast<C_BaseCSGrenade*>(SDK::LocalPawn->ActiveWeapon());
    if (pGrenade == nullptr)
        return;

    if ((SDK::LocalPawn->GetActualMoveType() == MOVETYPE_LADDER) || (SDK::LocalPawn->GetActualMoveType() == MOVETYPE_NOCLIP))
        return;

    if (SDK::freeze_time)
        return;

    CCSPlayerController* target = nullptr;
    auto best_fov = FLT_MAX;
    const std::lock_guard<std::mutex> guard{ g_cachedEntitiesMutex };

    for (const auto& it : g_cachedEntities) {
        C_BaseEntity* pEntity = I::GameResourceService->pGameEntitySystem->Get(it.m_handle);
        if (pEntity == nullptr)
            continue;

        CBaseHandle hEntity = pEntity->GetRefEHandle();
        if (hEntity != it.m_handle)
            continue;

        switch (it.m_type)
        {
        case CachedEntity_t::PLAYER_CONTROLLER: {
            CCSPlayerController* pPlayer = I::GameResourceService->pGameEntitySystem->Get<CCSPlayerController>(hEntity);
            if (pPlayer == nullptr)
                continue;

            C_CSPlayerPawn* player = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pPlayer->GetPawnHandle());
            if (!player)
                continue;

            if (player->GetHealth() <= 0 || !SDK::LocalPawn->IsOtherEnemy(player) || player->GetGameSceneNode()->IsDormant() || player->m_bGunGameImmunity())
                continue;

            Vector_t angles;
            angles.x = m_pcmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->x();
            angles.y = m_pcmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->y();

            auto fov = get_fov(angles, MATH::new_calculate_angle(SDK::LocalPawn->GetEyePosition(), pPlayer->GetPawnOrigin()));

            if (fov < best_fov) {
                best_fov = fov;
                target = pPlayer;
            }

            break;
        }
        }
    }

    if (!target)
        return;

    if ( SDK::Cmd->m_nButtons.m_nValue & IN_USE || SDK::LocalPawn->ActiveWeapon()->datawep()->m_WeaponType() == WEAPONTYPE_GRENADE)
        return;

    if (cfg_var::m_cfg->g_antiaim.baseyaw > 0 || cfg_var::m_cfg->g_antiaim.pitchtype > 0 && 
        !pGrenade->GetThrowTime() > 0 && SDK::LocalPawn->GetEflags() != FL_FROZEN )
    {
        m_pcmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->set_x(get_pitch(m_pcmd));
        m_pcmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->set_y(get_yaw(m_pcmd));
    }
}

float anti_hit::get_pitch(CUserCmd* m_pcmd)
{
    static std::uniform_int_distribution<int> pitch(-89, 89);

    const auto& cfg = cfg_var::m_cfg->g_antiaim;

    switch (cfg.pitchtype)
    {
    case 1: // Down
        return 89.f;
        break;
    case 2: // Up
        return -89.f;
        break;
    case 3: // Zero
        return 0.f;
        break;
    case 4: // Random
        return static_cast<float>(pitch(random_int));
        break;
    case 5: // Custom
        return cfg.custompitch;
        break;
    default:
        break;
    }
}

float anti_hit::get_yaw(CUserCmd* m_pcmd)
{
    static std::uniform_int_distribution<int> yaw_random(-180, 180);

    const auto& cfg = cfg_var::m_cfg->g_antiaim;

    float yaw = m_pcmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->y();

    // modes
    if (cfg.baseyaw == 1)
        yaw -= 180.f; // Backwards
    else if (cfg.baseyaw == 2) // Random
        yaw += static_cast<float>(yaw_random(random_int));
    else if (cfg.baseyaw == 3) // Custom
        yaw += cfg.customyaw;

    // manuals
    if (cfg.left_anti && (LOWORD(GetKeyState(cfg.left_anti)))) // left
        yaw += 90.f;
    else if (cfg.right_anti && (LOWORD(GetKeyState(cfg.right_anti)))) // right
        yaw -= 90.f;
    else if (cfg.backward_anti && (LOWORD(GetKeyState(cfg.backward_anti)))) // backward
        yaw -= 180.f;

    jitter_side = -jitter_side;

    return yaw;
}