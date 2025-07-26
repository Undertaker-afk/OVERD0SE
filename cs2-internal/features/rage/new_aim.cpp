#include "new_aim.hpp"

#include <iostream>
#include <memoryapi.h>
#include <mutex>
#include <array>
#include <format>

#include "../misc/movement.h"

#include "../../utilities/inputsystem.h"

#include "../../core/config.h"

#include "../../features/enginepred/pred.h"
#include "../../features/configs/configs_var.h"

float get_max_speed()
{
    constexpr float max = 285.0f;

    if (!SDK::LocalPawn || !cheat->alive)
        return max;

    const auto move = SDK::LocalPawn->m_pMovementServices();
    if (!move)
        return max;

    float max_speed = move->m_flMaxspeed();

    const auto activeWeapon = SDK::LocalPawn->ActiveWeapon();
    if (activeWeapon && activeWeapon->datawep())
    {
        const bool scoped = SDK::LocalPawn->IsScoped();
        const auto seeds = activeWeapon->datawep()->m_flMaxSpeed();

        max_speed = scoped ? seeds[1] : seeds[0];
    }

    return max_speed;
}

bool c_rage_bot::can_shoot(C_CSPlayerPawn* pawn, C_CSWeaponBase* activeWeapon)
{
    if (!activeWeapon || !SDK::LocalController)
        return false;

    // Проверка наличия патронов
    if (activeWeapon->clip1() <= 0)
        return false;

    // Проверка времени следующей атаки
    if (activeWeapon->m_nNextPrimaryAttackTick() > SDK::LocalController->m_nTickBase())
        return false;


    const bool ammo = activeWeapon->clip1() > 0;
    const bool fire = activeWeapon->m_nNextPrimaryAttackTick() <= SDK::LocalController->m_nTickBase();

    return ammo && fire;
}

void c_rage_bot::store_records()
{
    if (!I::Engine->IsInGame() || !I::Engine->IsConnected() || !SDK::LocalPawn)
        return;

    static auto sv_maxunlag = I::Cvar->Find(FNV1A::HashConst(CXOR("sv_maxunlag")));
    const int max_ticks = TIME_TO_TICKS(sv_maxunlag->get_float());

    const std::lock_guard<std::mutex> guard{ g_cachedEntitiesMutex };

    for (const auto& cached_entity : g_cachedEntities)
    {
        C_BaseEntity* entity = I::GameResourceService->pGameEntitySystem->Get(cached_entity.m_handle);
        if (!entity || cached_entity.m_handle != entity->GetRefEHandle())
            continue;

        if (cached_entity.m_type != CachedEntity_t::PLAYER_CONTROLLER)
            continue;

        CCSPlayerController* player_controller = I::GameResourceService->pGameEntitySystem->Get<CCSPlayerController>(cached_entity.m_handle);

        if (!player_controller || player_controller == SDK::LocalController)
            continue;

        const auto entry_index = cached_entity.m_handle.GetEntryIndex();
        auto& player_records = m_lag_records[entry_index];

        if (!player_controller->IsPawnAlive()) {
            m_lag_records.erase(entry_index);
            continue;
        }

        C_CSPlayerPawn* player_pawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(player_controller->GetPawnHandle());

        if (!player_pawn) {
            m_lag_records.erase(entry_index);
            continue;
        }

        if (player_pawn->GetTeam() == SDK::LocalPawn->GetTeam() || player_pawn == SDK::LocalPawn)
            continue;

        if (player_records.size() != max_ticks)
        {
            player_records.clear();
            player_records.resize(1);
        }

        player_records.emplace_front(player_pawn);

        if (player_records.size() > max_ticks)
            player_records.pop_back();
    }
}
//float get_delta_time(int tick_entity)
//{
//    int tick_count = I::GlobalVars->m_tick_count;
//    static float maxunlag_val = I::Cvar->Find(FNV1A::Hash("sv_maxunlag"))->value.fl;
//
//    float mod_value = fmodf(maxunlag_val, 0.015625);
//    float unlag_difference = maxunlag_val - mod_value;
//    float mod_ticks = mod_value * 64.0;
//    int unlag_ticks = (int)(float)((float)(unlag_difference * 64.0) + 0.5);
//
//    data_info info{};
//    info.tick = tick_entity;
//    info.time = tick_entity * 0.015625f;
//
//    int v20{};
//    float v19{}, v21{};
//
//    if (mod_ticks < (float)(1.0 - 0.0099999998))
//    {
//        v20 = tick_count - unlag_ticks;
//        if (mod_ticks > 0.0099999998)
//        {
//            v19 = -mod_ticks;
//            v21 = v19;
//            if (v19 < 0.0f)
//                v19 = v19 + 1.0f;
//        }
//        else
//        {
//            v19 = 0.0f;
//            v21 = 0.0f;
//        }
//    }
//    else // high ping 100+
//    {
//        v19 = 0.0f;
//        v20 = tick_count - (unlag_ticks + 1);
//        v21 = 0.0f;
//    }
//
//    auto v22 = v20 - 1;
//
//    data_info v40{};
//
//    v40.time = v19;
//    if (v21 >= 0.0)
//        v22 = v20;
//    v40.tick = v22;
//    if (v19 < (float)(1.0 - 0.0099999998))
//    {
//        if (v19 <= 0.0099999998)
//        {
//            v40.time = 0.0;
//        }
//    }
//    else
//    {
//        v40.tick = v22 + 1;
//        v40.time = 0.0;
//    }
//
//    auto tick = v40.tick;
//    auto v24 = (float)info.tick;
//    auto v38 = v40;
//    int new_tick{};
//    data_info v39{};
//
//    if ((float)((float)(v24 + info.time) * 0.015625) <= 0.0)
//    {
//        auto cmd_tick_count = tick_entity;
//        v40.tick = cmd_tick_count;
//        v40.time = 0.0;
//        new_tick = v40.tick;
//        tick = v38.tick;
//        v39 = v40;
//    }
//    else
//    {
//        new_tick = info.tick;
//        v39 = info;
//    }
//
//    int v32{};
//    float v30{};
//    int v33;
//    int v31;
//
//    if (tick <= new_tick && (tick < new_tick || v39.time > v38.time))
//    {
//        v33 = new_tick - tick;
//        v30 = v39.time - v38.time;
//        if ((float)(v39.time - v38.time) < 0.0)
//            v30 = v30 + 1.0;
//        v32 = v33 - 1;
//        if ((float)(v39.time - v38.time) >= 0.0)
//            v32 = v33;
//        if (v30 >= (float)(1.0 - 0.0099999998))
//        {
//            ++v32;
//            goto LABEL_48;
//        }
//    LABEL_47:
//        if (v30 > 0.0099999998)
//            goto LABEL_49;
//        goto LABEL_48;
//    }
//    v30 = v38.time - v39.time;
//    v31 = tick - new_tick;
//    if ((float)(v38.time - v39.time) < 0.0)
//        v30 = v30 + 1.0;
//    v32 = v31 - 1;
//    if ((float)(v38.time - v39.time) >= 0.0)
//        v32 = v31;
//    if (v30 < (float)(1.0 - 0.0099999998))
//        goto LABEL_47;
//    ++v32;
//LABEL_48:
//    v30 = 0.0;
//LABEL_49:
//    auto delta_time = (float)((float)v32 + v30) * 0.015625;
//
//    return delta_time;
//}

