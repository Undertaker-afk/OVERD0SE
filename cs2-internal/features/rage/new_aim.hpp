#include "../../utilities/common.h"
#include "../../utilities/draw.h"
#include "../../sdk/datatypes/vector.h"
#include "../../sdk/datatypes/transform.h"
#include "../../core/sdk.h"
#include "../../sdk/datatypes/qangle.h"
#include "../../core/config.h"
#include "../../core/config.h"
#include "../../sdk/datatypes/usercmd.h"
#include "../../core/sdk.h"
#include "../../sdk/entity.h"
#include "../../sdk/interfaces/iengineclient.h"
#include "../../sdk/interfaces/iglobalvars.h"
#include "../../sdk/interfaces/cgameentitysystem.h"
#include "../../sdk/datatypes/qangle.h"
#include "../../sdk/datatypes/vector.h"
#include "../misc/movement.h"
#include "../../sdk/interfaces/ccsgoinput.h"
#include "../../sdk/interfaces/ienginecvar.h"
#include "lagcomp.h"
#include "../../sdk/interfaces/events.h"
#include "../autowall/autowall.h"
#include "../../sdk/interfaces/itrace.h"

#include <iostream>
#include <memory>
#include <deque>
#include <array>
#include <unordered_map>

class CCSPlayerController;
class C_BaseEntity;
class C_CSPlayerPawn;
class CBaseHandle;
class CEntityInstance;
class CUserCmd;
class CBaseUserCmdPB;
class QAngle_t;
class CCSGOInput;
class IGameEvent;
class C_CSWeaponBase;
class CCSWeaponBaseVData;

struct lag_record_t 
{
	Vector_t m_origin{};
	C_CSPlayerPawn* m_pawn{};
	CSkeletonInstance* m_skeleton = nullptr;

	bone_data m_bone_data[128];
	bone_data m_bone_data_backup[128];

	float m_simulation_time{};

	Vector_t m_vec_mins{};
	Vector_t m_vec_maxs{};
	Vector_t m_vec_velocity{};

	bool m_throwing{};

	lag_record_t() {}

	inline lag_record_t(C_CSPlayerPawn* pawn) {
		this->store(pawn);
	}

	inline void store(C_CSPlayerPawn* pawn) {
		if (!pawn || !pawn->is_alive())
			return;

		auto scene_node = pawn->GetGameSceneNode();
		if (!scene_node)
			return;

		auto skeleton_instance = pawn->GetGameSceneNode()->GetSkeletonInstance();
		if (!skeleton_instance)
			return;

		auto collision = pawn->GetCollision();
		if (!collision)
			return;

		static auto setup_bones = reinterpret_cast<void(CS_FASTCALL*)(void*, int)>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, CXOR("E8 ? ? ? ? 48 8B 05 ? ? ? ? 44 0F B7 AB")), 0x1));

		m_pawn = pawn;

		m_skeleton = skeleton_instance;
		m_origin = scene_node->m_vecOrigin();

		setup_bones(m_skeleton, 128);

		std::memcpy(m_bone_data, skeleton_instance->m_pBoneCache, sizeof(Matrix2x4_t) * skeleton_instance->m_nBones);

		m_simulation_time = pawn->m_flSimulationTime();
		m_vec_mins = collision->GetMins();
		m_vec_maxs = collision->GetMaxs();
		m_vec_velocity = pawn->m_vecVelocity();

		m_throwing = pawn->is_throwing();
	}

	inline void apply(C_CSPlayerPawn* pawn) {
		if (!pawn || !pawn->is_alive())
			return;

		auto scene_node = pawn->GetGameSceneNode();
		if (!scene_node)
			return;

		auto skeleton_instance = scene_node->GetSkeletonInstance();
		if (!skeleton_instance)
			return;

		std::memcpy(m_bone_data_backup, skeleton_instance->m_pBoneCache, sizeof(Matrix2x4_t) * skeleton_instance->m_nBones);

		std::memcpy(skeleton_instance->m_pBoneCache, m_bone_data, sizeof(Matrix2x4_t) * skeleton_instance->m_nBones);
	}

	inline void reset(C_CSPlayerPawn* pawn) {
		if (!pawn || !pawn->is_alive())
			return;

		auto scene_node = pawn->GetGameSceneNode();
		if (!scene_node)
			return;

		auto skeleton_instance = scene_node->GetSkeletonInstance();
		if (!skeleton_instance)
			return;

		std::memcpy(skeleton_instance->m_pBoneCache, m_bone_data_backup, sizeof(Matrix2x4_t) * skeleton_instance->m_nBones);
	}

	bool is_valid() {
		float dead_time = (SDK::LocalPawn->m_flSimulationTime() * 0.015625f) - m_simulation_time + I::Client->GetLerpTime();
		return dead_time < 0.2f;
	}
};

