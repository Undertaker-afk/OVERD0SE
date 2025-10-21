```text
// ————————————————————————————————————————————————————————————————————————————
// 1) Core engine pointers (NOW USING HARDCODED OFFSETS - No more pattern scanning!)
//    ---------------------------------------------------------------------------
//    IMPORTANT: Pattern scanning has been REPLACED with hardcoded offsets
//    from cs2-dumper for better performance and reliability.

// File: cs2-internal/core/Offsets.hpp (NEW)
// File: cs2-internal/core/interfaces.cpp (MODIFIED)
// Source: cs2-dumper/output/offsets.json

// ===== REPLACED PATTERN SCANS WITH HARDCODED OFFSETS =====

// OLD: Pattern: "48 8D 0D ? ? ? ? 48 89 4D F8 E8 ? ? ? ? 48 8B D0"
// NEW: Offset::dwSchemaSystem (accessed via interface capture - no offset needed)

// OLD: Pattern: "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 05 ? ? ? ? 48 89 05 ? ? ? ?"
// NEW: Offsets::dwEntityList = 0x1D03BB0 (30410384)

// OLD: Pattern: "48 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? FF 50 ? 48 85 C0 74 ? 48 8B 08"
// NEW: Offsets::dwGameRules = 0x1E1C480 (31579008)

// OLD: Pattern: "48 8B 05 ? ? ? ? 48 85 C0 74 ? 4C 8D 05"
// NEW: Offsets::dwPlantedC4 = 0x1E21878 (31601288)

// OLD: Pattern: "48 8B 05 ? ? ? ? 48 8B D9 8B 81"
// NEW: Offsets::dwInputSystem (in inputsystem.dll) = 0x45A20 (285216)

// OLD: Pattern: "48 8D 05 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 33 D2"
// NEW: Offsets::dwLocalPlayerController = 0x1E0C448 (31499080)

// OLD: Pattern: "48 8B 05 ? ? ? ? 48 85 C0 74 05 48 8B 08 B8"
// NEW: Offsets::dwGlobalVars = 0x1BD0DD0 (29167184)
// USAGE: GlobalVars = *reinterpret_cast<IGlobalVars**>(clientBase + Offsets::dwGlobalVars);

// OLD: Pattern: "48 83 EC 28 8B 05 ? ? ? ? 8B 0C 88"
// NEW: Offsets::dwViewMatrix = 0x1E1E520 (31582496)

// OLD: Pattern: "4C 8D 05 ? ? ? ? 48 8D 15 ? ? ? ? 48 8D 0D ? ? ? ? E8"
// NEW: Offsets::dwViewAngles = 0x1E28630 (31624880)

// OLD: Pattern: "48 8B 0D ? ? ? ? 4C 8B C6 8B 10 E8"
// NEW: Offsets::dwCSGOInput = 0x1E26080 (31623168)
// USAGE: Input = *reinterpret_cast<CCSGOInput**>(clientBase + Offsets::dwCSGOInput);

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