int HitboxToHitgroup(int hitbox)
{
    if (hitbox == hitbox_head)
        return e_hitgroups::hitgroup_head;
    else if (hitbox == hitbox_stomach || hitbox == hitbox_pelvis)
        return e_hitgroups::hitgroup_stomach;
    else if (hitbox >= hitbox_chest && hitbox <= hitbox_upper_chest)
        return e_hitgroups::hitgroup_chest;
    else if (hitbox == hitbox_left_upper_arm || hitbox == hitbox_left_forearm || hitbox == hitbox_left_hand)
        return e_hitgroups::hitgroup_leftarm;
    else if (hitbox == hitbox_right_forearm || hitbox == hitbox_right_upper_arm || hitbox == hitbox_right_hand)
        return e_hitgroups::hitgroup_rightarm;
    else if (hitbox == hitbox_neck)
        return e_hitgroups::hitgroup_neck;
    else if (hitbox == hitbox_left_foot || hitbox == hitbox_left_calf || hitbox == hitbox_left_thigh)
        return e_hitgroups::hitgroup_leftleg;
    else if (hitbox == hitbox_right_foot || hitbox == hitbox_right_calf || hitbox == hitbox_right_thigh)
        return e_hitgroups::hitgroup_rightleg;
    else
        return e_hitgroups::hitgroup_generic;
}

void c_rage_bot::store_hitboxes(C_CSPlayerPawn* pawn)
{
    auto active_weapon = SDK::LocalPawn->ActiveWeapon();
    if (!active_weapon)
        return;

    auto weapon_data = active_weapon->datawep();
    if (!weapon_data)
        return;

    for (int i{}; i < hitbox_max; i++)
    {
        m_hitboxes.emplace_back((e_hitboxes)i);
    }

    for (auto it = m_hitboxes.begin(); it != m_hitboxes.end();)
    {
        int hitbox = *it;

        if (!rage_data.hitbox_head && hitbox == hitbox_head)
        {
            it = m_hitboxes.erase(it);
            continue;
        }

        if (!rage_data.hitbox_stomach)
        {
            if (hitbox == hitbox_pelvis || hitbox == hitbox_stomach)
            {
                it = m_hitboxes.erase(it);
                continue;
            }
        }

        if (!rage_data.hitbox_chest)
        {
            if (hitbox == hitbox_chest || hitbox == hitbox_lower_chest || hitbox == hitbox_upper_chest)
            {
                it = m_hitboxes.erase(it);
                continue;
            }
        }

        if (!rage_data.hitbox_arms)
        {
            if (hitbox == hitbox_right_hand || hitbox == hitbox_left_hand || hitbox == hitbox_right_upper_arm || hitbox == hitbox_right_forearm
                || hitbox == hitbox_left_upper_arm || hitbox == hitbox_left_forearm)
            {
                it = m_hitboxes.erase(it);
                continue;
            }
        }

        if (!rage_data.hitbox_legs)
        {
            if (hitbox == hitbox_left_thigh || hitbox == hitbox_left_calf || hitbox == hitbox_right_thigh || hitbox == hitbox_right_calf) {
                it = m_hitboxes.erase(it);
                continue;
            }
        }

        if (!rage_data.hitbox_feets)
        {
            if (hitbox == hitbox_right_foot || hitbox == hitbox_left_foot)
            {
                it = m_hitboxes.erase(it);
                continue;
            }
        }

        it++;
    }
}

