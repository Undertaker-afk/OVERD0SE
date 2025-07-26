#pragma once
#include "../../utilities/memory.h"
#include "../datatypes/color.h"
#include "../entity_handle.h"

#include "..\..\core\sdk.h"
#include "..\..\sdk\entity.h"

#define CLIP_TRACE_TO_PLAYERS "48 8B C4 55 56 48 8D A8 58 FF FF FF 48 81 EC 98 01 00 00 48"
#define TRACE_SHAPE "48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 55 41 56 48 8D AC 24"
#define COMBINE(x, y) x##y
#define COMBINE2(x, y) COMBINE(x, y)

#include <array>
#include <cstddef>

#define PAD_CLASS_DEBUG(sz) int COMBINE2(pad_, __COUNTER__)[sz];
#define CS2_PAD( number, size )                          \
private:                                                 \
    [[maybe_unused]] std::array< std::byte, size > m_unknown_##number{ }; \
public:
enum Contents_t {
	CONTENTS_EMPTY = 0,
	CONTENTS_SOLID = 0x1,
	CONTENTS_WINDOW = 0x2,
	CONTENTS_AUX = 0x4,
	CONTENTS_GRATE = 0x8,
	CONTENTS_SLIME = 0x10,
	CONTENTS_WATER = 0x20,
	CONTENTS_BLOCKLOS = 0x40,
	CONTENTS_OPAQUE = 0x80,
	CONTENTS_TESTFOGVOLUME = 0x100,
	CONTENTS_UNUSED = 0x200,
	CONTENTS_BLOCKLIGHT = 0x400,
	CONTENTS_TEAM1 = 0x800,
	CONTENTS_TEAM2 = 0x1000,
	CONTENTS_IGNORE_NODRAW_OPAQUE = 0x2000,
	CONTENTS_MOVEABLE = 0x4000,
	CONTENTS_AREAPORTAL = 0x8000,
	CONTENTS_PLAYERCLIP = 0x10000,
	CONTENTS_MONSTERCLIP = 0x20000,
	CONTENTS_CURRENT_0 = 0x40000,
	CONTENTS_CURRENT_90 = 0x80000,
	CONTENTS_CURRENT_180 = 0x100000,
	CONTENTS_CURRENT_270 = 0x200000,
	CONTENTS_CURRENT_UP = 0x400000,
	CONTENTS_CURRENT_DOWN = 0x800000,
	CONTENTS_ORIGIN = 0x1000000,
	CONTENTS_MONSTER = 0x2000000,
	CONTENTS_DEBRIS = 0x4000000,
	CONTENTS_DETAIL = 0x8000000,
	CONTENTS_TRANSLUCENT = 0x10000000,
	CONTENTS_LADDER = 0x20000000,
	CONTENTS_HITBOX = 0x40000000,
};

enum Masks_t {
	MASK_ALL = 0xFFFFFFFF,
	MASK_SOLID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
	MASK_PLAYERSOLID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
	MASK_NPCSOLID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
	MASK_NPCFLUID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
	MASK_WATER = CONTENTS_WATER | CONTENTS_MOVEABLE | CONTENTS_SLIME,
	MASK_OPAQUE = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_OPAQUE,
	MASK_OPAQUE_AND_NPCS = MASK_OPAQUE | CONTENTS_MONSTER,
	MASK_BLOCKLOS = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_BLOCKLOS,
	MASK_BLOCKLOS_AND_NPCS = MASK_BLOCKLOS | CONTENTS_MONSTER,
	MASK_VISIBLE = MASK_OPAQUE | CONTENTS_IGNORE_NODRAW_OPAQUE,
	MASK_VISIBLE_AND_NPCS = MASK_OPAQUE_AND_NPCS | CONTENTS_IGNORE_NODRAW_OPAQUE,
	MASK_SHOT = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE | CONTENTS_HITBOX,
	MASK_SHOT_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_DEBRIS,
	MASK_SHOT_HULL = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE,
	MASK_SHOT_PORTAL = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER,
	MASK_SOLID_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_GRATE,
	MASK_PLAYERSOLID_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_PLAYERCLIP | CONTENTS_GRATE,
	MASK_NPCSOLID_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE,
	MASK_NPCWORLDSTATIC = CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE,
	MASK_NPCWORLDSTATIC_FLUID = CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP,
	MASK_SPLITAREPORTAL = CONTENTS_WATER | CONTENTS_SLIME,
	MASK_CURRENT = CONTENTS_CURRENT_0 | CONTENTS_CURRENT_90 | CONTENTS_CURRENT_180 | CONTENTS_CURRENT_270 | CONTENTS_CURRENT_UP | CONTENTS_CURRENT_DOWN,
	MASK_DEADSOLID = CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_GRATE,
};

