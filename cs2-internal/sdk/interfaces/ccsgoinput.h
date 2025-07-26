#pragma once

// used: mem_pad
#include "../../utilities/memory.h"

// used: cusercmd
#include "../datatypes/usercmd.h"

#define MULTIPLAYER_BACKUP 150

class CMoveStepButtons
{
public:
	std::uint64_t nKeyboardPressed; //0x0000
	std::uint64_t nMouseWheelheelPressed; //0x0008
	std::uint64_t nUnPressed; //0x0010
	std::uint64_t nKeyboardCopy; //0x0018
}; //Size: 0x0020

class CSubtickMoves {
public:
	float flWhen; // 0x0000
	float flDelta; // 0x0004
	std::uint64_t nButton; // 0x0008
	bool bPressed; // 0x0010
	std::uint32_t nUnknown; // 0x0014
}; //Size: 0x0018

class CExtendedMoveData : public CMoveStepButtons {
public:
	float flForwardMove; // 0x0020
	float flSideMove; // 0x0024
	float flUpMove; // 0x0028
	int nMousedX; // 0x002C
	int nMousedY; // 0x0030
	std::uint32_t nSubticksCount; // 0x0034
	CSubtickMoves SubtickMoves[12]; // 0x0038
	Vector_t vecViewAngles; // 0x0158
	int nTargetHandle; // 0x0164
}; //Size: 0x0168

class CCSGOInput
{
public:
	MEM_PAD(0x250); // 0x0000
	bool bBlockShoot; // 0x0250
	bool bInThirdPerson; // 0x0251
	MEM_PAD(0x6); // 0x0252
	Vector_t vecThirdPersonAngles; // 0x0258
	MEM_PAD(0x14); // 0x0264
	CExtendedMoveData CurrentMoveData; // 0x0278
	std::uint32_t nWeaponSwitchTick; // 0x03E0
	MEM_PAD(0x22C); // 0x03E4
	bool bMouseIsIntialized; // 0x0610
	MEM_PAD(0x5); // 0x0611
	bool bInAttack; // 0x0616
	bool bInAttack2; // 0x0617
	int nAttackStartHistoryIndex1; // 0x0618
	int nAttackStartHistoryIndex2; // 0x061C
	int nAttackStartHistoryIndex3; // 0x0620
	MEM_PAD(0x4); // 0x0624
	int nMessageSize; // 0x0628
	MEM_PAD(0x4); // 0x062C
	CCSInputMessage* pInputMessage; // 0x0630
	MEM_PAD(0x10); // 0x0638
	std::uint32_t nWeaponShotFrame; // 0x0648

	static auto GetArrayOfUserCmds()
	{
		static auto gay = *reinterpret_cast<CUserCmd**>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, CXOR("48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B CF 4C 8B E8")), 0x3));

		return gay;
	}

	CUserCmd* GetUserCmd()
	{
		auto controller = CCSPlayerController::GetLocalPlayerController();
		if (!controller)
			return nullptr;

		static void* (CS_FASTCALL * GetCommandIndex)(void* pController, int* index) = reinterpret_cast<decltype(GetCommandIndex)>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, ("E8 ? ? ? ? 8B 8D ? ? ? ? 8D 51")), 0x1, 0x0));

		if (!GetCommandIndex)
			return nullptr;

		int index = 0;
		GetCommandIndex(controller, &index);
		int commandIndex = index - 1;

		if (commandIndex == -1)
			commandIndex = 0xFFFFFFFF;

		static void* (CS_FASTCALL * GetUserCmdBase)(void* a1, int a2) = reinterpret_cast<decltype(GetUserCmdBase)>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, ("E8 ? ? ? ? 48 8B CF 4C 8B E8 44 8B B8")), 0x1, 0x0));

		if (!GetUserCmdBase)
			return nullptr;

		static void* off_181D9AAA0 = *reinterpret_cast<void**>(MEM::ResolveRelativeAddress(MEM::FindPattern(CLIENT_DLL, ("48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B CF 4C 8B E8")), 0x3, 0x7));

		if (!off_181D9AAA0)
			return nullptr;

		void* pUserCmdBase = GetUserCmdBase(off_181D9AAA0, commandIndex);
		if (!pUserCmdBase)
			return nullptr;

		DWORD sequenceNumber = *reinterpret_cast<DWORD*>((uintptr_t)pUserCmdBase + 0x5C00);
		static CUserCmd* (CS_FASTCALL * GetUserCmd)(void* pController, DWORD sequenceNumber) = reinterpret_cast<decltype(GetUserCmd)>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, ("E8 ? ? ? ? 48 8B 0D ? ? ? ? 45 33 E4 48 89 44 24")), 0x1, 0x0));

		if (!GetUserCmd)
			return nullptr;

		//CUserCmd* pUserCmd = GetUserCmd(controller, sequenceNumber);
		//if (!pUserCmd)
		//	return nullptr;

		//if (!pUserCmd->m_csgoUserCmd.base().has_viewangles())
		//{
		//	pUserCmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->set_x(0);
		//	pUserCmd->m_csgoUserCmd.mutable_base()->mutable_viewangles()->set_y(0);
		//}

		return GetUserCmd(controller, sequenceNumber);
	}

	void SetViewAngle(QAngle_t& angView)
	{
		// @ida: this got called before GetMatricesForView
		using fnSetViewAngle = std::int64_t(CS_FASTCALL*)(void*, std::int32_t, QAngle_t&);
		static auto oSetViewAngle = reinterpret_cast<fnSetViewAngle>(MEM::FindPattern(CLIENT_DLL, CXOR("85 D2 75 3F 48")));
//
//#ifdef CS_PARANOID
//		CS_ASSERT(oSetViewAngle != nullptr);
//#endif

		oSetViewAngle(this, 0, std::ref(angView));
	}

	QAngle_t GetViewAngles()
	{
		using fnGetViewAngles = std::int64_t(CS_FASTCALL*)(CCSGOInput*, std::int32_t);
		static auto oGetViewAngles = reinterpret_cast<fnGetViewAngles>(MEM::FindPattern(CLIENT_DLL, CXOR("4C 8B C1 85 D2 74 08 48 8D 05 ? ? ? ? C3")));

		return *reinterpret_cast<QAngle_t*>(oGetViewAngles(this, 0));
	}
};