const char* get_extracted_weapon_name(C_CSWeaponBase* weapon)
{
    if (!weapon)
        return "";

    auto weapon_data = weapon->datawep();
    if (!weapon_data)
        return "";

    const char* szWeaponName = weapon_data->m_szName();
    const char* weaponPrefix = ("weapon_");
    const char* weaponNameStart = strstr(szWeaponName, weaponPrefix);
    const char* extractedWeaponName = weaponNameStart ? weaponNameStart + strlen(weaponPrefix) : szWeaponName;

    return extractedWeaponName;
}

void c_rage_bot::setup_weapon(C_CSPlayerPawn* pLocal) 
{
    if (!cfg_var::m_cfg->g_aim.enablerage || !pLocal || pLocal->GetHealth() <= 0)
        return;

    auto ActiveWeapon = pLocal->ActiveWeapon();
    if (!ActiveWeapon || !ActiveWeapon->IsWeapon())
        return;

    auto data = ActiveWeapon->datawep();
    if (!data)
        return;

    rage_data.rage_enable = cfg_var::m_cfg->g_aim.enablerage;

    const char* name = get_extracted_weapon_name(ActiveWeapon);
    int cfg_ind = -1;

    if (strcmp(name, CXOR("awp")) == 0)
        cfg_ind = 6;
    else if (strcmp(name, CXOR("ssg08")) == 0)
        cfg_ind = 5;
    else if (strcmp(name, CXOR("revolver")) == 0 || strcmp(name, CXOR("deagle")) == 0)
        cfg_ind = 2;
    else {
        switch (data->m_WeaponType()) {
        case WEAPONTYPE_PISTOL: cfg_ind = 1; break;
        case WEAPONTYPE_MACHINEGUN:
        case WEAPONTYPE_RIFLE: cfg_ind = 3; break;
        case WEAPONTYPE_SNIPER_RIFLE: cfg_ind = 4; break;
        }
    }

    if (cfg_ind == -1)
        return;

    const auto& cfg = cfg_var::m_cfg->g_aim.weapon[cfg_ind];

    rage_data.minimum_damage = cfg.mindmg;
    rage_data.minimum_damage_over = cfg.is_mindmg_override;
    rage_data.minimum_damage_over_var = cfg.mindmg_override_val;
    rage_data.hitchan_over = cfg.is_hitchan_override;
    rage_data.hitchan_over_var = cfg.hitchan_override_val;
    rage_data.rage_hitchance = cfg.chance;
    rage_data.auto_stop = cfg.autostop;
    rage_data.rage_target_select = cfg.rage_target_select;
    rage_data.auto_scope = cfg.autoscope;
    rage_data.point_scale = cfg.point_scale;

    rage_data.hitbox_head = cfg.m_head;
    rage_data.hitbox_chest = cfg.m_chest;
    rage_data.hitbox_stomach = cfg.m_stomach;
    rage_data.hitbox_arms = cfg.m_arms;
    rage_data.hitbox_legs = cfg.m_legs;
    rage_data.hitbox_feets = cfg.m_foot;

    rage_data.point_head = cfg.p_head;
    rage_data.point_chest = cfg.p_chest;
    rage_data.point_stomach = cfg.p_stomach;
    rage_data.point_arms = cfg.p_arms;
    rage_data.point_legs = cfg.p_legs;
    rage_data.point_feets = cfg.p_foot;
}

lag_record_t* c_rage_bot::select_last_record(int handle) 
{
    if (!SDK::LocalPawn->is_alive())
        return nullptr;

    auto& records = m_lag_records[handle];

    if (records.empty())
        return nullptr;

    lag_record_t* record = nullptr;

    auto iterator = std::find_if(records.begin(), records.end(), [&](lag_record_t& record)
        {
            if (record.m_pawn && record.is_valid())
                return true;

            return false;
        });

    if (iterator != records.end())
        record = &*iterator;

    return record;
}

lag_record_t* c_rage_bot::select_record(int handle) 
{
    if (!SDK::LocalPawn || !SDK::LocalPawn->is_alive())
        return nullptr;

    auto& records = m_lag_records[handle];
    if (records.empty())
        return nullptr;

    const auto last_record = select_last_record(handle);
    if (records.size() == 1 || records.size() == rage_data.rage_target_select)
        return last_record;

    if (m_hitboxes.empty())
        return nullptr;

    lag_record_t* best_record = nullptr;

    for (auto it = records.rbegin(); it != records.rend(); ++it) {
        auto* current_record = &*it;

        if (!current_record || !current_record->is_valid() || !current_record->m_pawn)
            continue;

        if (!best_record) {
            best_record = current_record;
            continue;
        }

        if (current_record->m_throwing) {
            best_record = current_record;
            continue;
        }

        const bool current_on_ground = (current_record->m_pawn->GetFlags() & FL_ONGROUND);
        const bool best_on_ground = (best_record->m_pawn->GetFlags() & FL_ONGROUND);

        if (current_on_ground != best_on_ground && current_on_ground)
            best_record = current_record;
    }

    if (!last_record || !last_record->is_valid())
        return best_record;

    if (!best_record)
        return nullptr;

    const auto compare_damage = [this](lag_record_t* record) {
        record->apply(record->m_pawn);
        const auto points = this->select_points(record, m_hitboxes, true);
        record->reset(record->m_pawn);
        return points.m_damage;
        };

    const int last_record_damage = compare_damage(last_record);
    const int best_record_damage = compare_damage(best_record);

    return (last_record_damage > best_record_damage) ? last_record : best_record;
}

