/*

Offsets.hpp (Offsets utilized by OVERD0SE)
Converted from pattern scanning to hardcoded offsets system

Based on Fusion-CS2's offset system and cs2-dumper outputs
Dumper: https://github.com/a2x/cs2-dumper

Last updated: cs2-dumper outputs (current)

*/

#pragma once

#include <cstddef>

namespace Offsets {
	// ============================================================================
	// Core engine pointers (from offsets.json)
	// These change almost every update - update from cs2-dumper/output/offsets.json
	// ============================================================================
	
	// client.dll offsets
	constexpr std::ptrdiff_t dwCSGOInput = 0x1E26080;			// 31623168
	constexpr std::ptrdiff_t dwEntityList = 0x1D03BB0;			// 30410384
	constexpr std::ptrdiff_t dwGameEntitySystem = 0x1FA3660;	// 33181664
	constexpr std::ptrdiff_t dwGameRules = 0x1E1C480;			// 31579008
	constexpr std::ptrdiff_t dwGlobalVars = 0x1BD0DD0;			// 29167184
	constexpr std::ptrdiff_t dwGlowManager = 0x1E19228;			// 31566360
	constexpr std::ptrdiff_t dwLocalPlayerController = 0x1E0C448;	// 31499080
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1BDB010;		// 29211408
	constexpr std::ptrdiff_t dwPlantedC4 = 0x1E21878;			// 31601288
	constexpr std::ptrdiff_t dwPrediction = 0x1BDAED0;			// 29211200
	constexpr std::ptrdiff_t dwSensitivity = 0x1E19DE8;			// 31569016
	constexpr std::ptrdiff_t dwViewAngles = 0x1E28630;			// 31624880
	constexpr std::ptrdiff_t dwViewMatrix = 0x1E1E520;			// 31582496
	constexpr std::ptrdiff_t dwViewRender = 0x1E1F250;			// 31586160
	constexpr std::ptrdiff_t dwWeaponC4 = 0x1DB9080;			// 31176448
	
	// engine2.dll offsets
	constexpr std::ptrdiff_t dwBuildNumber = 0x5EF164;			// 6222820
	constexpr std::ptrdiff_t dwNetworkGameClient = 0x5EE370;	// 6218992
	constexpr std::ptrdiff_t dwWindowHeight = 0x8EF0A4;			// 9360932
	constexpr std::ptrdiff_t dwWindowWidth = 0x8EF0A0;			// 9360928
	
	// inputsystem.dll offsets
	constexpr std::ptrdiff_t dwInputSystem = 0x45A20;			// 285216

	// ============================================================================
	// Schema offsets (from client_dll.json classes)
	// These don't change as often but should be updated from cs2-dumper outputs
	// ============================================================================
	
	// CCSPlayerController
	constexpr std::ptrdiff_t m_hPlayerPawn = 0x8FC;				// 2300
	constexpr std::ptrdiff_t m_iszPlayerName = 0x6E8;			// 1768
	constexpr std::ptrdiff_t m_iCompTeammateColor = 0x838;		// 2104
	constexpr std::ptrdiff_t m_iPing = 0x8B0;					// 2224
	constexpr std::ptrdiff_t m_iScore = 0x8F4;					// 2292
	constexpr std::ptrdiff_t m_bPawnIsAlive = 0x900;			// 2304
	
	// C_BaseEntity
	constexpr std::ptrdiff_t m_iHealth = 0x34C;					// 844
	constexpr std::ptrdiff_t m_iMaxHealth = 0x348;				// 840
	constexpr std::ptrdiff_t m_iTeamNum = 0x3EB;				// 1003
	constexpr std::ptrdiff_t m_fFlags = 0x3F8;					// 1016
	constexpr std::ptrdiff_t m_iEFlags = 0x374;					// 884
	constexpr std::ptrdiff_t m_pGameSceneNode = 0x330;			// 816
	constexpr std::ptrdiff_t m_pCollision = 0x340;				// 832
	constexpr std::ptrdiff_t m_vecAbsVelocity = 0x3FC;			// 1020
	constexpr std::ptrdiff_t m_vecBaseVelocity = 0x510;			// 1296
	constexpr std::ptrdiff_t m_vecVelocity = 0x430;				// 1072
	constexpr std::ptrdiff_t m_hOwnerEntity = 0x43C;			// 1084
	constexpr std::ptrdiff_t m_MoveType = 0x52A;				// 1322
	constexpr std::ptrdiff_t m_nActualMoveType = 0x526;			// 1318
	constexpr std::ptrdiff_t m_lifeState = 0x354;				// 852
	constexpr std::ptrdiff_t m_bTakesDamage = 0x3F1;			// 1009
	constexpr std::ptrdiff_t m_flSimulationTime = 0x3B4;		// 948
	