enum {
	SURF_LIGHT = 0x0001,
	SURF_SKY2D = 0x0002,
	SURF_SKY = 0x0004,
	SURF_WARP = 0x0008,
	SURF_TRANS = 0x0010,
	SURF_NOPORTAL = 0x0020,
	SURF_TRIGGER = 0x0040,
	SURF_NODRAW = 0x0080,
	SURF_HINT = 0x0100,
	SURF_SKIP = 0x0200,
	SURF_NOLIGHT = 0x0400,
	SURF_BUMPLIGHT = 0x0800,
	SURF_NOSHADOWS = 0x1000,
	SURF_NODECALS = 0x2000,
	SURF_NOPAINT = SURF_NODECALS,
	SURF_NOCHOP = 0x4000,
	SURF_HITBOX = 0x8000
};
struct ray_t
{
	Vector_t Start{};
	Vector_t End{};
	Vector_t Mins{};
	Vector_t Maxs{};
	std::byte pad_001[0x4];
	std::uint8_t UnkownType{};
};
static_assert(sizeof(ray_t) == 0x38);

struct ray_d
{
public:
	Vector_t m_start{};
	Vector_t m_end{};
	Vector_t m_mins{};
	Vector_t m_maxs{};
	std::byte pad_001[0x4];
	std::uint8_t type{};
};
static_assert(sizeof(ray_d) == 0x38);

struct surface_data_d
{
public:
	std::byte pad_001[0x8];
	float m_pen_data_modifier{};
	float m_dmg_modifier{};
	std::byte pad_002[0x4];
	int m_material{};
};
static_assert(sizeof(surface_data_d) == 0x18);

struct trace_hitbox_data_d
{
public:
	std::byte pad_001[0x38];
	int m_hit_group{};
	std::byte pad_002[0x4];
	int m_hitbox_id{};
};
static_assert(sizeof(trace_hitbox_data_d) == 0x44);

struct game_trace_d
{
public:
	game_trace_d() = default;

	surface_data_d* get_surface_data_t();
	int get_hitbox_id()
	{
		if (m_hitbox_data)
			return m_hitbox_data->m_hitbox_id;

		return 0;
	}

	int get_hit_group()
	{
		if (m_hitbox_data)
			return m_hitbox_data->m_hit_group;

		return 0;
	}

	void* m_surface{};
	C_CSPlayerPawn* m_hit_entity{};
	trace_hitbox_data_d* m_hitbox_data{};
	std::byte pad_001[0x38];
	std::uint32_t m_contents{};
	std::byte pad_002[0x24];
	Vector_t m_start{};
	Vector_t m_end{};
	Vector_t m_normal{};
	Vector_t m_pos{};
	std::byte pad_003[0x4];
	float m_fraction{};
	std::byte pad_004[0x6];
	bool m_all_solid{};
	std::byte pad_005[0x4D];
};

struct trace_filter_d
{
public:
	std::byte pad_001[0x8];
	std::int64_t m_mask{};
	std::array<std::int64_t, 2> m_ptr{};
	std::array<std::int32_t, 4> m_skip_handles{};
	std::array<std::int16_t, 2> m_arr_collisions{};
	std::int16_t m_ptr2{};
	std::uint8_t m_ptr3{};
	std::uint8_t m_ptr4{};
	std::uint8_t m_ptr5{};

	trace_filter_d() = default;
	trace_filter_d(std::uint64_t mask, C_CSPlayerPawn* entity, C_CSPlayerPawn* player, int layer);
};

class trace_filter_t
{
public:
	std::byte pad_001[0x8];
	std::int64_t trace_mask{};
	std::array<std::int64_t, 2> m_ptr{};
	std::array<std::int32_t, 4> SkipHandles{};
	std::array<std::int16_t, 2> Collisions{};
	std::int16_t m_ptr2{};
	std::uint8_t m_ptr3{};
	std::uint8_t m_ptr4{};
	std::uint8_t m_ptr5{};
};

class c_trace_filter
{
public:
	std::uintptr_t* ptr; // 0x0
	std::uint64_t trace_mask; // 0x8
	std::uint64_t unk1;
	std::uint64_t unk2;
	int skip_handle1; // 0x20
	int skip_handle2; // 0x24
	int skip_owner_handle1;
	int skip_owner_handle2;
	std::uint16_t collision1; // 0x30
	std::uint16_t collision2; // 0x32
	std::uint16_t N0000011C; // 0x34
	std::uint8_t layer; // 0x36
	std::uint8_t flags; // 0x37
	std::uint8_t null_it3; // 0x38
	c_trace_filter(std::uint64_t trace_mask, C_CSPlayerPawn* local_player, C_CSPlayerPawn* entity, std::uint8_t layer);
};

