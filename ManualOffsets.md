```text
// ————————————————————————————————————————————————————————————————————————————
// 1) Core engine pointers (from Fusion-CS2)
//    These are normally grabbed via signature scans in
//    cs2-internal/core/interfaces.cpp but can be hard-coded here:

dwEntityList         = 0x19F2488   // dumper name: dwEntityList
dwLocalPlayerPawn    = 0x1855CE8   // dumper name: dwLocalPlayerPawn
dwViewMatrix         = 0x1A54550   // dumper name: dwViewMatrix
dwViewAngles         = 0x1A5E650   // dumper name: dwViewAngles

// ————————————————————————————————————————————————————————————————————————————
// 2) SCHEMA_ADD_OFFSET entries in cs2-internal/sdk/entity.h
//    These three were already hard-coded in OVERD0SE:

CCSWeaponBaseVData->m_WeaponCategory  = 0x034C  // dumper: m_WeaponCategory
CCSWeaponBaseVData->m_bIsFullAuto     = 0x0D3C  // dumper: m_bIsFullAuto
CCSWeaponBaseVData->m_flCycleTime     = 0x0D44  // dumper: m_flCycleTime

// ————————————————————————————————————————————————————————————————————————————
// 3) All SCHEMA_ADD_FIELD/SCHEMA_ADD_FOREIGN_OFFSET fields
//    These are dynamically looked up at runtime in OVERD0SE.  To fully
//    convert to a static system, you must run a dumper (e.g. a2x/cs2-dumper)
//    and record the numeric offsets for each of the following fields:

// --- Weapon data (CCSWeaponBaseVData) ---
CCSWeaponBaseVData->m_szName               // dumper: m_szName
CCSWeaponBaseVData->m_WeaponType           // dumper: m_WeaponType
CCSWeaponBaseVData->m_flSpread             // dumper: m_flSpread
CCSWeaponBaseVData->m_flHeadshotMultiplier // dumper: m_flHeadshotMultiplier
CCSWeaponBaseVData->m_flArmorRatio         // dumper: m_flArmorRatio
CCSWeaponBaseVData->m_flPenetration        // dumper: m_flPenetration
CCSWeaponBaseVData->m_flRange              // dumper: m_flRange
CCSWeaponBaseVData->m_flRangeModifier      // dumper: m_flRangeModifier
CCSWeaponBaseVData->m_nDamage              // dumper: m_nDamage
CCSWeaponBaseVData->m_flInaccuracyCrouch   // dumper: m_flInaccuracyCrouch
CCSWeaponBaseVData->m_flInaccuracyStand    // dumper: m_flInaccuracyStand
CCSWeaponBaseVData->m_flMaxSpeed           // dumper: m_flMaxSpeed

// --- Entity fields (C_BaseEntity) ---
C_BaseEntity->m_pGameSceneNode   // dumper: m_pGameSceneNode
C_BaseEntity->m_pCollision       // dumper: m_pCollision
C_BaseEntity->m_iTeamNum         // dumper: m_iTeamNum
C_BaseEntity->m_hOwnerEntity     // dumper: m_hOwnerEntity
C_BaseEntity->m_vecBaseVelocity  // dumper: m_vecBaseVelocity
C_BaseEntity->m_vecAbsVelocity   // dumper: m_vecAbsVelocity
C_BaseEntity->m_flSimulationTime // dumper: m_flSimulationTime
C_BaseEntity->m_vecVelocity      // dumper: m_vecVelocity
C_BaseEntity->m_bTakesDamage     // dumper: m_bTakesDamage
C_BaseEntity->m_fFlags           // dumper: m_fFlags
C_BaseEntity->m_iEFlags          // dumper: m_iEFlags
C_BaseEntity->m_MoveType         // dumper: m_MoveType
C_BaseEntity->m_nActualMoveType  // dumper: m_nActualMoveType
C_BaseEntity->m_lifeState        // dumper: m_lifeState
C_BaseEntity->m_iHealth          // dumper: m_iHealth
C_BaseEntity->m_iMaxHealth       // dumper: m_iMaxHealth
C_BaseEntity->m_flWaterLevel     // dumper: m_flWaterLevel

// --- Other commonly used fields (example names) ---
m_hPlayerPawn     // dumper: m_hPlayerPawn
m_iszPlayerName   // dumper: m_iszPlayerName
m_vOldOrigin      // dumper: m_vOldOrigin
m_pSceneNode      // dumper: m_pSceneNode
m_modelState      // dumper: m_modelState
m_vecViewOffset   // dumper: m_vecViewOffset
```