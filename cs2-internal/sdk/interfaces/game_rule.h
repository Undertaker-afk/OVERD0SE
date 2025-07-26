#pragma once

// used: mem_pad
#include "../../utilities/memory.h"
#include "../../sdk/datatypes/vector.h"

class Vector_t;

class c_view_vectors
{
public:
	Vector_t vec_view; //0x0000
	Vector_t hull_min; //0x000C
	Vector_t hull_max; //0x0018
	Vector_t duck_hull_min; //0x0024
	Vector_t duck_hull_max; //0x0030
	Vector_t duck_view; //0x003C
	Vector_t obs_hull_min; //0x0048
	Vector_t obs_hull_max; //0x0054
	Vector_t dead_view; //0x0060
}; //Size: 0x006C

class c_cs_game_rules
{
public:
	char pad_0000[64]; //0x0000
	bool freeze_pause; //0x0040
	char pad_0041[15]; //0x0041
	float cur_pause_time; //0x0050
	char pad_0054[40]; //0x0054
	float anim_swap_time; //0x007C
	char pad_0080[16]; //0x0080
};