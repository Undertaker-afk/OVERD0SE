```text
// ————————————————————————————————————————————————————————————————————————————
// 1) Core engine pointers (signature scans in cs2-internal/core/interfaces.cpp)
//    ---------------------------------------------------------------------------
//    All of these are declared via CSigScan objects and initialized in
//    H::Setup() by calling their .Scan() methods against client.dll or engine.dll.

// File: cs2-internal/core/interfaces.cpp
// URL:  https://github.com/Bloodysharp/OVERD0SE/blob/main/cs2-internal/core/interfaces.cpp

// Location ~line 45: signature for SchemaSystem interface
Pattern: "48 8D 0D ? ? ? ? 48 89 4D F8 E8 ? ? ? ? 48 8B D0"
// Scans for: I::SchemaSystem (dumper: dwSchemaSystem)

// Location ~line 60: signature for GameEntitySystem
Pattern: "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 05 ? ? ? ? 48 89 05 ? ? ? ?"
// Scans for: I::EntityList or GameEntitySystem (dumper: dwEntityList)

// Location ~line 75: signature for GameRules
Pattern: "48 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? FF 50 ? 48 85 C0 74 ? 48 8B 08"
// Scans for: I::GameRules (dumper: dwGameRules)

// Location ~line 90: signature for PlantedC4
Pattern: "48 8B 05 ? ? ? ? 48 85 C0 74 ? 4C 8D 05"
// Scans for: I::PlantedC4 (dumper: dwPlantedC4)

// Location ~line 105: signature for InputSystem
Pattern: "48 8B 05 ? ? ? ? 48 8B D9 8B 81"
// Scans for: I::InputSystem (dumper: dwInputSystem)

// Location ~line 120: signature for LocalPlayerController
Pattern: "48 8D 05 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 33 D2"
// Scans for: I::LocalPlayerController (dumper: dwLocalPlayerController)

// Location ~line 135: signature for GlobalVars
Pattern: "48 8B 05 ? ? ? ? 48 85 C0 74 05 48 8B 08 B8"
// Scans for: I::GlobalVars (dumper: dwGlobalVars)

// Location ~line 150: signature for ViewMatrix
Pattern: "48 83 EC 28 8B 05 ? ? ? ? 8B 0C 88"
// Scans for: dwViewMatrix

// Location ~line 165: signature for ViewAngles
Pattern: "4C 8D 05 ? ? ? ? 48 8D 15 ? ? ? ? 48 8D 0D ? ? ? ? E8"
// Scans for: dwViewAngles

// ————————————————————————————————————————————————————————————————————————————
// 2) SCHEMA_ADD_OFFSET entries in cs2-internal/sdk/entity.h
//    ---------------------------------------------
//    These are already hard-coded and require no signature scan:

CCSWeaponBaseVData->m_WeaponCategory  = 0x034C  // dumper: m_WeaponCategory
CCSWeaponBaseVData->m_bIsFullAuto     = 0x0D3C  // dumper: m_bIsFullAuto
CCSWeaponBaseVData->m_flCycleTime     = 0x0D44  // dumper: m_flCycleTime

// ————————————————————————————————————————————————————————————————————————————
// 3) All dynamic-schema fields (SCHEMA_ADD_FIELD / SCHEMA_ADD_FOREIGN_OFFSET)
//    ---------------------------------------------------------------------------
//    These use the Schema system (no pattern). To replace them, you must
//    dump each field’s numeric offset (with a2x/cs2-dumper) and hardcode here:

// --- Weapon data (CCSWeaponBaseVData) ---
m_szName               // dumper: m_szName
m_WeaponType           // dumper: m_WeaponType
m_flSpread             // dumper: m_flSpread
m_flHeadshotMultiplier // dumper: m_flHeadshotMultiplier
m_flArmorRatio         // dumper: m_flArmorRatio
m_flPenetration        // dumper: m_flPenetration
m_flRange              // dumper: m_flRange
m_flRangeModifier      // dumper: m_flRangeModifier
m_nDamage              // dumper: m_nDamage
m_flInaccuracyCrouch   // dumper: m_flInaccuracyCrouch
m_flInaccuracyStand    // dumper: m_flInaccuracyStand
m_flMaxSpeed           // dumper: m_flMaxSpeed

// --- Entity fields (C_BaseEntity) ---
m_pGameSceneNode     // dumper: m_pGameSceneNode
m_pCollision         // dumper: m_pCollision
m_iTeamNum           // dumper: m_iTeamNum
m_hOwnerEntity       // dumper: m_hOwnerEntity
m_vecBaseVelocity    // dumper: m_vecBaseVelocity
m_vecAbsVelocity     // dumper: m_vecAbsVelocity
m_flSimulationTime   // dumper: m_flSimulationTime
m_vecVelocity        // dumper: m_vecVelocity
m_bTakesDamage       // dumper: m_bTakesDamage
m_fFlags             // dumper: m_fFlags
m_iEFlags            // dumper: m_iEFlags
m_MoveType           // dumper: m_MoveType
m_nActualMoveType    // dumper: m_nActualMoveType
m_lifeState          // dumper: m_lifeState
m_iHealth            // dumper: m_iHealth
m_iMaxHealth         // dumper: m_iMaxHealth
m_flWaterLevel       // dumper: m_flWaterLevel

// You’ll need to fill in exact hex offsets for each of the above by running
// a2x/cs2-dumper (or similar) and copying the values into your new Offsets.hpp.
```