struct C_TraceHitboxData {
	CS2_PAD(0, 0x38)
	int m_hitgroup{ };
};

struct trace_hitbox_data_t
{
	std::byte pad_001[0x38];
	int m_hit_group{};
	std::byte pad_002[0x4];
	int m_hitbox_id{};
};

static_assert(sizeof(trace_hitbox_data_t) == 0x44);

class game_trace_t {
public:

	int get_hitbox_id()
	{
		if (HitboxData)
			return HitboxData->m_hitbox_id;

		return 0;
	}

	int get_hit_group()
	{
		if (HitboxData)
			return HitboxData->m_hit_group;

		return 0;
	}

	void* Surface;
	C_CSPlayerPawn* HitEntity;
	trace_hitbox_data_t* HitboxData;
	CS2_PAD(0, 0x38)
	std::uint32_t Contents;
	CS2_PAD(1, 0x24)
	Vector_t m_start_pos, m_end_pos, m_normal, m_pos;
	MEM_PAD(0x4);
	float Fraction;
	MEM_PAD(0x6);
	bool m_all_solid;
	CS2_PAD(4, 0x4D)
};

static_assert(sizeof(game_trace_t) == 0x108);

struct UpdateValueT {
	float previousLenght{ };
	float currentLenght{ };
	CS2_PAD(0, 0x8)
		std::int16_t handleIdx{ };
	CS2_PAD(1, 0x6)
};
struct trace_arr_element_t {
	CS2_PAD(0, 0x30)
};

struct trace_data_t {
	std::int32_t m_uk1{ };
	float m_uk2{ 52.0f };
	void* m_arr_pointer{ };
	std::int32_t m_uk3{ 128 };
	std::int32_t m_uk4{ static_cast<std::int32_t>(0x80000000) };
	std::array< trace_arr_element_t, 0x80 > m_arr = { };
	CS2_PAD(0, 0x8)
		std::int64_t m_num_update{ };
	void* m_pointer_update_value{};
	CS2_PAD(1, 0xC8)
		Vector_t m_start{ }, m_end{ };
	CS2_PAD(2, 0x50)
};

class c_trace_info {
public:
	float m_unk;
	float m_distance;
	float m_damage;
	std::uint32_t m_pen_count;
	std::uint32_t m_handle;
	std::uint32_t m_handle_2;
};

class c_trace_data
{
public:
	char pad_001[0x8];
	void* m_array_ptr;
	char pad_002[0x1810];
	int m_surfaces;
	c_trace_info* m_trace_info;
	char pad_003[0x8];
	void* m_some_unk_ptr;
	char pad_005[0xB8];
	Vector_t m_start;
	Vector_t m_end;
	char pad_004[0x50];
};

struct trace_data_d {
	char pad_001[0x8];
	void* m_array_ptr;
	char pad_002[0x1810];
	int m_surfaces;
	c_trace_info* m_trace_info;
	char pad_003[0x8];
	void* m_some_unk_ptr;
	char pad_005[0xB8];
	Vector_t m_start;
	Vector_t m_end;
	char pad_004[0x50];
};

class i_trace
{
	using fn_init_trace_info = void(CS_FASTCALL*) (game_trace_d*);
	using fn_get_trace_info = void(CS_FASTCALL*) (trace_data_d*, game_trace_d*, c_trace_info*);

public:
	void init_trace(trace_filter_d& filter, C_CSPlayerPawn* pawn, uint64_t mask, uint8_t layer, uint16_t unknown) {
		using fn_init_trace_t = trace_filter_d * (CS_FASTCALL*)(trace_filter_d&, void*, uint64_t, uint8_t, uint16_t);
		static fn_init_trace_t fn = reinterpret_cast<fn_init_trace_t>(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 0F B6 41 37 33")));

		fn(filter, pawn, mask, layer, unknown);
	}

	void init_trace2(c_trace_filter& filter, C_CSPlayerPawn* pawn, uint64_t mask, uint8_t layer, uint16_t unknown) {
		using fn_init_trace_t = c_trace_filter * (CS_FASTCALL*)(c_trace_filter&, void*, uint64_t, uint8_t, uint16_t);
		static fn_init_trace_t fn = reinterpret_cast<fn_init_trace_t>(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 0F B6 41 37 33")));

		fn(filter, pawn, mask, layer, unknown);
	}

	bool trace_shape(ray_d* ray, Vector_t start, Vector_t end, trace_filter_d* filter, game_trace_d* game_trace)
	{
		using fn_trace_shape = bool(CS_FASTCALL*)(i_trace*, ray_d*, Vector_t*, Vector_t*, trace_filter_d*, game_trace_d*);
		static fn_trace_shape o_trace_shape = reinterpret_cast<fn_trace_shape>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, "E8 ? ? ? ? F3 0F 10 85 ? ? ? ? 0F 2E C7 0F 8A"), 0x1));

