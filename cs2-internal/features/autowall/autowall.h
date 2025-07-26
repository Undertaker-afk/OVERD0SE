#pragma once
#include <memory>
#include <array>

#include "../../utilities/common.h"
#include "../../utilities/draw.h"

#include "../../sdk/datatypes/vector.h"
#include "../../sdk/datatypes/transform.h"
#include "../../sdk/datatypes/qangle.h"

#include "../../core/config.h"

class C_BaseEntity;
class CCSWeaponBaseVData;
class Vector_t;
class C_CSPlayerPawn;
class CCSPlayerController;
class CBaseHandle;
class CEntityInstance;
class CUserCmd;
class CBaseUserCmdPB;
class QAngle_t;
class IGameEvent;

struct penetration_data_t {
    float m_damage{ };
    uint8_t m_hitgroup = -1;
    int m_hitbox = -1;
    bool m_penetrated{ true };
};

class c_auto_wall {
public:
    float get_hitgroup_damage_multiplier(C_CSPlayerPawn* player, float head_shot_mult, int hit_group);
    void scale_damage(int hit_group, C_CSPlayerPawn* player, CCSWeaponBaseVData* wpn_data, float& damage);
};

class c_fx_firebullet
{
public:
    bool run(C_CSPlayerPawn* entity, Vector_t& start, Vector_t& end, penetration_data_t& pen_data);
};

inline auto g_auto_wall = std::make_unique<c_auto_wall>();
inline auto g_fx_firebullet = std::make_unique<c_fx_firebullet>();