void c_rage_bot::remove_target(int entry_index)
{
    if (auto it = m_aim_targets.find(entry_index); it != m_aim_targets.end())
        m_aim_targets.erase(it);
}

void c_rage_bot::find_target() 
{
    if (!SDK::LocalPawn || !SDK::LocalPawn->is_alive()) {
        m_best_target = nullptr;
        return;
    }

    m_aim_targets.clear();

    const std::lock_guard<std::mutex> guard{ g_cachedEntitiesMutex };

    const auto local_team = SDK::LocalPawn->GetTeam();
    const auto local_controller = SDK::LocalController;
    const auto game_entity_system = I::GameResourceService->pGameEntitySystem;

    for (const auto& cached_entity : g_cachedEntities) {
        C_BaseEntity* entity = game_entity_system->Get(cached_entity.m_handle);
        if (!entity)
            continue;

        const auto entry_index = entity->GetRefEHandle().GetEntryIndex();
        CCSPlayerController* player_controller = game_entity_system->Get<CCSPlayerController>(entry_index);

        if (!player_controller || player_controller == local_controller) {
            remove_target(entry_index);
            continue;
        }

        if (!player_controller->IsPawnAlive()) {
            remove_target(entry_index);
            continue;
        }

        C_CSPlayerPawn* player_pawn = game_entity_system->Get<C_CSPlayerPawn>(player_controller->GetPawnHandle());
        if (!player_pawn) {
            remove_target(entry_index);
            continue;
        }

        if (player_pawn->GetTeam() == local_team || player_pawn == SDK::LocalPawn) {
            remove_target(entry_index);
            continue;
        }

        if (!m_aim_targets.contains(entry_index))
            m_aim_targets.emplace(entry_index, aim_target_t(player_pawn, entry_index));
    }

    if (m_aim_targets.empty()) {
        m_best_target = nullptr;
        return;
    }

    aim_target_t* best_target = this->get_nearest_target();
    if (!best_target || !best_target->m_pawn || !best_target->m_pawn->is_alive()) {
        m_best_target = nullptr;
        return;
    }

    lag_record_t* best_record = this->select_record(best_target->m_handle);
    if (!best_record) {
        m_best_target = nullptr;
        return;
    }

    best_target->store(best_record);
    m_best_target = best_target;
}

bool c_rage_bot::automatic_stop(C_CSPlayerPawn* pLocal, C_CSWeaponBase* pWeapon, CUserCmd* pCmd)
{
    if (!shouldstop || !rage_data.auto_stop || !pLocal || !pWeapon)
        return false;

    if (!(pLocal->GetFlags() & FL_ONGROUND))
        return false;

    const auto pWeaponData = pWeapon->datawep();
    if (!pWeaponData)
        return false;

    const float flMaxSpeed = get_max_speed() * 0.2f;
    const float flCurrentSpeed = pLocal->speed_move();
    const float flAbsSpeed = std::fabsf(flCurrentSpeed);

    if (flAbsSpeed <= flMaxSpeed)
    {
        g_misc_movement::ClampMovement(pCmd, flMaxSpeed);
        return true;
    }

    static auto pCvarFriction = I::Cvar->Find(FNV1A::HashConst(CXOR("sv_friction")));
    const float flSurfaceFriction = pLocal->m_pMovementServices()->m_flSurfaceFriction();

    const float flNewSpeed = flAbsSpeed - ((flAbsSpeed * 0.17f) * pCvarFriction->get_float()) * flSurfaceFriction;
    const float flMaxAccelSpeed = fabsf((0.17f * flSurfaceFriction) * (get_max_speed() * pCvarFriction->get_float()));
    const float flAccelSpeed = fabsf(fminf(
        (0.17f * flSurfaceFriction) * ((flAbsSpeed * 0.8f) * pCvarFriction->get_float()),
        (flAbsSpeed * 0.8f) - flNewSpeed
    ));

    const float flSpeedFactor = (flMaxAccelSpeed / flAccelSpeed) * 0.015625f;

    Vector_t vecVelocityAngle;
    MATH::vector_angles_gomo(pLocal->m_vecVelocity(), vecVelocityAngle);

    vecVelocityAngle.y = cheat->viewangles.y - vecVelocityAngle.y;

    Vector_t vecDirection;
    MATH::angle_vectors(vecVelocityAngle, vecDirection);

    const float flVelocityLength = pLocal->m_vecVelocity().Length2D();
    const Vector_t vecStop = vecDirection * -flVelocityLength;

    if (flVelocityLength > 13.0f) {
        pCmd->m_csgoUserCmd.mutable_base()->set_forwardmove(vecStop.x * flSpeedFactor);
        pCmd->m_csgoUserCmd.mutable_base()->set_leftmove(vecStop.y * flSpeedFactor);
    }
    else {
        pCmd->m_csgoUserCmd.mutable_base()->set_forwardmove(0.0f);
        pCmd->m_csgoUserCmd.mutable_base()->set_leftmove(0.0f);
    }

    return true;
}

