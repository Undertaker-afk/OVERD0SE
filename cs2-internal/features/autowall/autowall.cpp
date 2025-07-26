#include "autowall.h"

#include "../../core/sdk.h"
#include "../../core/convars.h"

#include "../../sdk/entity.h"
#include "../../sdk/interfaces/iengineclient.h"
#include "../../sdk/interfaces/iglobalvars.h"
#include "../../sdk/interfaces/cgameentitysystem.h"
#include "../../sdk/datatypes/qangle.h"
#include "../../sdk/datatypes/vector.h"
#include "../../sdk/entity_handle.h"
#include "../../sdk/interfaces/ienginecvar.h"
#include "../../sdk/datatypes/usercmd.h"
#include "../../sdk/interfaces/itrace.h"

#include <mutex>
#include <array>

unsigned int get_handle_entity(C_CSPlayerPawn* entity)
{
	if (!entity)
		return -1;

	using function_t = unsigned int(CS_FASTCALL*)(C_CSPlayerPawn*);
	static function_t fn = reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, CXOR("48 85 C9 74 32 48 8B 41 10 48 85 C0 74 29 44")));

	return fn(entity);
}

int get_colision_entity(C_CSPlayerPawn* entity)
{
	if (!entity)
		return -1;

	using function_t = unsigned int(CS_FASTCALL*)(C_CSPlayerPawn*);
	static function_t fn = reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, CXOR("40 53 48 83 EC 20 48 8B D9 48 85 C9 74 24")));

	return fn(entity);
}

c_trace_filter::c_trace_filter(std::uint64_t trace_mask, C_CSPlayerPawn* local_player, C_CSPlayerPawn* entity, std::uint8_t layer)
{
	static auto ptr_s = reinterpret_cast<std::uintptr_t*>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, CXOR("48 8D 05 ?? ?? ?? ?? 89 5C 24 54")), 0x3));
	this->ptr = ptr_s;
	this->trace_mask = trace_mask;

	if (local_player)
	{
		this->skip_handle1 = get_handle_entity(local_player);
		this->skip_owner_handle1 = get_handle_entity(local_player);
		this->collision1 = get_colision_entity(local_player);
	}
	else
	{
		this->skip_handle1 = -1;
		this->collision1 = -1;
		this->skip_owner_handle1 = -1;
	}

	if (entity)
	{

		this->skip_handle1 = get_handle_entity(entity);
		this->skip_owner_handle1 = get_handle_entity(entity);
		this->collision1 = get_colision_entity(entity);
	}
	else
	{
		this->skip_handle2 = -1;
		this->collision2 = -1;
		this->skip_owner_handle2 = -1;
	}

	this->N0000011C = 7;
	this->layer = layer;
	this->flags = 0x49;

	this->null_it3 = 0;
	this->unk1 = 0;
	this->unk2 = 0;
}

void init_trace_data(c_trace_data* trace_data, Vector_t& start, Vector_t& end, c_trace_filter& filter, int& pen_count)
{
	using function_t = void(CS_FASTCALL*)(c_trace_data*, Vector_t, Vector_t, c_trace_filter, int);
	static function_t fn = *reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 56 41 57 48 83 EC 40 F2")));
	
	CS_ASSERT(fn != nullptr);

	return fn(trace_data, start, end, filter, pen_count);
}

void get_trace(c_trace_data* trace_data, game_trace_t* trace, c_trace_info* trace_info)
{
	using function_t = void(CS_FASTCALL*)(c_trace_data*, game_trace_t*, c_trace_info*);
	static function_t fn = *reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, CXOR("41 0F B7 40 ?? F3 41 0F 10 10")));

	CS_ASSERT(fn != nullptr);

	return fn(trace_data, trace, trace_info);
}

void run_trace(c_trace_data* trace_data,
	float damage,
	float penetration,
	float range_modifier,
	unsigned int penetration_count,
	unsigned int teamnum,
	int* sv_showimpacts_penetration)
{

	using function_t = void(CS_FASTCALL*)(c_trace_data*, float, float, float, unsigned int, unsigned int, int*);
	static function_t fn = *reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, CXOR("40 53 57 41 56 48 83 EC ? 8B 84 24")));
	
	CS_ASSERT(fn != nullptr);

	return fn(trace_data, damage, penetration, range_modifier, penetration_count, 0, nullptr);
}

