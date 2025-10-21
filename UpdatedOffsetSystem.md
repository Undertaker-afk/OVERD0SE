# OVERD0SE - Updated Hardcoded Offset System

## Overview

OVERD0SE has been updated to use a **hardcoded offset system** similar to Fusion-CS2, replacing the previous pattern/signature scanning approach. This change provides:

- ✅ **Faster initialization** - No runtime pattern scanning needed
- ✅ **More reliable** - Direct memory access without signature failures
- ✅ **Easier updates** - Simply update offsets from cs2-dumper when game updates
- ✅ **Better maintainability** - All offsets centralized in one file

## How It Works

### Old System (Pattern Scanning)
```cpp
// Old approach - runtime pattern scanning
Input = *reinterpret_cast<CCSGOInput**>(
    MEM::ResolveRelativeAddress(
        MEM::FindPattern(CLIENT_DLL, "48 8B 0D ? ? ? ? 4C 8B C6 8B 10 E8"), 
        0x3, 0x7
    )
);
```

### New System (Hardcoded Offsets)
```cpp
// New approach - hardcoded offsets from cs2-dumper
const void* hClientDll = MEM::GetModuleBaseHandle(CLIENT_DLL);
Input = *reinterpret_cast<CCSGOInput**>(
    reinterpret_cast<std::uintptr_t>(hClientDll) + Offsets::dwCSGOInput
);
```

## Offset Sources

All offsets are obtained from the **cs2-dumper** project outputs:
- **Core offsets**: `cs2-dumper/output/offsets.json`
- **Schema offsets**: `cs2-dumper/output/client_dll.json`

## File Structure

### Core Offsets File
**Location**: `cs2-internal/core/Offsets.hpp`

This file contains all hardcoded offsets organized into categories:

1. **Core Engine Pointers** (from `offsets.json`)
   - dwEntityList, dwLocalPlayerPawn, dwViewMatrix, dwViewAngles, etc.
   - These change almost every game update

2. **Schema Offsets** (from `client_dll.json` classes)
   - Entity field offsets (m_iHealth, m_iTeamNum, etc.)
   - These change less frequently

## Complete Offset List

### Client.dll Core Offsets

| Offset Name | Value (Hex) | Value (Dec) | Source | Update Frequency |
|------------|-------------|-------------|--------|------------------|
| `dwCSGOInput` | `0x1E26080` | 31623168 | offsets.json | Almost every update |
| `dwEntityList` | `0x1D03BB0` | 30410384 | offsets.json | Almost every update |
| `dwGameEntitySystem` | `0x1FA3660` | 33181664 | offsets.json | Almost every update |
| `dwGameRules` | `0x1E1C480` | 31579008 | offsets.json | Almost every update |
| `dwGlobalVars` | `0x1BD0DD0` | 29167184 | offsets.json | Almost every update |
| `dwGlowManager` | `0x1E19228` | 31566360 | offsets.json | Almost every update |
| `dwLocalPlayerController` | `0x1E0C448` | 31499080 | offsets.json | Almost every update |
| `dwLocalPlayerPawn` | `0x1BDB010` | 29211408 | offsets.json | Almost every update |
| `dwPlantedC4` | `0x1E21878` | 31601288 | offsets.json | Almost every update |
| `dwViewAngles` | `0x1E28630` | 31624880 | offsets.json | Almost every update |
| `dwViewMatrix` | `0x1E1E520` | 31582496 | offsets.json | Almost every update |

### Schema Offsets (C_BaseEntity)

| Field Name | Offset (Hex) | Offset (Dec) | Description |
|-----------|--------------|--------------|-------------|
| `m_iHealth` | `0x34C` | 844 | Entity health |
| `m_iMaxHealth` | `0x348` | 840 | Maximum health |
| `m_iTeamNum` | `0x3EB` | 1003 | Team number |
| `m_fFlags` | `0x3F8` | 1016 | Entity flags |
| `m_pGameSceneNode` | `0x330` | 816 | Game scene node pointer |
| `m_vecAbsVelocity` | `0x3FC` | 1020 | Absolute velocity |
| `m_vecVelocity` | `0x430` | 1072 | Velocity vector |

### Schema Offsets (CCSPlayerController)

| Field Name | Offset (Hex) | Offset (Dec) | Description |
|-----------|--------------|--------------|-------------|
| `m_hPlayerPawn` | `0x8FC` | 2300 | Player pawn handle |
| `m_iszPlayerName` | `0x6E8` | 1768 | Player name |
| `m_iPing` | `0x8B0` | 2224 | Player ping |
| `m_bPawnIsAlive` | `0x900` | 2304 | Is pawn alive flag |

### Schema Offsets (C_CSPlayerPawn)