aim_target_t* c_rage_bot::get_nearest_target() 
{
    if (!SDK::LocalPawn->is_alive())
        return nullptr;

    aim_target_t* best_target{};

    auto local_data = g_prediction->get_local_data();

    if (!local_data)
        return nullptr;

    auto active = SDK::LocalPawn->ActiveWeapon();

    if (!active)
        return nullptr;

    auto data = SDK::LocalPawn->ActiveWeapon()->datawep();

    if (!data)
        return nullptr;

    Vector_t shoot_pos = local_data->m_eye_pos;

    float best_distance = FLT_MAX;

    for (auto it = m_aim_targets.begin(); it != m_aim_targets.end(); it = std::next(it))
    {
        aim_target_t* target = &it->second;

        if (!target->m_pawn
            || !target->m_pawn->is_alive()
            || !target->m_pawn->GetGameSceneNode()
            || !target->m_pawn->GetGameSceneNode()->GetSkeletonInstance()
            || !target->m_pawn->GetGameSceneNode()->GetSkeletonInstance()->GetModel().GetHitboxData()
            || target->m_pawn->m_bGunGameImmunity()) {

            continue;
        }

        auto minimum_damage = get_minimum_damage(target->m_pawn->GetHealth());
        auto best_damage = 0;

        for (auto& hitbox : m_hitboxes) 
        {
            penetration_data_t pen_data{};
            pen_data.m_hitgroup = HitboxToHitgroup(hitbox);

            if (!g_fx_firebullet->run(target->m_pawn, shoot_pos, target->m_pawn->GetGameSceneNode()->GetSkeletonInstance()->GetModel().GetHitboxData()[hitbox].pos, pen_data))
                continue;

            if (pen_data.m_damage > 0)
            {
                best_target = target;
                best_damage = pen_data.m_damage;
            }
        }
    }

    if (!best_target)
        return nullptr;

    return best_target;
}

constexpr double M_PI2 = 3.14159265358979323846;

void vector_transform(Vector_t& in, Matrix3x4_t& matrix, Vector_t& out) {
    out = {
        in.Dot({ matrix[0][0], matrix[0][1], matrix[0][2] }) + matrix[0][3],
        in.Dot({ matrix[1][0], matrix[1][1], matrix[1][2] }) + matrix[1][3],
        in.Dot({ matrix[2][0], matrix[2][1], matrix[2][2] }) + matrix[2][3]
    };
}

Vector_t c_rage_bot::transform_point(Matrix3x4_t matrix, Vector_t point)
{
    Vector_t result{ };
    vector_transform(point, matrix, result);
    return result;
}

std::vector<Vector_t> c_rage_bot::calculate_sphere_points(float radius, int num_points) 
{
    std::vector<Vector_t> points;
    points.reserve(num_points);

    const float phi = M_PI2 * (3.0f - std::sqrt(5.0f));

    for (int i = 0; i < num_points; ++i) {
        const float y = 1 - (i / static_cast<float>(num_points - 1)) * 2;
        const float radius_at_y = std::sqrt(1 - y * y);
        const float theta = phi * i;

        points.emplace_back(
            std::cos(theta) * radius_at_y * radius,
            y * radius,
            std::sin(theta) * radius_at_y * radius
        );
    }

    return points;
}

std::vector<Vector_t> c_rage_bot::calculate_points(int num_points, float radius, float height, Matrix3x4_t& matrix, hitbox_data_t& hitbox) 
{
    std::vector<Vector_t> points;
    points.reserve(num_points);

    const float angle_step = 2.f * M_PI2 / num_points;

    for (int i = 0; i < num_points; ++i) {
        const float theta = angle_step * i;
        Vector_t in_point = {
            hitbox.m_mins.x + hitbox.m_radius * height,
            radius * std::cos(theta),
            radius * std::sin(theta)
        };

        Vector_t vec_point{};
        vector_transform(in_point, matrix, vec_point);
        points.push_back(vec_point);
    }

    return points;
}

bool c_rage_bot::multi_points(lag_record_t* record, int hitbox, std::vector<aim_point_t>& points, bool center_only) 
{
    if (!record || !record->m_pawn || !record->m_pawn->is_alive())
        return false;

    const auto pawn = record->m_pawn;
    const auto local_data = g_prediction->get_local_data();
    if (!local_data)
        return false;

    hitbox_data_t hitbox_data = get_hitbox_data(pawn, hitbox);
    if (hitbox_data.m_invalid_data || this->get_hitbox_from_menu(hitbox) == -1)
        return false;

    Matrix3x4_t matrix = transform_to_matrix(record->m_bone_data[hitbox_data.m_num_bone]);

    Vector_t center = transform_point(matrix, (hitbox_data.m_mins + hitbox_data.m_maxs) * 0.5f);
    points.emplace_back(center, hitbox, true);

    if (center_only || hitbox_data.m_radius <= 0.f)
        return true;

    const float radius = hitbox_data.m_radius * (rage_data.point_scale / 150.f);

    if (hitbox == hitbox_head) 
    {
        const auto sphere_points = calculate_sphere_points(radius / 1.1f, 8);

        for (const auto& offset : sphere_points)
            points.emplace_back(transform_point(matrix, hitbox_data.m_maxs + offset), hitbox);
    }
    else 
    {
        const auto capsule_points = calculate_points(4, radius, 0.5f, matrix, hitbox_data);

        for (const auto& point : capsule_points)
            points.emplace_back(point, hitbox);
    }

    return !points.empty();
}

int c_rage_bot::get_minimum_damage(int health)
{
    int bullets = SDK::LocalPawn->ActiveWeapon()->clip1();
    auto minimum_damage = (rage_data.minimum_damage_over && HIWORD(GetKeyState(rage_data.minimum_damage_over)) ? rage_data.minimum_damage_over_var : rage_data.minimum_damage);

    if (bullets == 1) {
        minimum_damage = health + 1;
        return minimum_damage;
    }

    if (minimum_damage > 100)
        minimum_damage = health + minimum_damage - 100;
    else if (minimum_damage > health)
        minimum_damage = health + 1;

    return minimum_damage;
}

