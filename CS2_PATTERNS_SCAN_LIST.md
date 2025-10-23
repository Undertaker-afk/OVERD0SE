# CS2 Internal - Core & Interface Pattern Scan Reference

This document catalogs the pattern scans currently used in the cs2-internal project's `core/` and `sdk/interfaces/` directories, organized by file location and linked to their corresponding offsets from cs2-dumper where applicable.

> **Scope:** Only pattern scans that live in `cs2-internal/core` and `cs2-internal/sdk/interfaces` are covered here. Other modules (e.g. `sdk/entity.cpp`, `sdk/datatypes`, `features`, `utilities`) contain additional patterns that are outside this document's present scope and will be documented separately.

## Table of Contents
- [core/interfaces.cpp](#coreinterfacescpp)
- [core/hooks.cpp](#corehookscpp)
- [sdk/interfaces/events.cpp](#sdkinterfaceseventscpp)
- [sdk/interfaces/cgameentitysystem.h](#sdkinterfacescgameentitysystemh)
- [sdk/interfaces/ccsgoinput.h](#sdkinterfacesccsgoinputh)
- [sdk/interfaces/imaterialsystem.h](#sdkinterfacesimaterialsystemh)
- [sdk/interfaces/сgameparticlemanagersystem.hpp](#sdkinterfacesсgameparticlemanagersystemhpp)
- [sdk/interfaces/iengineclient.h](#sdkinterfacesinengineclienth)
- [sdk/interfaces/itrace.h](#sdkinterfacesitraceh)

---

## core/interfaces.cpp

### 1. CCSGOInput (Get Input Function)
- **Pattern:** `48 8B 0D ? ? ? ? 4C 8B C6 8B 10 E8`
- **File:** `cs2-internal/core/interfaces.cpp:69`
- **Offset Name:** `get_func` / `I::Get_input()`
- **CS2-Dumper Offset:** `dwCSGOInput` (client.dll: 31623168 / 0x1E25700)
- **Description:** Returns CCSGOInput pointer

### 2. i_trace (Trace Interface)
- **Pattern:** `4C 8B 35 ? ? ? ? 24`
- **File:** `cs2-internal/core/interfaces.cpp:171`
- **Offset Name:** `Trace`
- **CS2-Dumper Offset:** N/A (Interface pointer)
- **Description:** Trace system interface for raycasting

### 3. ISwapChainDx11 (SwapChain)
- **Pattern:** `66 0F 7F 0D 83 C9 43 ? 48 8B F7 66 0F 7F 05 88 C9 43 ?`
- **File:** `cs2-internal/core/interfaces.cpp:177`
- **Module:** `rendersystem.dll`
- **Offset Name:** `SwapChain`
- **CS2-Dumper Offset:** N/A (Renderer interface)
- **Description:** DirectX 11 swap chain interface

### 4. CCSGOInput (Input Pointer)
- **Pattern:** `48 8B 0D ? ? ? ? 4C 8B C6 8B 10 E8`
- **File:** `cs2-internal/core/interfaces.cpp:195`
- **Offset Name:** `Input`
- **CS2-Dumper Offset:** `dwCSGOInput` (client.dll: 31623168 / 0x1E25700)
- **Description:** Main input system pointer

### 5. CGameParticleManagerSystem
- **Pattern:** `49 63 81 ?? ?? ?? ?? 85 C0`
- **File:** `cs2-internal/core/interfaces.cpp:197`
- **Offset Name:** `MGameParticleManagerSystem`
- **CS2-Dumper Offset:** N/A (Game system)
- **Description:** Particle manager system

### 6. IGlobalVars (Global Variables)
- **Pattern:** `48 8B 05 ?? ?? ?? ?? 44 8B B7 ?? ?? ?? ?? 8B 70 04 B8 ?? ?? ?? ??`
- **File:** `cs2-internal/core/interfaces.cpp:200`
- **Offset Name:** `GlobalVars`
- **CS2-Dumper Offset:** `dwGlobalVars` (client.dll: 29167184 / 0x1BD3550)
- **Description:** Global game variables including tick count, real time, etc.

### 7. CBaseGameSystemFactory (Factory List)
- **Pattern:** `48 8B 3D ? ? ? ? 48 85 FF 0F 84 ? ? ? ? BE`
- **File:** `cs2-internal/core/interfaces.cpp:232`
- **Offset Name:** `pFactoryList`
- **CS2-Dumper Offset:** N/A (Game system factory)
- **Description:** Factory list for game systems (Inventory, GC Client, etc.)

---

## core/hooks.cpp

### 8. GetHitboxSet (CSigScan)
- **Pattern:** `E8 ? ? ? ? 48 85 C0 0F 85 ? ? ? ? 44 8D 48 07`
- **Alternative:** `41 8B D6 E8 ? ? ? ? 4C 8B F8`
- **File:** `cs2-internal/core/hooks.cpp:63-67`
- **Offset Name:** `C_BaseEntity::GetHitboxSet`
- **CS2-Dumper Offset:** N/A (Virtual function)
- **Description:** Gets hitbox set for entity

### 9. GetBoneName (CSigScan)
- **Pattern:** `48 8B CE E8 ? ? ? ? 48 8B 0F`
- **File:** `cs2-internal/core/hooks.cpp:68-71`
- **Offset Name:** `CModel::GetBoneName`
- **CS2-Dumper Offset:** N/A (Model function)
- **Description:** Gets bone name from model

### 10. HitboxToWorldTransforms (CSigScan)
- **Pattern:** `E8 ? ? ? ? 4C 8B A3`
- **File:** `cs2-internal/core/hooks.cpp:72-75`
- **Offset Name:** `C_BaseEntity::HitboxToWorldTransforms`
- **CS2-Dumper Offset:** N/A (Transform function)
- **Description:** Converts hitbox coordinates to world space

### 11. ComputeHitboxSurroundingBox (CSigScan)
- **Pattern:** `E9 ? ? ? ? F6 43 5B FD`
- **File:** `cs2-internal/core/hooks.cpp:77-80`
- **Offset Name:** `C_BaseEntity::ComputeHitboxSurroundingBox`
- **CS2-Dumper Offset:** N/A (Hitbox calculation)
- **Description:** Computes bounding box around hitboxes

### 12. CreateMove Hook
- **Pattern:** `48 8B C4 4C 89 40 18 48 89 48 08 55 53 57`
- **File:** `cs2-internal/core/hooks.cpp:121`
- **Offset Name:** `CreateMove`
- **CS2-Dumper Offset:** N/A (Hook function)
- **Description:** User command creation hook

### 13. DrawGlow Hook
- **Pattern:** `40 53 48 83 EC ? 48 8B 54 24`
- **File:** `cs2-internal/core/hooks.cpp:122`
- **Offset Name:** `DrawGlow`
- **CS2-Dumper Offset:** N/A (Rendering function)
- **Description:** Glow rendering hook

### 14. GetMatrixForView Hook
- **Pattern:** `40 53 48 81 EC ? ? ? ? 49 8B C1`
- **File:** `cs2-internal/core/hooks.cpp:123`
- **Offset Name:** `GetMatrixForView`
- **CS2-Dumper Offset:** N/A (View matrix function)
- **Description:** View matrix calculation hook

### 15. SmokeUpdate Hook
- **Pattern:** `40 56 57 41 56 48 83 EC ? 8B 35`
- **File:** `cs2-internal/core/hooks.cpp:124`
- **Offset Name:** `SmokeModulatione`
- **CS2-Dumper Offset:** N/A (Smoke rendering)
- **Description:** Smoke modulation/update hook

### 16. Pvs Hook
- **Pattern:** `40 53 48 83 EC ? 48 8B 01 48 8B D9 FF 90 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 43`
- **File:** `cs2-internal/core/hooks.cpp:125`
- **Offset Name:** `pvs`
- **CS2-Dumper Offset:** N/A (PVS check)
- **Description:** Potentially Visible Set check

### 17. OverrideView Hook
- **Pattern:** `48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 48 8B FA E8`
- **File:** `cs2-internal/core/hooks.cpp:126`
- **Offset Name:** `OverrideView`
- **CS2-Dumper Offset:** N/A (View override)
- **Description:** Camera view override hook

### 18. ShouldRender Hook
- **Pattern:** `40 53 48 83 EC ? 48 8B D9 83 FA ? 75 ? 48 8B 0D ? ? ? ? 48 8D 54 24 ? 48 8B 01 FF 90 ? ? ? ? 8B 10`
- **File:** `cs2-internal/core/hooks.cpp:127`
- **Offset Name:** `ShouldRender`
- **CS2-Dumper Offset:** N/A (Render check)
- **Description:** Entity render check hook

### 19. RenderLegsFPV Hook
- **Pattern:** `48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 01`
- **File:** `cs2-internal/core/hooks.cpp:128`
- **Offset Name:** `RenderLegsFPV`
- **CS2-Dumper Offset:** N/A (First person view)
- **Description:** First-person view legs rendering

### 20. HandleGameEvents Hook
- **Pattern:** `40 55 53 56 57 41 55 41 57 48 8D 6C 24`
- **File:** `cs2-internal/core/hooks.cpp:129`
- **Offset Name:** `GameEvents`
- **CS2-Dumper Offset:** N/A (Event system)
- **Description:** Game event handler hook

### 21. HudUpdate Hook
- **Pattern:** `40 55 53 56 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 65 48 8B 04 25`
- **File:** `cs2-internal/core/hooks.cpp:130`
- **Offset Name:** `HudUpdate`
- **CS2-Dumper Offset:** N/A (HUD system)
- **Description:** HUD update hook

### 22. DrawViewModel Hook
- **Pattern:** `48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8B EC 48 83 EC ? 4D 8B E0`
- **File:** `cs2-internal/core/hooks.cpp:131`
- **Offset Name:** `DrawViewModel`
- **CS2-Dumper Offset:** N/A (Viewmodel rendering)
- **Description:** Viewmodel drawing hook

### 23. LevelInit Hook
- **Pattern:** `48 89 5C 24 ? 56 48 83 EC ? 48 8B 0D ? ? ? ? 48 8B F2`
- **File:** `cs2-internal/core/hooks.cpp:132`
- **Offset Name:** `LevelInit`
- **CS2-Dumper Offset:** N/A (Level initialization)
- **Description:** Level initialization hook

### 24. DrawSceneObject Hook
- **Pattern:** `48 89 54 24 ? 55 57 41 55 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 49 63 F9`
- **File:** `cs2-internal/core/hooks.cpp:133`
- **Module:** `scenesystem.dll`
- **Offset Name:** `DrawSceneObject`
- **CS2-Dumper Offset:** N/A (Scene rendering)
- **Description:** Scene object drawing hook

### 25. LightingModulate Hook
- **Pattern:** `48 89 54 24 ? 53 41 56 41 57`
- **File:** `cs2-internal/core/hooks.cpp:134`
- **Module:** `scenesystem.dll`
- **Offset Name:** `LightingModulate`
- **CS2-Dumper Offset:** N/A (Lighting system)
- **Description:** Lighting modulation hook

### 26. LevelShutdown Hook
- **Pattern:** `48 83 EC ? 48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 45 33 C9 45 33 C0 48 8B 01 FF 50 ? 48 85 C0 74 ? 48 8B 0D ? ? ? ? 48 8B D0 4C 8B 01 41 FF 50 ? 48 83 C4`
- **File:** `cs2-internal/core/hooks.cpp:135`
- **Offset Name:** `LevelShutdown`
- **CS2-Dumper Offset:** N/A (Level cleanup)
- **Description:** Level shutdown hook

### 27. DrawObject Hook
- **Pattern:** `48 8B C4 48 89 50 10 53 41 55 41 56 48 81 EC ? ? ? ? 4D 63 F1`
- **File:** `cs2-internal/core/hooks.cpp:136`
- **Module:** `scenesystem.dll`
- **Offset Name:** `DrawObject`
- **CS2-Dumper Offset:** N/A (Object drawing)
- **Description:** Object drawing hook

### 28. UpdateScens Hook
- **Pattern:** `48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 4C 8B F9`
- **File:** `cs2-internal/core/hooks.cpp:137`
- **Module:** `scenesystem.dll`
- **Offset Name:** `UpdateScens`
- **CS2-Dumper Offset:** N/A (Scene update)
- **Description:** Scene update hook

### 29. UpdatePostProcessing Hook
- **Pattern:** `48 89 5C 24 ? 57 48 83 EC ? 80 B9 ? ? ? ? ? 8B DA 48 8B F9`
- **File:** `cs2-internal/core/hooks.cpp:138`
- **Offset Name:** `UpdatePostProcessing`
- **CS2-Dumper Offset:** N/A (Post-processing)
- **Description:** Post-processing update hook

### 30. SetModel Hook
- **Pattern:** `40 53 48 83 EC 20 48 8B D9 4C 8B C2 48 8B 0D ? ? ? ? 48 8D 54 24`
- **File:** `cs2-internal/core/hooks.cpp:139`
- **Offset Name:** `SetModel`
- **CS2-Dumper Offset:** N/A (Model setter)
- **Description:** Model setting hook

### 31. CameraInput Hook
- **Pattern:** Virtual function at VTABLE::CLIENT::CAMERA
- **File:** `cs2-internal/core/hooks.cpp:142`
- **Offset Name:** `CameraInput`
- **CS2-Dumper Offset:** N/A (Camera input)
- **Description:** Camera input hook

### 32. FrameStageNotify Hook
- **Pattern:** `48 89 5C 24 10 56 48 83 EC 30 8B 05`
- **File:** `cs2-internal/core/hooks.cpp:144`
- **Offset Name:** `FrameStageNotify`
- **CS2-Dumper Offset:** N/A (Frame stage)
- **Description:** Frame stage notification hook

### 33. GlobalVars (Alternative)
- **Pattern:** `48 8B 0D ? ? ? ? 4C 8D 05 ? ? ? ? 48 85 D2`
- **File:** `cs2-internal/core/hooks.cpp:826`
- **Offset Name:** `GlobalVars`
- **CS2-Dumper Offset:** `dwGlobalVars` (client.dll: 29167184 / 0x1BD3550)
- **Description:** Alternative global variables pattern

---

## sdk/interfaces/events.cpp

### 34. IGameEvent::get_int
- **Pattern:** `48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 41 56 48 83 EC 30 48 8B 01 41 8B F0 4C 8B F1 41 B0 01 48 8D 4C 24 20 48 8B DA 48 8B 78`
- **File:** `cs2-internal/sdk/interfaces/events.cpp:6`
- **Offset Name:** `get_int`
- **CS2-Dumper Offset:** N/A (Game event function)
- **Description:** Gets integer value from game event

### 35. IGameEvent::GetFloat2
- **Pattern:** `48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 40 48 8B 01 48 8B F1 0F 29 74 24 30 48 8D 4C 24 20 0F 28 F2 48 8B DA 48 8B 78`
- **File:** `cs2-internal/sdk/interfaces/events.cpp:14`
- **Offset Name:** `GetFloat2`
- **CS2-Dumper Offset:** N/A (Game event function)
- **Description:** Gets float value from game event

### 36. IGameEvent::get_player_pawn_from_id
- **Pattern:** `48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 30 48 8B 01 48 8B F1 41 B0 01 48 8D 4C 24 20 48 8B FA 48 8B 98`
- **File:** `cs2-internal/sdk/interfaces/events.cpp:22`
- **Offset Name:** `get_player_pawn_from_id`
- **CS2-Dumper Offset:** N/A (Game event function)
- **Description:** Gets player pawn from event ID

### 37. IGameEvent::get_pointer_from_id (Part 1)
- **Pattern:** `48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 30 48 8B 01 48 8B F1 41 B0 01 48 8D 4C 24 20 48 8B FA 48 8B 98`
- **File:** `cs2-internal/sdk/interfaces/events.cpp:33`
- **Offset Name:** `get_pointer_from_id` (ID getter)
- **CS2-Dumper Offset:** N/A (Game event function)
- **Description:** Gets ID from event

### 38. IGameEvent::get_pointer_from_id (Part 2)
- **Pattern:** `E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 48 8B D8 48 85 C9`
- **File:** `cs2-internal/sdk/interfaces/events.cpp:44`
- **Offset Name:** `get_pointer_from_id` (Pointer converter)
- **CS2-Dumper Offset:** N/A (Game event function)
- **Description:** Converts ID to pointer

---

## sdk/interfaces/cgameentitysystem.h

### 39. CGameEntitySystem::GetEntityByIndex
- **Pattern:** `81 FA FE 7F 00 00 77 36`
- **File:** `cs2-internal/sdk/interfaces/cgameentitysystem.h:45`
- **Offset Name:** `GetBaseEntity`
- **CS2-Dumper Offset:** `dwGameEntitySystem` (client.dll: 33181664 / 0x1FA36E0)
- **Description:** Gets entity by index from entity system

---

## sdk/interfaces/ccsgoinput.h

### 40. CCSGOInput::GetArrayOfUserCmds
- **Pattern:** `48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B CF 4C 8B E8`
- **File:** `cs2-internal/sdk/interfaces/ccsgoinput.h:70`
- **Offset Name:** `GetArrayOfUserCmds`
- **CS2-Dumper Offset:** Related to `dwCSGOInput`
- **Description:** Gets user command array

### 41. CCSGOInput::GetCommandIndex
- **Pattern:** `E8 ? ? ? ? 8B 8D ? ? ? ? 8D 51`
- **File:** `cs2-internal/sdk/interfaces/ccsgoinput.h:81`
- **Offset Name:** `GetCommandIndex`
- **CS2-Dumper Offset:** N/A (Input function)
- **Description:** Gets current command index

### 42. CCSGOInput::GetUserCmdBase
- **Pattern:** `E8 ? ? ? ? 48 8B CF 4C 8B E8 44 8B B8`
- **File:** `cs2-internal/sdk/interfaces/ccsgoinput.h:93`
- **Offset Name:** `GetUserCmdBase`
- **CS2-Dumper Offset:** N/A (Input function)
- **Description:** Gets user command base

### 43. CCSGOInput::off_181D9AAA0
- **Pattern:** `48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B CF 4C 8B E8`
- **File:** `cs2-internal/sdk/interfaces/ccsgoinput.h:98`
- **Offset Name:** `off_181D9AAA0`
- **CS2-Dumper Offset:** N/A (Input pointer)
- **Description:** Input system offset

### 44. CCSGOInput::GetUserCmd
- **Pattern:** `E8 ? ? ? ? 48 8B 0D ? ? ? ? 45 33 E4 48 89 44 24`
- **File:** `cs2-internal/sdk/interfaces/ccsgoinput.h:108`
- **Offset Name:** `GetUserCmd`
- **CS2-Dumper Offset:** N/A (Input function)
- **Description:** Gets user command by sequence number

### 45. CCSGOInput::SetViewAngle
- **Pattern:** `85 D2 75 3F 48`
- **File:** `cs2-internal/sdk/interfaces/ccsgoinput.h:130`
- **Offset Name:** `SetViewAngle`
- **CS2-Dumper Offset:** Related to `dwViewAngles` (client.dll: 31624880 / 0x1E25CB0)
- **Description:** Sets view angles

### 46. CCSGOInput::GetViewAngles
- **Pattern:** `4C 8B C1 85 D2 74 08 48 8D 05 ? ? ? ? C3`
- **File:** `cs2-internal/sdk/interfaces/ccsgoinput.h:142`
- **Offset Name:** `GetViewAngles`
- **CS2-Dumper Offset:** Related to `dwViewAngles` (client.dll: 31624880 / 0x1E25CB0)
- **Description:** Gets current view angles

---

## sdk/interfaces/imaterialsystem.h

### 47. material_key_var_t::find_key
- **Pattern:** `48 89 5C 24 ? 57 48 81 EC ? ? ? ? 33 C0 8B DA`
- **File:** `cs2-internal/sdk/interfaces/imaterialsystem.h:38`
- **Module:** `particles.dll`
- **Offset Name:** `find_key_var`
- **CS2-Dumper Offset:** N/A (Material system)
- **Description:** Finds material key by name

### 48. material_data_t::set_shader_type
- **Pattern:** `48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 0F B6 01 45 0F B6 F9 8B 2A 4D 8B E0 4C 8B 72 ? 48 8B F9 C0 E8 ? 24 ? 3C ? 74 ? 41 B0 ? B2 ? E8 ? ? ? ? 0F B6 07 33 DB C0 E8 ? 24 ? 3C ? 75 ? 48 8B 77 ? EB ? 48 8B F3 4C 8D 44 24 ? C7 44 24 ? ? ? ? ? 48 8D 54 24 ? 89 6C 24 ? 48 8B CE 4C 89 74 24 ? E8 ? ? ? ? 8B D0 83 F8 ? 75 ? 45 33 C9 89 6C 24 ? 4C 8D 44 24 ? 4C 89 74 24 ? 48 8B D7 48 8B CE E8 ? ? ? ? 8B D0 0F B6 0F C0 E9 ? 80 E1 ? 80 F9 ? 75 ? 48 8B 4F ? EB ? 48 8B CB 8B 41 ? 85 C0 74 ? 48 8D 59 ? 83 F8 ? 76 ? 48 8B 1B 48 63 C2 4D 85 E4`
- **File:** `cs2-internal/sdk/interfaces/imaterialsystem.h:65`
- **Module:** `particles.dll`
- **Offset Name:** `SetMaterialShaderType`
- **CS2-Dumper Offset:** N/A (Material system)
- **Description:** Sets material shader type

### 49. material_data_t::set_material_function
- **Pattern:** `48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 0F B6 01 45 0F B6 F9 8B 2A 48 8B F9`
- **File:** `cs2-internal/sdk/interfaces/imaterialsystem.h:77`
- **Module:** `particles.dll`
- **Offset Name:** `SetMaterialFunction`
- **CS2-Dumper Offset:** N/A (Material system)
- **Description:** Sets material function parameter

---

## sdk/interfaces/сgameparticlemanagersystem.hpp

### 50. CGameParticleManagerSystem::create_effect_index
- **Pattern:** `40 57 48 83 EC 20 49 8B 00`
- **File:** `cs2-internal/sdk/interfaces/сgameparticlemanagersystem.hpp:74`
- **Offset Name:** `create_effect_index`
- **CS2-Dumper Offset:** N/A (Particle system)
- **Description:** Creates particle effect index

### 51. CGameParticleManagerSystem::cache_particle_effect
- **Pattern:** `4C 8B DC 53 48 83 EC ? 48 8B 84 24`
- **File:** `cs2-internal/sdk/interfaces/сgameparticlemanagersystem.hpp:80`
- **Offset Name:** `cache_particle_effect`
- **CS2-Dumper Offset:** N/A (Particle system)
- **Description:** Caches particle effect

### 52. CGameParticleManagerSystem::create_effect
- **Pattern:** `48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? F3 0F 10 1D ? ? ? ? 41 8B F8 8B DA 4C 8D 05`
- **File:** `cs2-internal/sdk/interfaces/сgameparticlemanagersystem.hpp:86`
- **Offset Name:** `create_effect`
- **CS2-Dumper Offset:** N/A (Particle system)
- **Description:** Creates particle effect

### 53. CGameParticleManagerSystem::unk_function
- **Pattern:** `40 57 48 83 EC 20 49 8B 00`
- **File:** `cs2-internal/sdk/interfaces/сgameparticlemanagersystem.hpp:92`
- **Offset Name:** `unk_function`
- **CS2-Dumper Offset:** N/A (Particle system)
- **Description:** Unknown particle function

---

## sdk/interfaces/iengineclient.h

### 54. ISource2Client::GetEconItemSystem
- **Pattern:** `48 8B 05 ?? ?? ?? ?? 48 85 C0 75 6B`
- **File:** `cs2-internal/sdk/interfaces/iengineclient.h:102`
- **Offset Name:** `GetEconItemSystem`
- **CS2-Dumper Offset:** N/A (Economy system)
- **Description:** Gets economy item system

---

## sdk/interfaces/itrace.h

### 55. i_trace::init_trace (trace_filter_d)
- **Pattern:** `48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 0F B6 41 37 33`
- **File:** `cs2-internal/sdk/interfaces/itrace.h:361`
- **Offset Name:** `init_trace`
- **CS2-Dumper Offset:** N/A (Trace system)
- **Description:** Initializes trace filter

### 56. i_trace::init_trace2 (c_trace_filter)
- **Pattern:** `48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 0F B6 41 37 33`
- **File:** `cs2-internal/sdk/interfaces/itrace.h:368`
- **Offset Name:** `init_trace2`
- **CS2-Dumper Offset:** N/A (Trace system)
- **Description:** Initializes trace filter (alternative)

### 57. i_trace::trace_shape
- **Pattern:** `E8 ? ? ? ? F3 0F 10 85 ? ? ? ? 0F 2E C7 0F 8A`
- **File:** `cs2-internal/sdk/interfaces/itrace.h:376`
- **Offset Name:** `trace_shape`
- **CS2-Dumper Offset:** N/A (Trace system)
- **Description:** Traces shape from start to end

### 58. i_trace::clip_ray_to_entity (ray_d)
- **Pattern:** `48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 54 41 56 41 57 48 81 EC C0 00 00 00 48 8B 9C`
- **File:** `cs2-internal/sdk/interfaces/itrace.h:384`
- **Offset Name:** `clip_ray_to_entity`
- **CS2-Dumper Offset:** N/A (Trace system)
- **Description:** Clips ray to entity

### 59. i_trace::clip_ray_to_entity2 (ray_t)
- **Pattern:** `48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 54 41 56 41 57 48 81 EC C0 00 00 00 48 8B 9C`
- **File:** `cs2-internal/sdk/interfaces/itrace.h:392`
- **Offset Name:** `clip_ray_to_entity2`
- **CS2-Dumper Offset:** N/A (Trace system)
- **Description:** Clips ray to entity (alternative)

### 60. i_trace::create_trace
- **Pattern:** `48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 57 41 56 41 57 48 83 EC 40 F2`
- **File:** `cs2-internal/sdk/interfaces/itrace.h:399`
- **Offset Name:** `create_trace`
- **CS2-Dumper Offset:** N/A (Trace system)
- **Description:** Creates trace data

### 61. i_trace::get_trace_info
- **Pattern:** `41 0F B7 40 ?? F3 41 0F 10 10`
- **File:** `cs2-internal/sdk/interfaces/itrace.h:405`
- **Offset Name:** `get_trace_info`
- **CS2-Dumper Offset:** N/A (Trace system)
- **Description:** Gets trace information

### 62. i_trace::Init (trace_filter_t)
- **Pattern:** `48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 0F B6 41 37 33`
- **File:** `cs2-internal/sdk/interfaces/itrace.h:413`
- **Offset Name:** `Init`
- **CS2-Dumper Offset:** N/A (Trace system)
- **Description:** Initializes trace filter

### 63. i_trace::ClipTraceToPlayers
- **Pattern:** CLIP_TRACE_TO_PLAYERS (defined elsewhere)
- **Alternative:** `48 8B C4 55 56 48 8D A8 58`
- **File:** `cs2-internal/sdk/interfaces/itrace.h:426-431`
- **Offset Name:** `ClipTraceToPlayers`
- **CS2-Dumper Offset:** N/A (Trace system)
- **Description:** Clips trace to player entities

### 64. i_trace::TraceShape
- **Pattern:** `48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 48 89 4C 24 08 55 41 54 41 55 41 56 41 57 48 8D`
- **File:** `cs2-internal/sdk/interfaces/itrace.h:442`
- **Offset Name:** `TraceShape`
- **CS2-Dumper Offset:** N/A (Trace system)
- **Description:** Traces shape along ray

---

## CS2-Dumper Offsets Reference

Based on `cs2-dumper/output/offsets.json`:

### client.dll Offsets
- `dwCSGOInput`: 31623168 (0x1E25700)
- `dwEntityList`: 30410384 (0x1D01890)
- `dwGameEntitySystem`: 33181664 (0x1FA36E0)
- `dwGameEntitySystem_highestEntityIndex`: 8432 (0x20F0)
- `dwGameRules`: 31579008 (0x1E1A0C0)
- `dwGlobalVars`: 29167184 (0x1BD3550)
- `dwGlowManager`: 31566360 (0x1E16E18)
- `dwLocalPlayerController`: 31499080 (0x1E06148)
- `dwLocalPlayerPawn`: 29211408 (0x1BDE410)
- `dwPlantedC4`: 31601288 (0x1E1E208)
- `dwPrediction`: 29211200 (0x1BDE340)
- `dwSensitivity`: 31569016 (0x1E174F8)
- `dwSensitivity_sensitivity`: 80 (0x50)
- `dwViewAngles`: 31624880 (0x1E25CB0)
- `dwViewMatrix`: 31582496 (0x1E1C420)
- `dwViewRender`: 31586160 (0x1E1D270)
- `dwWeaponC4`: 31176448 (0x1DBD100)

### engine2.dll Offsets
- `dwBuildNumber`: 6222820 (0x5EF564)
- `dwNetworkGameClient`: 6218992 (0x5EE570)
- `dwNetworkGameClient_clientTickCount`: 856 (0x358)
- `dwNetworkGameClient_deltaTick`: 572 (0x23C)
- `dwNetworkGameClient_isBackgroundMap`: 2888751 (0x2C14AF)
- `dwNetworkGameClient_localPlayer`: 232 (0xE8)
- `dwNetworkGameClient_maxClients`: 560 (0x230)
- `dwNetworkGameClient_serverTickCount`: 572 (0x23C)
- `dwNetworkGameClient_signOnState`: 544 (0x220)
- `dwWindowHeight`: 9360932 (0x8ED2A4)
- `dwWindowWidth`: 9360928 (0x8ED2A0)

### inputsystem.dll Offsets
- `dwInputSystem`: 285216 (0x45A20)

---

## Pattern Usage Notes

1. **? in patterns** represent wildcard bytes that can vary between game versions
2. **GetAbsoluteAddress** resolves RIP-relative addresses (relative calls/jumps)
3. **ResolveRelativeAddress** resolves lea instructions and similar
4. **CXOR** is used for compile-time string encryption
5. Most patterns scan in **client.dll** unless otherwise specified

## Modules
- `CLIENT_DLL` = client.dll
- `ENGINE2_DLL` = engine2.dll
- `SCENESYSTEM_DLL` = scenesystem.dll
- `PARTICLES_DLL` = particles.dll
- `RENDERSYSTEM_DLL` = rendersystem.dll
- `TIER0_DLL` = tier0.dll
- `SCHEMASYSTEM_DLL` = schemasystem.dll
- `INPUTSYSTEM_DLL` = inputsystem.dll

---

*Last Updated: 2024*
*Note: Patterns may change with game updates. Always verify signatures after CS2 patches.*