struct hitbox_data_t
{
	hitbox_data_t() = default;

	int m_num_hitbox;
	int m_num_bone;
	int m_num_hitgroup;
	c_hitbox* m_hitbox_data;
	float m_radius;
	Matrix3x4a_t m_matrix;
	Vector_t m_mins;
	Vector_t m_maxs;
	bool m_invalid_data{ true };
};

struct aim_point_t 
{
	aim_point_t() = default;

	aim_point_t(Vector_t point, int hitbox, bool center = false) {
		this->m_point = point;
		this->m_hitbox = hitbox;
		this->m_center = center;

		this->m_valid = true;
	}

	bool m_valid{ false };
	Vector_t m_point{};
	int m_hitbox{};
	bool m_center{};
	float m_damage{ 0.f };
};

struct aim_target_t 
{
	lag_record_t* m_lag_record{};

	aim_point_t m_best_point{};

	C_CSPlayerPawn* m_pawn{ nullptr };

	int m_handle{};

	aim_target_t() = default;

	inline aim_target_t(C_CSPlayerPawn* pawn, int handle) 
	{
		if (!pawn)
			return;

		m_pawn = pawn;
		m_handle = handle;
	}

	inline void store(lag_record_t* record) 
	{
		if (!record || !record->m_pawn)
			return;

		m_lag_record = record;
	}

	inline void reset() 
	{
		m_lag_record = nullptr;
		m_pawn = nullptr;
		m_best_point = {};
	}
};

struct accuracy_data_t {
	std::array < float_t, 256 > m_inaccuracy_arr = { };
	std::array < float_t, 256 > m_spread_arr = { };
	std::array < float_t, 256 > m_multi_values_arr = { };
	std::array < float_t, 256 > m_inaccuracy_multi_values_arr = { };
	std::array < float_t, 256 > m_spread_multi_values_arr = { };

	float_t m_add_accuracy = 0.0f;
	float_t m_lowest_accuracy = 0.0f;

	float_t m_inaccuracy = 0.0f;
	float_t m_spread = 0.0f;
	float_t m_min_inaccuracy = 0.0f;

	bool m_has_valid_accuracy_data = false;
	bool m_calculate_accuracy = false;
	bool m_has_maximum_accuracy = false;
	bool m_has_fired = false;
} inline m_accuracy_data;

inline Matrix3x4_t transform_to_matrix(bone_data& in) {
	Matrix3x4_t matrix;

	const Vector4D_t& rot = in.rot;
	const Vector_t& pos = in.pos;

	float xx = rot.x * rot.x;
	float yy = rot.y * rot.y;
	float zz = rot.z * rot.z;
	float xy = rot.x * rot.y;
	float xz = rot.x * rot.z;
	float yz = rot.y * rot.z;
	float wx = rot.w * rot.x;
	float wy = rot.w * rot.y;
	float wz = rot.w * rot.z;

	matrix[0][0] = 1.0f - 2.0f * (yy + zz);
	matrix[1][0] = 2.0f * (xy + wz);
	matrix[2][0] = 2.0f * (xz - wy);

	matrix[0][1] = 2.0f * (xy - wz);
	matrix[1][1] = 1.0f - 2.0f * (xx + zz);
	matrix[2][1] = 2.0f * (yz + wx);

	matrix[0][2] = 2.0f * (xz + wy);
	matrix[1][2] = 2.0f * (yz - wx);
	matrix[2][2] = 1.0f - 2.0f * (xx + yy);

	matrix[0][3] = pos.x;
	matrix[1][3] = pos.y;
	matrix[2][3] = pos.z;

	return matrix;
}

class c_rage_bot 
{
	std::vector<e_hitboxes> m_hitboxes;

	std::unordered_map<int, std::deque<lag_record_t>> m_lag_records{};
	std::unordered_map<int, aim_target_t> m_aim_targets{};

	aim_target_t* m_best_target{};

	inline int get_hitbox_from_menu(int hitbox) {
		switch (hitbox) {
		case hitbox_head:
			return 0;
		case hitbox_chest:
		case hitbox_lower_chest:
		case hitbox_upper_chest:
			return 1;
		case hitbox_pelvis:
		case hitbox_stomach:
			return 2;
		case hitbox_right_hand:
		case hitbox_left_hand:
		case hitbox_right_upper_arm:
		case hitbox_right_forearm:
		case hitbox_left_upper_arm:
		case hitbox_left_forearm:
			return 3;
		case hitbox_right_thigh:
		case hitbox_left_thigh:
		case hitbox_right_calf:
		case hitbox_left_calf:
			return 4;
		case hitbox_right_foot:
		case hitbox_left_foot:
			return 5;
		}

		return -1;
	}