aim_point_t c_rage_bot::select_points(lag_record_t* record, const std::vector<e_hitboxes>& hitboxes, bool center_only) 
{
    if (!SDK::LocalPawn || !SDK::LocalPawn->is_alive() || !record || !record->m_pawn || !record->m_pawn->is_alive())
        return { Vector_t(0.f, 0.f, 0.f), -1 };

    const auto weapon_data = SDK::LocalPawn->ActiveWeapon()->datawep();
    if (!weapon_data)
        return { Vector_t(0.f, 0.f, 0.f), -1 };

    const auto local_data = g_prediction->get_local_data();
    if (!local_data)
        return { Vector_t(0.f, 0.f, 0.f), -1 };

    Vector_t shoot_pos = local_data->m_eye_pos;
    const int minimum_damage = get_minimum_damage(record->m_pawn->GetHealth());

    aim_point_t best_point{ Vector_t(0.f, 0.f, 0.f), -1 };
    float best_damage = 0.f;

    std::vector<aim_point_t> points;
    points.reserve(center_only ? hitboxes.size() : hitboxes.size() * 5);

    for (const auto hitbox : hitboxes)
    {
        if (hitbox < 0 || hitbox >= e_hitboxes::hitbox_max)
            continue;

        const auto& hitbox_data = record->m_pawn->GetGameSceneNode()->GetSkeletonInstance()->GetModel().GetHitboxData()[hitbox];
        points.clear();

        if (!multi_points(record, hitbox, points, center_only))
            continue;

        for (auto& point : points) {
            penetration_data_t pen_data;

            pen_data.m_hitgroup = static_cast<uint8_t>(HitboxToHitgroup(hitbox));
            pen_data.m_damage = 0.f;

            if (!g_fx_firebullet->run(record->m_pawn, shoot_pos, point.m_point, pen_data) || pen_data.m_damage < 1)
                continue;

            point.m_damage = pen_data.m_damage;

            if (point.m_damage >= record->m_pawn->GetHealth()) {
                SDK::points_for_print.push_back(point.m_point);
                return point;
            }

            if (point.m_damage >= minimum_damage && point.m_damage > best_damage) {
                best_point = point;
                best_damage = point.m_damage;
            }

            SDK::points_for_print.push_back(point.m_point);
        }
    }

    return best_point;
}

void c_rage_bot::scan_target() 
{
    if (!SDK::LocalPawn || !SDK::LocalPawn->is_alive() || !m_best_target || !m_best_target->m_lag_record || m_hitboxes.empty())
    {
        m_best_target = nullptr;
        return;
    }

    auto* lag_record = m_best_target->m_lag_record;
    auto* target_pawn = m_best_target->m_pawn;

    int min_damage = get_minimum_damage(target_pawn->GetHealth());

    if (SDK::LocalPawn->ActiveWeapon()->datawep()->m_WeaponType() == WEAPONTYPE_TASER)
        min_damage = target_pawn->GetHealth();

    lag_record->apply(target_pawn);
    aim_point_t best_point = select_points(lag_record, m_hitboxes);
    lag_record->reset(target_pawn);

    if (best_point.m_hitbox == -1 || best_point.m_damage < min_damage)
        m_best_target = nullptr;
    else
        m_best_target->m_best_point = best_point;
}

void c_rage_bot::calculate_max_accuracy() 
{
    auto* local_player = SDK::LocalPawn;
    if (!local_player || !local_player->is_alive())
        return;

    auto* active_weapon = local_player->ActiveWeapon();
    if (!active_weapon)
        return;

    auto* weapon_data = active_weapon->datawep();
    if (!weapon_data ||
        weapon_data->m_WeaponType() == WEAPONTYPE_KNIFE ||
        weapon_data->m_WeaponType() == WEAPONTYPE_GRENADE ||
        weapon_data->m_WeaponType() == WEAPONTYPE_C4)
        return;

    auto local_data = g_prediction->get_local_data();
    if (!local_data)
        return;

    bool is_scoped = local_player->IsScoped();

    if (local_player->m_pMovementServices() && local_player->m_pMovementServices()->m_bDucking()) {
        m_accuracy_data.m_min_inaccuracy = is_scoped ?
            weapon_data->m_flInaccuracyCrouch()[0] :
            weapon_data->m_flInaccuracyCrouch()[1];
    }
    else {
        m_accuracy_data.m_min_inaccuracy = is_scoped ?
            weapon_data->m_flInaccuracyStand()[0] :
            weapon_data->m_flInaccuracyStand()[1];
    }

    m_accuracy_data.m_calculate_accuracy = weapon_is_at_max_accuracy(weapon_data);

    if (m_accuracy_data.m_calculate_accuracy)
        m_accuracy_data.m_lowest_accuracy = (local_data->m_inaccuracy + local_data->m_spread * 500.f);
}

