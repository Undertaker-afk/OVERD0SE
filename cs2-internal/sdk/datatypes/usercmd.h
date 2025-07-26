#pragma once

#include "../../core/sdk.h"
#include "../../sdk/datatypes/proto/usercmd.pb.hpp"
#include "../../sdk/datatypes/proto/cs_usercmd.pb.hpp"

// compiled protobuf messages and looked at what bits are used in them
enum ECommandButtons : std::uint64_t
{
	IN_ATTACK = 1 << 0,
	IN_JUMP = 1 << 1,
	IN_DUCK = 1 << 2,
	IN_FORWARD = 1 << 3,
	IN_BACK = 1 << 4,
	IN_USE = 1 << 5,
	IN_LEFT = 1 << 7,
	IN_RIGHT = 1 << 8,
	IN_MOVELEFT = 1 << 9,
	IN_MOVERIGHT = 1 << 10,
	IN_SECOND_ATTACK = 1 << 11,
	IN_RELOAD = 1 << 13,
	IN_SPRINT = 1 << 16,
	IN_JOYAUTOSPRINT = 1 << 17,
	IN_SHOWSCORES = 1ULL << 33,
	IN_ZOOM = 1ULL << 34,
	IN_LOOKATWEAPON = 1ULL << 35
};

class CInButtonState
{
public:
	std::byte pad_0000[8]; //0x0000
	uint64_t m_nValue; //0x0008
	uint64_t m_nValueChanged; //0x0010
	uint64_t m_nValueScroll; //0x0018
}; //Size: 0x0020
static_assert(sizeof(CInButtonState) == 0x20);

template <typename T>
struct repeated_ptr_field_t
{
	struct c_rep_t
	{
		int m_allocated_size;
		T* m_elements[(INT_MAX - 2 * sizeof(int)) / sizeof(void*)];
	};

	void* m_arena;
	int m_current_size;
	int m_total_size;
	c_rep_t* m_rep;
};

inline float AngleDifference(float current, float target) {
	float delta = target - current;

	while (delta > 180.0f) delta -= 360.0f;
	while (delta < -180.0f) delta += 360.0f;

	return delta;
}

inline float ClampAngleChange(float current, float target, float maxDelta) 
{
	float delta = AngleDifference(current, target);

	if (delta > maxDelta) {
		target = current + maxDelta;
	}
	else if (delta < -maxDelta) {
		target = current - maxDelta;
	}

	return target;
}

inline float SnapToNearest90Degrees(float angle) 
{
	while (angle < 0) angle += 360.0f;
	while (angle >= 360.0f) angle -= 360.0f;

	if (angle >= 0 && angle < 45) return 0.0f;
	if (angle >= 45 && angle < 135) return 90.0f;
	if (angle >= 135 && angle < 225) return 180.0f;
	if (angle >= 225 && angle < 315) return 270.0f;

	return 0.0f;
}

class CUserCmd
{
public:
	std::byte pad_valve_shizo[0x8];
	int nPrevSequenceNumber;
	CSGOUserCmdPB m_csgoUserCmd;
	CInButtonState m_nButtons; //0x0040
private:
	std::byte pad_0068[0x8]; //0x0068
public:
	uint32_t random_seed; //0x0070
	float current_time;//0x0074
private:
	std::byte pad_78[16];//0x0078
public:

	void SetSubTickAngle(const QAngle_t& angView) {

		for (int i = 0; i < m_csgoUserCmd.input_history_size(); i++)
		{
			auto* pInputEntry = m_csgoUserCmd.mutable_input_history(i);
			if (pInputEntry == nullptr)
				continue;

			if (!pInputEntry->has_view_angles())
				continue;


			pInputEntry->mutable_view_angles()->set_x(angView.x);
			pInputEntry->mutable_view_angles()->set_y(angView.y);
			pInputEntry->mutable_view_angles()->set_z(angView.z);
		}
	}

	void AdjustAttackStartIndex(float nTick) {
		m_csgoUserCmd.set_attack1_start_history_index(nTick);
		m_csgoUserCmd.set_attack2_start_history_index(nTick);
		m_csgoUserCmd.set_attack3_start_history_index(nTick);
	}

	CSubtickMoveStep* create_new_subtick_move_step(repeated_ptr_field_t<CSubtickMoveStep>* rept_ptr, void* arena) {
		static auto fn_create_new_subtick_move_step = reinterpret_cast<CSubtickMoveStep * (CS_FASTCALL*)(void*)>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, CXOR("E8 ? ? ? ? 48 8B D0 48 8D 4F ? E8 ? ? ? ? 48 8B D0")), 0x1));
		static auto fn_add_element_to_rep_field_container = reinterpret_cast<CSubtickMoveStep * (CS_FASTCALL*)(repeated_ptr_field_t<CSubtickMoveStep>*, void*)>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, CXOR("E8 ? ? ? ? 48 8B D0 8B 4A ? F3 41 0F 10 46")), 0x1));


		auto subtick_move = fn_create_new_subtick_move_step(arena);

		return fn_add_element_to_rep_field_container(rept_ptr, subtick_move);
	}

	CSubtickMoveStep* add_subtick_move_step(CUserCmd* user_cmd) {

		CSubtickMoveStep* subtick_move_step = nullptr;

		auto rept_field_move_steps = reinterpret_cast<repeated_ptr_field_t<CSubtickMoveStep>*>((PBYTE)user_cmd->m_csgoUserCmd.mutable_base() + 0x18);

		if (rept_field_move_steps->m_rep && rept_field_move_steps->m_current_size < rept_field_move_steps->m_rep->m_allocated_size)
			subtick_move_step = rept_field_move_steps->m_rep->m_elements[rept_field_move_steps->m_current_size++];
		else
			subtick_move_step = create_new_subtick_move_step(rept_field_move_steps, rept_field_move_steps->m_arena);

		return subtick_move_step;
	}

	void ProcessSubTick(CSubtickMoveStep* move_step, uint64_t m_button, float m_when, bool m_pressed, bool m_need_to_be_deleted)
	{
		if (m_need_to_be_deleted)
			move_step->clear_button();
		else
			move_step->set_button(m_button);

		move_step->set_when(m_when);
		move_step->set_pressed(m_pressed);
	}
};