| Field Name | Offset (Hex) | Offset (Dec) | Description |
|-----------|--------------|--------------|-------------|
| `m_iShotsFired` | `0x24FC` | 9468 | Shots fired counter |
| `m_flFlashDuration` | `0x2510` | 9488 | Flash duration |
| `m_bIsScoped` | `0x2548` | 9544 | Is scoped flag |
| `m_ArmorValue` | `0x277C` | 10108 | Armor value |
| `m_vecViewOffset` | `0xD80` | 3456 | View offset |

### Schema Offsets (CCSWeaponBaseVData)

| Field Name | Offset (Hex) | Offset (Dec) | Description |
|-----------|--------------|--------------|-------------|
| `m_WeaponType` | `0x440` | 1088 | Weapon type |
| `m_WeaponCategory` | `0x444` | 1092 | Weapon category |
| `m_bIsFullAuto` | `0x734` | 1844 | Is full auto |
| `m_flCycleTime` | `0x740` | 1856 | Cycle time |
| `m_flHeadshotMultiplier` | `0x824` | 2084 | Headshot multiplier |
| `m_flArmorRatio` | `0x828` | 2088 | Armor penetration ratio |
| `m_nDamage` | `0x818` | 2072 | Base damage |

### Schema Offsets (CGameSceneNode)

| Field Name | Offset (Hex) | Offset (Dec) | Description |
|-----------|--------------|--------------|-------------|
| `m_vecAbsOrigin` | `0xD0` | 208 | Absolute origin |
| `m_vecOrigin` | `0x88` | 136 | Origin vector |
| `m_angRotation` | `0xC0` | 192 | Rotation angle |
| `m_bDormant` | `0x10B` | 267 | Is dormant flag |

## Usage Examples

### Accessing Core Offsets
```cpp
#include "core/Offsets.hpp"

// Get client.dll base
const void* hClientDll = MEM::GetModuleBaseHandle(CLIENT_DLL);

// Access dwEntityList
auto entityList = *reinterpret_cast<CGameEntitySystem**>(
    reinterpret_cast<std::uintptr_t>(hClientDll) + Offsets::dwEntityList
);

// Access dwLocalPlayerPawn
auto localPawn = *reinterpret_cast<C_CSPlayerPawn**>(
    reinterpret_cast<std::uintptr_t>(hClientDll) + Offsets::dwLocalPlayerPawn
);
```

### Accessing Entity Fields
```cpp
// Using schema offsets (already integrated in entity classes)
C_BaseEntity* entity = /* ... */;
int health = *reinterpret_cast<int*>(
    reinterpret_cast<std::uintptr_t>(entity) + Offsets::m_iHealth
);
```

## How to Update Offsets

When CS2 updates and breaks the cheat:

1. **Update cs2-dumper outputs**:
   ```bash
   cd cs2-dumper
   cargo run
   ```

2. **Open generated files**:
   - `cs2-dumper/output/offsets.json` - Core offsets
   - `cs2-dumper/output/client_dll.json` - Schema offsets

3. **Update `cs2-internal/core/Offsets.hpp`**:
   - Replace core offset values from `offsets.json`
   - Update schema offsets from `client_dll.json` classes
   - Keep the same structure and naming

4. **Rebuild the project**

## Files Modified

- ✅ **Created**: `cs2-internal/core/Offsets.hpp` - Central offset storage
- ✅ **Modified**: `cs2-internal/core/interfaces.h` - Added Offsets.hpp include
- ✅ **Modified**: `cs2-internal/core/interfaces.cpp` - Replaced pattern scans with hardcoded offsets
- ✅ **Updated**: `OVERD0SE_Offsets.md` - Updated documentation

## Benefits vs Pattern Scanning

| Aspect | Pattern Scanning (Old) | Hardcoded Offsets (New) |
|--------|----------------------|------------------------|
| **Speed** | Slow (scans memory at startup) | Fast (direct access) |
| **Reliability** | Can fail if patterns change | Always works if offsets are correct |
| **Maintenance** | Need to find new patterns after updates | Copy values from cs2-dumper |
| **Code Size** | Larger (pattern strings) | Smaller (just numbers) |
| **Update Time** | Medium (find patterns, test) | Fast (copy from dumper) |

## Compatibility

This system is compatible with:
- ✅ CS2 current version (offsets from latest cs2-dumper run)
- ✅ Works alongside interface capture system
- ✅ Can be mixed with pattern scanning for items not in cs2-dumper

## Notes

- Some features still use pattern scanning where cs2-dumper doesn't provide offsets
- The interface capture system (I::SchemaSystem, I::InputSystem, etc.) remains unchanged
- Schema system queries still work normally for dynamic class discovery
- Offsets should be updated after every major CS2 game update

## Reference

- **cs2-dumper**: https://github.com/a2x/cs2-dumper
- **Fusion-CS2**: Inspiration for this offset system structure
- **OVERD0SE**: https://github.com/Bloodysharp/OVERD0SE