	hitbox_data_t get_hitbox_data(C_CSPlayerPawn* player_pawn, const int hitbox_id) {
		if (!player_pawn)
			return hitbox_data_t();

		auto game_scene = player_pawn->GetGameSceneNode();
		if (!game_scene)
			return hitbox_data_t();

		auto skeleton = game_scene->GetSkeletonInstance();
		if (!skeleton)
			return hitbox_data_t();

		auto& model_state = skeleton->GetModel();
		auto& model = model_state.m_hModel();

		auto hitbox_data = model->get_hitbox(hitbox_id);
		if (!hitbox_data)
			return hitbox_data_t();

		const auto bone_index = player_pawn->get_bone_index(hitbox_data->m_name);
		const auto bones = model_state.GetHitboxData();

		hitbox_data_t hitbox;
		hitbox.m_num_hitbox = hitbox_id;
		hitbox.m_num_bone = bone_index;
		hitbox.m_hitbox_data = hitbox_data;
		hitbox.m_radius = hitbox_data->m_flShapeRadius;
		hitbox.m_num_hitgroup = hitbox_data->m_nHitBoxIndex;
		hitbox.m_matrix = transform_to_matrix(bones[bone_index]);
		hitbox.m_mins = hitbox_data->m_vMinBounds;
		hitbox.m_maxs = hitbox_data->m_vMaxBounds;
		hitbox.m_invalid_data = false;

		return hitbox;
	}

	inline lag_record_t* get_first_record(int handle) {
		if (m_lag_records.find(handle) == m_lag_records.end())
			return nullptr;

		auto& records = m_lag_records[handle];

		if (records.empty())
			return nullptr;

		auto record = &records.front();
		if (!record->is_valid())
			return nullptr;

		return record;
	}

	Vector_t get_removed_aim_punch_angle(C_CSPlayerPawn* local_player) {
		using fnUpdateAnimPunch = void(CS_FASTCALL*)(void*, Vector_t*, float, bool);
		static auto fn = reinterpret_cast<fnUpdateAnimPunch>((MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 08 48 89 74 24 18 57 48 83 EC 70 48 8B F2 0F"))));

		Vector_t aim_punch = {};
		fn(local_player, &aim_punch, 0.0f, true);
		return aim_punch;
	}

	bool shouldstop;
	std::vector<std::tuple<float, float, float>> precomputed_seeds = {};

	struct aim_info
	{
		int minimum_damage;
		int minimum_damage_over;
		int minimum_damage_over_var;
		int hitchan_over;
		int hitchan_over_var;
		int rage_hitchance;
		int rage_target_select;
		int point_scale;
		int rage_enable;

		bool auto_stop;
		bool penetration;
		bool auto_scope;
		bool remove_weapon_accuracy_spread;

		bool hitbox_head;
		bool hitbox_chest;
		bool hitbox_stomach;
		bool hitbox_arms;
		bool hitbox_legs;
		bool hitbox_feets;

		bool point_head;
		bool point_chest;
		bool point_stomach;
		bool point_arms;
		bool point_legs;
		bool point_feets;

		bool accurate;
	} rage_data;

	void store_hitboxes(C_CSPlayerPawn* pawn);
	void setup_weapon(C_CSPlayerPawn* pLocal);

	lag_record_t* select_last_record(int handle);
	lag_record_t* select_record(int handle);

	void remove_target(int entry_index);
	void find_target();

	Vector_t transform_point(Matrix3x4_t matrix, Vector_t point);

	std::vector<Vector_t> calculate_sphere_points(float radius, int num_points);
	std::vector<Vector_t> calculate_points(int num_points, float radius, float height, Matrix3x4_t& matrix, hitbox_data_t& hitbox);

	bool multi_points(lag_record_t* record, int hitbox, std::vector<aim_point_t>& points, bool center_only = false);

	aim_point_t select_points(lag_record_t* record, const std::vector<e_hitboxes>& hitboxes, bool center_only = false);
	aim_target_t* get_nearest_target();

	void scan_target();

	bool weapon_is_at_max_accuracy(CCSWeaponBaseVData* weapon_data);
	bool hitchance(Vector_t& pos, bone_data* matrix, Vector_t angles, C_CSPlayerPawn* player, int index);

	int get_minimum_damage(int health);

	void process_backtrack(lag_record_t* record);
	void process_attack(CUserCmd* user_cmd, Vector_t angle);
public:
	void build_seed_table();

	bool automatic_stop(C_CSPlayerPawn* pLocal, C_CSWeaponBase* weapon, CUserCmd* cmd);
	bool can_shoot(C_CSPlayerPawn* pawn, C_CSWeaponBase* active_weapon);

	void store_records();
	void calculate_max_accuracy();
	void on_create_move();
};

const auto g_rage_bot = std::make_unique<c_rage_bot>();