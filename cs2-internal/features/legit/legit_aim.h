#pragma once

#include <iostream>
#include <memory>
#include <deque>
#include <array>
#include <unordered_map>

class CUserCmd;
class CBaseUserCmdPB;
class CCSGOInputHistoryEntryPB;

class CCSPlayerController;
class C_CSPlayerPawn;

struct QAngle_t;

class c_legit_bot {

public:
	void on_create_move(CBaseUserCmdPB* pBaseCmd, CCSPlayerController* pLocalController, C_CSPlayerPawn* pLocalPawn);
	void process_assist(CBaseUserCmdPB* pUserCmd, C_CSPlayerPawn* pLocalPawn, CCSPlayerController* pLocalController);
	void drawfov();
};

const auto g_legit_bot = std::make_unique<c_legit_bot>();