bool c_rage_bot::weapon_is_at_max_accuracy(CCSWeaponBaseVData* weapon_data) 
{
    if (!SDK::LocalPawn || !g_prediction->get_local_data())
        return false;

    short item_definition = SDK::LocalPawn->get_active_item_definition();
    if (item_definition == -1)
        return false;

    auto local_data = g_prediction->get_local_data();
    float current_inaccuracy = local_data->m_inaccuracy;

    if ((item_definition == WEAPON_SSG08 || item_definition == WEAPON_REVOLVER) && !(SDK::LocalPawn->GetFlags() & FL_ONGROUND) && current_inaccuracy <= 0.009f) 
        return true;

    if (!(weapon_data->m_WeaponType() == WEAPONTYPE_SNIPER_RIFLE || item_definition == WEAPON_AUG || item_definition == WEAPON_SG553) && SDK::LocalPawn->IsScoped())
        return false;

    return (current_inaccuracy - m_accuracy_data.m_min_inaccuracy) <= 0.0001f;
}

void angle_vectors(Vector_t angles, Vector_t& forward, Vector_t& right, Vector_t& up)
{
    float			angle;
    static float	sr, sp, sy, cr, cp, cy;

    angle = angles.y * ((3.14159265358979323846 * 2) / 360);
    sy = sin(angle);
    cy = cos(angle);

    angle = angles.x * ((3.14159265358979323846 * 2) / 360);
    sp = sin(angle);
    cp = cos(angle);

    angle = angles.z * ((3.14159265358979323846 * 2) / 360);
    sr = sin(angle);
    cr = cos(angle);

    forward.x = cp * cy;
    forward.y = cp * sy;
    forward.z = -sp;

    right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
    right.y = (-1 * sr * sp * sy + -1 * cr * cy);
    right.z = -1 * sr * cp;

    up.x = (cr * sp * cy + -sr * -sy);
    up.y = (cr * sp * sy + -sr * cy);
    up.z = cr * cp;
}

bool c_rage_bot::hitchance(Vector_t& pos, bone_data* matrix, Vector_t angles, C_CSPlayerPawn* player, int index)
{
    if (!SDK::LocalPawn || !player)
        return false;

    auto* weapon = SDK::LocalPawn->ActiveWeapon();
    if (!weapon || !weapon->datawep())
        return false;

    auto* scenenode = player->GetGameSceneNode();
    if (!scenenode || !scenenode->GetSkeletonInstance())
        return false;

    auto model = scenenode->GetSkeletonInstance()->GetModel().m_hModel();
    if (!model.is_valid() || !model->get_hitbox(index))
        return false;

    auto* local_data = g_prediction->get_local_data();
    if (!local_data)
        return false;

    weapon->update_accuracy_penality();
    if (weapon_is_at_max_accuracy(weapon->datawep()))
        return true;

    float hitchance_percent = (rage_data.hitchan_over && HIWORD(GetKeyState(rage_data.hitchan_over)))
        ? rage_data.hitchan_over_var
        : rage_data.rage_hitchance;

    float dd_hitchance = hitchance_percent / 100.f;
    if (dd_hitchance >= 1.f)
        return true;

    Vector_t fwd, right, up;
    angle_vectors(angles, fwd, right, up);

    c_trace_filter filter(0x1C3003, SDK::LocalPawn, nullptr, 4);
    Vector_t shoot_pos = local_data->m_eye_pos;

    float range = weapon->datawep()->m_flRange();
    float accuracy = local_data->m_inaccuracy;

    int total_hits = 0;
    int total_seeds = static_cast<int>(precomputed_seeds.size());
    int required_hits = static_cast<int>(dd_hitchance * total_seeds);

    for (size_t i = 0; i < precomputed_seeds.size(); i++)
    {
        auto& seed = precomputed_seeds[i];

        Vector2D_t spread = {
            std::get<2>(seed) * std::get<0>(seed) * accuracy,
            std::get<1>(seed) * std::get<0>(seed) * accuracy
        };

        Vector_t dir = fwd + (right * spread.x) + (up * spread.y);
        dir.normalizess();

        Vector_t end = shoot_pos + (dir * range);

        game_trace_t trace = {};
        ray_t ray = {};
        I::Trace->clip_ray_to_entity2(&ray, shoot_pos, end, player, &filter, &trace);

        if (trace.HitEntity == player)
            total_hits++;

        const int remaining_seeds = total_seeds - static_cast<int>(i) - 1;
        if ((total_hits + remaining_seeds) < required_hits)
            return false;

        if (total_hits >= required_hits)
            return true;
    }

    return (static_cast<float>(total_hits) / total_seeds) >= dd_hitchance;
}

void c_rage_bot::build_seed_table()
{
    if (!precomputed_seeds.empty())
        return;

    precomputed_seeds.clear();
    precomputed_seeds.reserve(256);

    for (auto i = 0; i < 256; i++)
    {
        I::m_random_seed(i + 1);
        const float rand1 = I::m_random_float(0.f, 1.f);
        const float rand2 = I::m_random_float(0.f, 2.f * M_PI);

        precomputed_seeds.emplace_back(rand1, sin(rand2), cos(rand2));
    }
}