float c_auto_wall::get_hitgroup_damage_multiplier(C_CSPlayerPawn* player, float head_shot_mult, int hit_group)
{
	float mult = 1.F;

	static CConVar* mp_damage_scale_ct_head = I::Cvar->Find(FNV1A::HashConst(CXOR("mp_damage_scale_ct_head")));
	static CConVar* mp_damage_scale_t_head = I::Cvar->Find(FNV1A::HashConst(CXOR("mp_damage_scale_t_head")));
	static CConVar* mp_damage_scale_ct_body = I::Cvar->Find(FNV1A::HashConst(CXOR("mp_damage_scale_ct_body")));
	static CConVar* mp_damage_scale_t_body = I::Cvar->Find(FNV1A::HashConst(CXOR("mp_damage_scale_t_body")));

	auto head_scale = player->GetTeam() == 3 ? mp_damage_scale_ct_head->get_float() : mp_damage_scale_t_head->get_float();
	auto body_scale = player->GetTeam() == 3 ? mp_damage_scale_ct_body->get_float() : mp_damage_scale_t_body->get_float();

	switch (hit_group)
	{
	case 1:
		mult = head_shot_mult * head_scale;
		break;
	case 3:
		mult = 1.25F * body_scale;
		break;
	case 6:
	case 7:
		mult = 0.75F * body_scale;
		break;
	default:
		mult *= body_scale;
	}

	return mult;
}

void c_auto_wall::scale_damage(int hit_group, C_CSPlayerPawn* player, CCSWeaponBaseVData* wpn_data, float& damage)
{
	if (!player)
		return;

	damage *= get_hitgroup_damage_multiplier(player, wpn_data ? wpn_data->m_flHeadshotMultiplier() : 4.f, hit_group);

	if (player->GetArmorValue() > 0 && wpn_data)
	{
		if (hit_group == 1)
		{
			if (player->GetItemServices() && player->GetItemServices()->m_bHasHelmet())
				damage *= (wpn_data->m_flArmorRatio() * 0.5f);
		}
		else
			damage *= (wpn_data->m_flArmorRatio() * 0.5f);
	}
}

bool c_fx_firebullet::run(C_CSPlayerPawn* entity, Vector_t& start, Vector_t& end, penetration_data_t& pen_data)
{
	auto weapon = SDK::LocalPawn->ActiveWeapon();
	if (!weapon)
		return false;

	auto weapon_data = weapon->datawep();
	if (!weapon_data)
		return false;

	c_trace_data* trace_data = (c_trace_data*)I::MemAlloc->Alloc(sizeof(c_trace_data));
	ZeroMemory(trace_data, sizeof(c_trace_data));

	c_trace_filter filter(0x1C3003, SDK::LocalPawn, nullptr, 4);
	int penetration_count = 4;

	Vector_t vec_direction = end - start;

	vec_direction.normalizess();

	Vector_t end_pos = vec_direction * weapon_data->m_flRange();

	init_trace_data(trace_data, start, end_pos, filter, penetration_count);

	bool ret = false;

	if (trace_data->m_trace_info)
	{
		run_trace(trace_data, weapon_data->m_nDamage(), weapon_data->m_flPenetration(), weapon_data->m_flRangeModifier(), penetration_count, SDK::LocalPawn->GetTeam(), nullptr);

		for (int i = 0; i < trace_data->m_surfaces; i++)
		{
			trace_data->m_trace_info[i].m_unk = 0.f;
			auto trace_info = trace_data->m_trace_info[i];

			game_trace_t trace{};
			get_trace(trace_data, &trace, &trace_info);

			if (entity)
			{
				if (trace.HitEntity && entity && trace.HitEntity == entity)
				{
					pen_data.m_damage = trace_info.m_damage;

					g_auto_wall->scale_damage(trace.get_hit_group(), trace.HitEntity, weapon_data, pen_data.m_damage);

					pen_data.m_hitbox = trace.get_hitbox_id();
					pen_data.m_hitgroup = trace.get_hit_group();

					return true;
				}
			}
			else
				SDK::can_penetration = trace_info.m_pen_count != 4 && trace_info.m_pen_count != 0;
		}
	}

	if (trace_data)
	{
		if (trace_data->m_array_ptr)
			I::MemAlloc->Free(trace_data->m_array_ptr);
		if (trace_data->m_some_unk_ptr)
			I::MemAlloc->Free(trace_data->m_some_unk_ptr);
		if (trace_data->m_trace_info)
			I::MemAlloc->Free(trace_data->m_trace_info);

		I::MemAlloc->Free(trace_data);
	}

	return false;
}