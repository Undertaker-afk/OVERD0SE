#pragma once

// used: viewmatrix_t
#include "../sdk/datatypes/matrix.h"
// used: color_t
#include "../sdk/datatypes/color.h"
#include <array>

#pragma region sdk_definitions
// @source: master/public/worldsize.h
// world coordinate bounds
#define MAX_COORD_FLOAT 16'384.f
#define MIN_COORD_FLOAT (-MAX_COORD_FLOAT)

// @source: master/public/vphysics_interface.h
// coordinates are in HL units. 1 unit == 1 inch
#define METERS_PER_INCH 0.0254f
#pragma endregion

class CCSPlayerController;
class C_CSPlayerPawn;
class CUserCmd;

namespace SDK
{
	// capture game's exported functions
	bool Setup();

	inline ViewMatrix_t ViewMatrix = ViewMatrix_t();
	inline Vector_t CameraPosition = Vector_t();
	inline CCSPlayerController* LocalController = nullptr;
	inline C_CSPlayerPawn* LocalPawn = nullptr;
	inline CUserCmd* Cmd = nullptr;
	inline bool freeze_time;
	inline bool bullets_es;
	inline float shoot_time;
	inline float last_hurt_time;
	inline int backup_tick;
	inline QAngle_t backup_VA;
	inline Vector_t vec_scr;
	inline Vector_t vec_pont_after_W2S;
	inline std::vector<Vector_t> points_for_print{};

	inline int last_hp;
	inline int last_dmg;
	inline int hitted_hitbox;
	inline int k_size;
	inline bool has_target;
	inline bool can_penetration = false;
	inline bool loaded_script = false;

	inline bool did_hit_ent;
	inline int num_hit_ent;
	inline int num_needed_ent;
	inline int hit_dmg;
	inline int hit_dmg_after;
	inline int hit_hitbox;
	inline int hit_hitgroup;

	struct TickData {
		bool processed;

		QAngle_t viewangles = { 0,0,0 };
		QAngle_t viewangles_second;

		int tick_cmd;
		int target_index;

		Vector_t target_index_dist;

		int last_num_cmd;
		int shot_num_cmd;
	};

	static TickData tickData;
	inline void(CS_CDECL* fnConColorMsg)(const Color_t&, const char*, ...) = nullptr;
}

class Cheat
{
public:
	bool onground = false;
	bool alive = false;
	bool canShot = false;
	bool willShot = false;
	bool menu_open = true;
	bool unload = false;

	float sv_maxunlag = 0.f;
	float lerp = 0.f;
	float latency = 0.f;
	float interval = 0.f;

	QAngle_t aim_angle = QAngle_t(0, 0, 0);

	std::array<float, 16> world_screen;

	QAngle_t viewangles = QAngle_t(0, 0, 0);
	Vector_t viewangles_vec;
	Vector_t eye_position{};
};

inline Cheat* cheat = new Cheat();