void c_rage_bot::process_backtrack(lag_record_t* record) 
{
    if (!record)
        return;

    const int tick_shoot = TIME_TO_TICKS(record->m_simulation_time);

    for (int i = 0; i < SDK::Cmd->m_csgoUserCmd.input_history_size(); i++) 
    {
        if (SDK::Cmd->m_csgoUserCmd.attack1_start_history_index() == -1)
            continue;

        auto input_history = SDK::Cmd->m_csgoUserCmd.mutable_input_history(i);
        if (!input_history)
            continue;

        if (input_history->has_cl_interp()) 
        {
            auto interp_info = input_history->mutable_cl_interp();
            interp_info->set_frac(0.f);
        }

        if (input_history->has_sv_interp0()) 
        {
            auto interp_info = input_history->mutable_sv_interp0();
            interp_info->set_src_tick(tick_shoot);
            interp_info->set_dst_tick(tick_shoot);
            interp_info->set_frac(0.f);
        }

        if (input_history->has_sv_interp1()) 
        {
            auto interp_info = input_history->mutable_sv_interp1();
            interp_info->set_src_tick(tick_shoot);
            interp_info->set_dst_tick(tick_shoot);
            interp_info->set_frac(0.f);
        }

        input_history->set_render_tick_count(tick_shoot);

        SDK::Cmd->m_csgoUserCmd.mutable_base()->set_client_tick(tick_shoot);

        input_history->set_player_tick_count(SDK::LocalController->m_nTickBase());
        input_history->set_player_tick_fraction(g_prediction->get_local_data()->m_player_tick_fraction);
    }
}

void c_rage_bot::process_attack(CUserCmd* user_cmd, Vector_t angle) 
{
    for (int i = 0; i < SDK::Cmd->m_csgoUserCmd.input_history_size(); i++) 
    {
        auto container = SDK::Cmd->m_csgoUserCmd.mutable_input_history(i);

        if (container)
            container->set_player_tick_count(g_prediction->get_local_data()->m_shoot_tick);
    }

    user_cmd->m_csgoUserCmd.set_attack3_start_history_index(0);
    user_cmd->m_csgoUserCmd.set_attack1_start_history_index(0);

    process_backtrack(m_best_target->m_lag_record);

    if (cfg_var::m_cfg->g_aim.auto_fire)
        user_cmd->m_nButtons.m_nValue |= IN_ATTACK;

    if (!(user_cmd->m_nButtons.m_nValue & IN_ATTACK))
        return;

    for (int i = 0; i < user_cmd->m_csgoUserCmd.input_history_size(); i++)
    {
        auto tick = user_cmd->m_csgoUserCmd.mutable_input_history(i);

        if (tick) 
        {
            tick->mutable_view_angles()->set_x(angle.x);
            tick->mutable_view_angles()->set_y(angle.y);
            tick->mutable_view_angles()->set_z(angle.z);
        }
    }

    if (!cfg_var::m_cfg->g_antiaim.hideshots)
    {
        user_cmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->set_x(angle.x);
        user_cmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->set_y(angle.y);
    }
    else
    {
        user_cmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->set_x(179.f);
        user_cmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->set_z(179.f);
    }

    shouldstop = false;
}

Vector_t aim_direction(Vector_t src, Vector_t dst)
{
    Vector_t result;
    Vector_t delta = dst - src;

    MATH::new_vector_angles(delta, result);
    return result;
}

void c_rage_bot::on_create_move() 
{
    if (!cfg_var::m_cfg->g_aim.enablerage || !I::Engine->IsInGame())
        return;

    m_hitboxes.clear();

    if (m_best_target)
        m_best_target->reset();

    SDK::points_for_print.clear();
    shouldstop = false;

    auto* user_cmd = SDK::Cmd;
    if (!user_cmd)
        return;

    auto* local_player = SDK::LocalPawn;
    auto* local_data = g_prediction->get_local_data();
    if (!local_player || !local_player->is_alive() || !local_data)
        return;

    auto* active_weapon = local_player->ActiveWeapon();
    if (!active_weapon)
        return;

    auto* weapon_data = active_weapon->datawep();
    if (!weapon_data || weapon_data->m_WeaponType() == WEAPONTYPE_KNIFE ||
        weapon_data->m_WeaponType() == WEAPONTYPE_GRENADE || weapon_data->m_WeaponType() == WEAPONTYPE_C4)
        return;

    auto max_speed = get_max_speed();
    local_data->m_eye_pos += local_data->m_velocity * 0.015625f * 300;

    setup_weapon(local_player);
    store_hitboxes(local_player);

    if (m_hitboxes.empty())
        return;

    find_target();
    scan_target();

    if (!m_best_target || !m_best_target->m_best_point.m_valid || !can_shoot(local_player, active_weapon))
        return;

    shouldstop = true;

    bool no_spread = I::Cvar->Find(FNV1A::HashConst(CXOR("weapon_accuracy_nospread")))->get_bool();
    if (weapon_data->m_WeaponType() == WEAPONTYPE_SNIPER_RIFLE && !(user_cmd->m_nButtons.m_nValue & IN_ZOOM) && !local_player->IsScoped() && active_weapon->m_zoomLevel() == 0 && (local_player->GetFlags() & FL_ONGROUND) && !no_spread && rage_data.auto_scope)
        user_cmd->m_nButtons.m_nValue |= IN_SECOND_ATTACK;

    m_best_target->m_lag_record->apply(m_best_target->m_pawn);

    Vector_t angle = aim_direction(local_data->m_eye_pos, m_best_target->m_best_point.m_point);
    Vector_t best_angle = angle - get_removed_aim_punch_angle(local_player);

    if (hitchance(angle, m_best_target->m_pawn->GetGameSceneNode()->GetSkeletonInstance()->GetModel().GetHitboxData(), angle, m_best_target->m_pawn, m_best_target->m_best_point.m_hitbox))
        process_attack(user_cmd, best_angle);

    m_best_target->m_lag_record->reset(m_best_target->m_pawn);
}