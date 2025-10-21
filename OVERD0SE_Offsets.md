| File | Pattern/Offset | Dumper Name (a2x/cs2-dumper) |
| :--- | :--- | :--- |
| `cs2-internal/core/interfaces.cpp` | `48 8D 0D ? ? ? ? 48 89 4D F8 E8 ? ? ? ? 48 8B D0` | `dwSchemaSystem` |
| `cs2-internal/core/interfaces.cpp` | `48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 05 ? ? ? ? 48 89 05 ? ? ? ?` | `dwGameEntitySystem` |
| `cs2-internal/core/interfaces.cpp` | `48 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? FF 50 ? 48 85 C0 74 ? 48 8B 08` | `dwGameRules` |
| `cs2-internal/core/interfaces.cpp` | `48 8B 05 ? ? ? ? 48 85 C0 74 ? 4C 8D 05` | `dwPlantedC4` |
| `cs2-internal/core/interfaces.cpp` | `48 8B 05 ? ? ? ? 48 8B D9 8B 81` | `dwInputSystem` |
| `cs2-internal/core/interfaces.cpp` | `48 8D 05 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 33 D2` | `dwLocalPlayerController` |
| `cs2-internal/core/interfaces.cpp` | `48 8B 05 ? ? ? ? 48 85 C0 74 05 48 8B 08 B8` | `dwGlobalVars` |
| `cs2-internal/core/interfaces.cpp` | `48 83 EC 28 8B 05 ? ? ? ? 8B 0C 88` | `dwViewMatrix` |
| `cs2-internal/core/interfaces.cpp` | `4C 8D 05 ? ? ? ? 48 8D 15 ? ? ? ? 48 8D 0D ? ? ? ? E8` | `dwViewAngles` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B D9 48 8B FA E8` | `fnGetFileWeaponInfoFromHandle` |
| `cs2-internal/core/interfaces.cpp` | `40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 48 85 C0 75 0C` | `fnGetPlayerPawn` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC 30 4C 8B F1` | `fnSetDynamicAttributeValue` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC 30 4C 8B F1` | `fnSetViewModelAttributeValue` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 60 45 8B F0` | `fnServerRankRevealAll` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B D9 41 8B F0 48 8B 49 08 48 8B F2 E8 ? ? ? ? 48 8B C8` | `fnOnFriendEquip` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 70 4C 8B F1` | `fnGiveOnFriendEquip` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC 40 48 8B E9` | `fnUiComponent_SetInput` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B D9 4C 8B C1` | `fnUiComponent_SetEquipped` |
| `cs2-internal/core/interfaces.cpp` | `E8 ? ? ? ? 48 8B D8 48 85 C0 74 15` | `fnGetGCSOCache` |
| `cs2-internal/core/interfaces.cpp` | `E8 ? ? ? ? 48 85 C0 74 08 48 8B C8 E8 ? ? ? ? 48 8B C8` | `fnFindSOCache` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 49 8B F0 41 8B E8 48 8B F2 48 8B D9 45 33 C0` | `fnAddLootToList` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 60 48 8B F1` | `fnCreateSharedObjectSub` |
| `cs2-internal/core/interfaces.cpp` | `E8 ? ? ? ? 8B 53 10 49 8B C6` | `fnGetSOCache` |
| `cs2-internal/core/interfaces.cpp` | `E8 ? ? ? ? 4C 8D 4D 27` | `fnGetInventoryManager` |
| `cs2-internal/core/interfaces.cpp` | `E8 ? ? ? ? 48 8B F8 48 85 C0 74 36` | `fnGetLocalPlayerSub` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 08 57 48 83 EC 20 48 8B F9 E8 ? ? ? ? 48 8B 17` | `fnClearInventory` |
| `cs2-internal/core/interfaces.cpp` | `45 33 C0 B8 ? ? ? ? E8 ? ? ? ? 48 2B E0` | `fnEquipItem` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F9 B9` | `fnGetItemSchema` |
| `cs2-internal/core/interfaces.cpp` | `48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 55 41 54 41 55 41 56 41 57 48 8D A8` | `fnCreateBaseTypeCache` |
| `cs2-internal/core/interfaces.cpp` | `48 8B 0D ? ? ? ? 48 85 C9 74 ? E8 ? ? ? ? 48 85 C0 74 ? 48 8B 00 48 8B C8 FF 90` | `fnGetGCClient` |
| `cs2-internal/core/interfaces.cpp` | `48 8B C4 48 89 58 10 48 89 70 18 48 89 78 20 55 41 56 41 57 48 8D 68 A1 48 81 EC` | `fnGCRetrieveMessage` |
| `cs2-internal/core/interfaces.cpp` | `40 53 55 56 57 41 54 41 56 41 57 48 83 EC 58` | `fnGCSendMessage` |
| `cs2-internal/core/interfaces.cpp` | `48 83 EC 28 B9 ? ? ? ? E8 ? ? ? ? 48 85 C0 74 08 48 8B C8 E8 ? ? ? ? 48 8B C8` | `fnGetSUBMGCText` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B F8 48 8B F2 48 8B D9 41 83 F8 02` | `fnIsPaintable` |
| `cs2-internal/core/interfaces.cpp` | `48 83 EC 28 48 8B 05 ? ? ? ? 48 8D 15 ? ? ? ? 48 85 C0 74 05 E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 83 C4 28 C3 CC CC CC CC CC CC CC CC CC 48 83 EC 28 B2 01` | `fnGetEconItemView` |
| `cs2-internal/core/interfaces.cpp` | `48 8B C4 48 89 58 08 48 89 70 10 57 48 83 EC 20 48 8B D9 48 8B 09 48 8B FA` | `fnGetSOCData` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 20 49 8B F0 41 8B E8` | `fnGetStickerKitInfo` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F1 41 8B D8` | `fnGetMusicKitInfo` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC 30 4D 8B F1` | `fnSetStatTrak` |
| `cs2-internal/core/interfaces.cpp` | `40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 48 85 C0 74 08 4C 8B 00 4D 85 C0 74 05` | `fnSetPlayerComm` |
| `cs2-internal/core/interfaces.cpp` | `48 83 EC 28 41 80 79 04 00 74 31` | `fnIsMuted` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B EA 4C 8B F1` | `fnLineGoesThroughSmoke` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 60 48 8B F1` | `fnRankCrt` |
| `cs2-internal/core/interfaces.cpp` | `48 8B C4 48 89 58 08 48 89 70 10 57 48 83 EC 20 48 8B D9 48 8B F2 E8` | `fnWrite` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 08 57 48 83 EC 20 48 8B D9 48 8B 0D ? ? ? ? 48 8B F2` | `fnSendClMsg` |
| `cs2-internal/core/interfaces.cpp` | `48 8B C4 48 89 58 10 48 89 70 18 57 48 83 EC 20 48 8B D9 48 8B F2 E8` | `fnSendSvMsg` |
| `cs2-internal/core/interfaces.cpp` | `48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 54 41 56 41 57 48 83 EC 20 48 8B EA 4C 8B F1` | `fnFlashDuration` |
| `cs2-internal/core/interfaces.cpp` | `48 83 EC 28 48 8B 05 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 8B 19` | `fnWeaponData` |
| `cs2-internal/core/interfaces.cpp` | `48 83 EC 28 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 33 D2` | `fnPlayerData` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC 40 48 8B E9 41 8B D8 48 8B F2 45 8B F0 8B 49 04` | `fnCanSee` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 6F 45 8B F9` | `fnHandleBullet` |
| `cs2-internal/core/interfaces.cpp` | `48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 4C 89 60 20 55 41 54 41 55 41 56 41 57 48 8D 68 B1 48 81 EC ? ? ? ? 45 33 F6` | `fnTraceShape` |
| `cs2-internal/core/interfaces.cpp` | `48 83 EC 28 48 8B 1D ? ? ? ? 48 85 DB 74 05 E8 ? ? ? ? 48 8B D8` | `fnConvar` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC 50 48 8B EA 4C 8B F1` | `fnTraceFilter` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F9 48 8B 0D ? ? ? ? 41 8B D8 48 8B F2` | `fnSubTick` |
| `cs2-internal/core/interfaces.cpp` | `40 53 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 8B D9 48 8B 0D` | `fnRandomSeed` |
| `cs2-internal/core/interfaces.cpp` | `48 83 EC 28 48 63 C2 48 8D 0D` | `fnRandomFloat` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B D9 41 8B F0 48 8B FA E8` | `fnRandomInt` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 08 57 48 83 EC 20 8B F9 48 8B D9 E8` | `fnGetWeaponData` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC 30 48 8B F1 4C 8B F2 48 8B D9` | `fnPenetration` |
| `cs2-internal/core/interfaces.cpp` | `48 83 EC 28 48 8B 0D ? ? ? ? 48 85 C9 74 ? E8 ? ? ? ? 48 85 C0 74 ? 48 8B 00 48 8B C8 FF 90` | `fnGetClient` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F9 48 8B 0D` | `fnGetAnimation` |
| `cs2-internal/core/interfaces.cpp` | `48 8B C4 4C 89 40 18 48 89 50 10 48 89 48 08 53 48 83 EC 20 48 8B D9 48 85 D2` | `fnUpdate` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC 60 4C 8B F1` | `fnSetup` |
| `cs2-internal/core/interfaces.cpp` | `48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 54 41 56 41 57 48 83 EC 20 48 8B D9 41 8B F1` | `fnGetLayer` |
| `cs2-internal/core/interfaces.cpp` | `48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 48 83 EC 20 48 8B F9 48 8B 09` | `fnIsLoaded` |
| `cs2-internal/core/interfaces.cpp` | `48 83 EC 28 48 8B 05 ? ? ? ? 48 85 C0 74 05 E8 ? ? ? ? 48 8B C8` | `fnIsOwner` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 08 48 89 6C 24 10 56 57 41 54 41 56 41 57 48 83 EC 30 48 8B F1 4C 8B F2` | `fnSetPose` |
| `cs2-internal/core/interfaces.cpp` | `48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 48 83 EC 20 48 8B D9 48 8B F2 E8` | `fnGetSequence` |
| `cs2-internal/core/interfaces.cpp` | `48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 55 48 8D 68 A9 48 81 EC` | `fnInvalidate` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 08 57 48 83 EC 20 8B F9 48 8B D9 E8` | `fnGetSequenceAct` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B F9 41 8B D8 48 8B F2 E8` | `fnSetSequence` |
| `cs2-internal/core/interfaces.cpp` | `48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B EA 4C 8B F1 E8` | `fnReset` |
| `cs2-internal/core/interfaces.cpp` | `48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 48 83 EC 20 48 8B F2 48 8B D9` | `fnIsActive` |
| `cs2-internal/core/interfaces.cpp` | `48 83 EC 28 8B 05 ? ? ? ? 8B 0C 88` | `fnGetMatricesForView` |