	// C_BasePlayerPawn
	constexpr std::ptrdiff_t m_vOldOrigin = 0x15A0;				// 5536
	constexpr std::ptrdiff_t m_vecViewOffset = 0xD80;			// 3456
	constexpr std::ptrdiff_t m_pWeaponServices = 0x13F0;		// 5104
	constexpr std::ptrdiff_t m_pCameraServices = 0x13E8;		// 5096
	constexpr std::ptrdiff_t m_pMovementServices = 0x13F8;		// 5112
	
	// CGameSceneNode
	constexpr std::ptrdiff_t m_pSceneNode = 0x8;				// 8
	constexpr std::ptrdiff_t m_vecAbsOrigin = 0xD0;				// 208
	constexpr std::ptrdiff_t m_vecOrigin = 0x88;				// 136
	constexpr std::ptrdiff_t m_angRotation = 0xC0;				// 192
	constexpr std::ptrdiff_t m_angAbsRotation = 0xDC;			// 220
	constexpr std::ptrdiff_t m_bDormant = 0x10B;				// 267
	
	// C_BaseModelEntity
	constexpr std::ptrdiff_t m_modelState = 0x190;				// 400
	
	// CCSWeaponBaseVData
	constexpr std::ptrdiff_t m_WeaponType = 0x440;				// 1088
	constexpr std::ptrdiff_t m_WeaponCategory = 0x444;			// 1092
	constexpr std::ptrdiff_t m_bIsFullAuto = 0x734;				// 1844
	constexpr std::ptrdiff_t m_flCycleTime = 0x740;				// 1856
	constexpr std::ptrdiff_t m_flHeadshotMultiplier = 0x824;	// 2084
	constexpr std::ptrdiff_t m_flArmorRatio = 0x828;			// 2088
	constexpr std::ptrdiff_t m_flPenetration = 0x82C;			// 2092
	constexpr std::ptrdiff_t m_flRange = 0x830;					// 2096
	constexpr std::ptrdiff_t m_flRangeModifier = 0x834;			// 2100
	constexpr std::ptrdiff_t m_nDamage = 0x818;					// 2072
	constexpr std::ptrdiff_t m_flInaccuracyCrouch = 0x758;		// 1880
	constexpr std::ptrdiff_t m_flInaccuracyStand = 0x75C;		// 1884
	constexpr std::ptrdiff_t m_flMaxSpeed = 0x7E0;				// 2016
	constexpr std::ptrdiff_t m_flSpread = 0x760;				// 1888
	constexpr std::ptrdiff_t m_szName = 0x60;					// 96
	
	// C_CSPlayerPawn
	constexpr std::ptrdiff_t m_iShotsFired = 0x24FC;			// 9468
	constexpr std::ptrdiff_t m_flFlashDuration = 0x2510;		// 9488
	constexpr std::ptrdiff_t m_bIsScoped = 0x2548;				// 9544
	constexpr std::ptrdiff_t m_ArmorValue = 0x277C;				// 10108
	constexpr std::ptrdiff_t m_pClippingWeapon = 0x1408;		// 5128
	constexpr std::ptrdiff_t m_bWaitForNoAttack = 0x2524;		// 9508
	constexpr std::ptrdiff_t m_bIsDefusing = 0x24C8;			// 9416
	constexpr std::ptrdiff_t m_bIsGrabbingHostage = 0x24D0;		// 9424
	
	// C_CSWeaponBase
	constexpr std::ptrdiff_t m_iClip1 = 0x15B4;				// 5556
	constexpr std::ptrdiff_t m_iClip2 = 0x15B8;				// 5560
	constexpr std::ptrdiff_t m_pReserveAmmo = 0x15BC;			// 5564
	constexpr std::ptrdiff_t m_nNextPrimaryAttackTick = 0x1678;	// 5752
	constexpr std::ptrdiff_t m_flNextPrimaryAttackTickRatio = 0x167C;	// 5756
	
	// CPlayer_WeaponServices
	constexpr std::ptrdiff_t m_hActiveWeapon = 0x58;			// 88
	constexpr std::ptrdiff_t m_hMyWeapons = 0x60;				// 96
	
	// CPlayer_CameraServices  
	constexpr std::ptrdiff_t m_iFOV = 0x210;					// 528
	
	// CCSPlayerController_InGameMoneyServices
	constexpr std::ptrdiff_t m_iAccount = 0x40;					// 64
	constexpr std::ptrdiff_t m_iTotalCashSpent = 0x48;			// 72
	constexpr std::ptrdiff_t m_iCashSpentThisRound = 0x4C;		// 76
}