		return o_trace_shape(this, ray, &start, &end, filter, game_trace);
	}

	bool clip_ray_to_entity(ray_d* ray, const Vector_t& start, const Vector_t& end, C_CSPlayerPawn* player, trace_filter_d* filter, game_trace_d* trace)
	{
		using function_t = bool(CS_FASTCALL*)(i_trace*, ray_d*, const Vector_t&, const Vector_t&, C_CSPlayerPawn*, trace_filter_d*, game_trace_d*);
		static function_t fn = reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 54 41 56 41 57 48 81 EC C0 00 00 00 48 8B 9C"));

		return fn(this, ray, start, end, player, filter, trace);
	}

	bool clip_ray_to_entity2(ray_t* ray, const Vector_t& start, const Vector_t& end, C_CSPlayerPawn* player, c_trace_filter* filter, game_trace_t* trace)
	{
		using function_t = bool(CS_FASTCALL*)(i_trace*, ray_t*, const Vector_t&, const Vector_t&, C_CSPlayerPawn*, c_trace_filter*, game_trace_t*);
		static function_t fn = reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 54 41 56 41 57 48 81 EC C0 00 00 00 48 8B 9C"));

		return fn(this, ray, start, end, player, filter, trace);
	}

	void create_trace(trace_data_d* const trace, Vector_t start, Vector_t end, const trace_filter_d& filter, int penetration_count) {
		using fn_create_trace_t = void(CS_FASTCALL*) (trace_data_d*, Vector_t, Vector_t, trace_filter_d, void*, void*, void*, void*, int);
		static fn_create_trace_t fn = reinterpret_cast<fn_create_trace_t>(MEM::FindPattern(CLIENT_DLL, "48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 57 41 56 41 57 48 83 EC 40 F2"));

		fn(trace, start, end, filter, nullptr, nullptr, nullptr, nullptr, penetration_count);
	}

	void get_trace_info(trace_data_d* const trace, game_trace_d* const hit, c_trace_info* trace_info) {
		static fn_get_trace_info fn = reinterpret_cast<fn_get_trace_info>(MEM::FindPattern(CLIENT_DLL, "41 0F B7 40 ?? F3 41 0F 10 10"));

		fn(trace, hit, trace_info);
	}

	void Init(trace_filter_t& filter, C_CSPlayerPawn* skip, uint64_t mask, uint8_t layer, uint16_t idk)
	{
		using function_t = trace_filter_t * (CS_FASTCALL*)(trace_filter_t&, void*, uint64_t, uint8_t, uint16_t);
		static function_t fn = reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 0F B6 41 37 33")));

		if (fn == nullptr)
			printf(CXOR("trace_filter_t Invalid signature!\n"));

		CS_ASSERT(fn != nullptr);

		fn(filter, skip, mask, layer, idk);
	}

	void ClipTraceToPlayers(Vector_t& start, Vector_t& end, trace_filter_t* filter, game_trace_t* trace, float min, int length, float max)
	{
		using function_t = void(CS_FASTCALL*)(Vector_t&, Vector_t&, trace_filter_t*, game_trace_t*, float, int, float);
		static function_t fn = reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, CLIP_TRACE_TO_PLAYERS));

		if (fn == nullptr)
		{
			printf(CXOR("ClipTraceToPlayers creating new signature!\n"));
			fn = reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, "48 8B C4 55 56 48 8D A8 58"));
		}

		CS_ASSERT(fn != nullptr);

		fn(start, end, filter, trace, min, max, length);
	}

	void TraceShape(ray_t& ray, Vector_t* start, Vector_t* end, trace_filter_t filter, game_trace_t& trace)
	{
		using function_t = bool(CS_FASTCALL*)(void*, ray_t&, Vector_t*, Vector_t*, trace_filter_t, game_trace_t&);
		static function_t fn = reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 48 89 4C 24 08 55 41 54 41 55 41 56 41 57 48 8D"));
		
		CS_ASSERT(fn != nullptr);
		
		fn(this, ray, start, end, filter, trace);
	}
};