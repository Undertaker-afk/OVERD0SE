#pragma once

#include "interfaces/igameresourceservice.h"
#include "interfaces/ischemasystem.h"

#include "../core/schema.h"
#include "../core/pointer/pointer.hpp"
#include "../core/csig/sigscan.hpp"
#include "../core/schemav2.hpp"
#include "../core/interfaces.h"

#include "../utilities/draw.h"

#include "datatypes/vector.h"
#include "datatypes/qangle.h"
#include "datatypes/color.h"
#include "datatypes/transform.h"
#include "datatypes/utlstring.h"
#include "datatypes/utlvector.h"
#include "datatypes/cstronghandle.hpp"

#include "entity_handle.h"

#include "../sdk/datatypes/usercmd.h"

#include <mutex>

using GameTime_t = std::int32_t;
using GameTick_t = std::int32_t;

enum TRACE_TYPE : int {
	DEFAULT = 0,
	AIMBOT = 1
};

class CFiringModeFloat
{
public:
	float flValue[2];
};

class CEconItemDefinition;
class CCSPlayerInventory;

enum e_hitgroups : std::uint32_t
{
	hitgroup_generic,
	hitgroup_head,
	hitgroup_chest,
	hitgroup_stomach,
	hitgroup_leftarm,
	hitgroup_rightarm,
	hitgroup_leftleg,
	hitgroup_rightleg,
	hitgroup_neck,
	hitgroup_gear = 10U
};

enum e_hitboxes : std::uint32_t {
	/* head */
	hitbox_head,
	hitbox_neck,
	/* body */
	hitbox_pelvis,
	hitbox_stomach,
	hitbox_chest,
	hitbox_lower_chest,
	hitbox_upper_chest,
	/* legs */
	hitbox_right_thigh,
	hitbox_left_thigh,
	hitbox_right_calf,
	hitbox_left_calf,
	hitbox_right_foot,
	hitbox_left_foot,
	/* arms */
	hitbox_right_hand,
	hitbox_left_hand,
	hitbox_right_upper_arm,
	hitbox_right_forearm,
	hitbox_left_upper_arm,
	hitbox_left_forearm,
	hitbox_max
};

enum HITBOXES : uint32_t {
	HEAD = 6,
	NECK = 5,
	CHEST = 4,
	RIGHT_CHEST = 8,
	LEFT_CHEST = 13,
	STOMACH = 3,
	PELVIS = 2,
	CENTER = 1,
	L_LEG = 23,
	L_FEET = 24,
	R_LEG = 26,
	R_FEET = 27
};

enum CSWeaponType : std::uint32_t {
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL = 1,
	WEAPONTYPE_SUBMACHINEGUN = 2,
	WEAPONTYPE_RIFLE = 3,
	WEAPONTYPE_SHOTGUN = 4,
	WEAPONTYPE_SNIPER_RIFLE = 5,
	WEAPONTYPE_MACHINEGUN = 6,
	WEAPONTYPE_C4 = 7,
	WEAPONTYPE_TASER = 8,
	WEAPONTYPE_GRENADE = 9,
	WEAPONTYPE_EQUIPMENT = 10,
	WEAPONTYPE_STACKABLEITEM = 11,
	WEAPONTYPE_FISTS = 12,
	WEAPONTYPE_BREACHCHARGE = 13,
	WEAPONTYPE_BUMPMINE = 14,
	WEAPONTYPE_TABLET = 15,
	WEAPONTYPE_MELEE = 16,
	WEAPONTYPE_SHIELD = 17,
	WEAPONTYPE_ZONE_REPULSOR = 18,
	WEAPONTYPE_UNKNOWN = 19,
};

enum class ClassID : int {
	CAK47 = 0,
	CBarnLight = 1,
	CBaseAnimGraph = 2,
	CBaseEntity = 7,
	CBaseModelEntity = 10,
	CBombTarget = 18,
	CBreakable = 21,
	CBreakableProp = 22,
	CC4 = 25,
	CCSGameRulesProxy = 29,
	CCSGO_TeamIntroCounterTerroristPosition = 30,
	CCSGO_TeamIntroTerroristPosition = 31,
	CCSGO_TeamSelectCounterTerroristPosition = 32,
	CCSGO_TeamSelectTerroristPosition = 33,
	CCSGOViewModel = 36,
	CCSMinimapBoundary = 37,
	CCSObserverPawn = 38,
	CCSPlayerController = 39,
	CCSPlayerPawn = 40,
	CCSPlayerResource = 42,
	CCSTeam = 43,
	CDEagle = 45,
	CDecoyGrenade = 46,
	CDecoyProjectile = 47,
	CDynamicProp = 49,
	CEnvCombinedLightProbeVolume = 54,
	CEnvCubemapBox = 56,
	CEnvCubemapFog = 57,
	CEnvSky = 64,
	CEnvWind = 67,
	CFlashbang = 72,
	CFlashbangProjectile = 73,
	CFuncBrush = 76,
	CHEGrenade = 88,
	CHEGrenadeProjectile = 89,
	CIncendiaryGrenade = 93,
	CInferno = 94,
	CInfoOffscreenPanoramaTexture = 97,
	CKnife = 102,
	CLightEnvironmentEntity = 105,
	CMolotovGrenade = 112,
	CMolotovProjectile = 113,
	COmniLight = 114,
	CParticleSystem = 115,
	CPathParticleRope = 116,
	CPhysicsPropMultiplayer = 119,
	CPlantedC4 = 121,
	CPlayerPing = 122,
	CPlayerVisibility = 124,
	CPointCamera = 125,
	CPointEntity = 130,
	CPostProcessingVolume = 133,
	CRectLight = 141,
	CSkyCamera = 147,
	CSmokeGrenade = 148,
	CSmokeGrenadeProjectile = 149,
	CVoteController = 173,
	CWeaponAWP = 176,
	CWeaponElite = 179,
	CWeaponG3SG1 = 182,
	CWeaponGlock = 184,
	CWeaponHKP2000 = 185,
	CWeaponM4A1 = 187,
	CWeaponMP7 = 192,
	CWeaponNegev = 194,
	CWeaponP250 = 196,
	CWeaponSCAR20 = 200,
	CWeaponSSG08 = 203,
	CWeaponTaser = 204,
	CWeaponTec9 = 205,
	CWeaponXM1014 = 208,
	CWorld = 210,
};

enum e_item_def_index_t : short
{
	WEAPON_DEAGLE = 1,
	WEAPON_DUAL_BERETTAS = 2,
	WEAPON_FIVE_SEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALIL = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A4 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP = 24,
	WEAPON_MP5SD = 23,
	WEAPON_XM1024 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG553 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCDENDIARY = 48,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_M4A1_S = 60,
	WEAPON_USP_S = 61,
	WEAPON_CZ75 = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFECSS = 503,
	WEAPON_KNIFEFLIP = 505,
	WEAPON_KNIFEGUT = 506,
	WEAPON_KNIFEKARAMBIT = 507,
	WEAPON_KNIFEM9BAYONET = 508,
	WEAPON_KNIFETACTICAL = 509,
	WEAPON_KNIFEFALCHION = 512,
	WEAPON_KNIFESURVIVALBOWIE = 514,
	WEAPON_KNIFEBUTTERFLY = 515,
	WEAPON_KNIFEPUSH = 516,
	WEAPON_KNIFECORD = 517,
	WEAPON_KNIFECANIS = 518,
	WEAPON_KNIFEURSUS = 519,
	WEAPON_KNIFEGYPSYJACKKNIFE = 520,
	WEAPON_KNIFEOUTDOOR = 521,
	WEAPON_KNIFESTILETTO = 522,
	WEAPON_KNIFEWIDOWMAKER = 523,
	WEAPON_KNIFESKELETON = 525,
	WEAPON_KNIFEKUKRI = 526,
};


// MGetKV3ClassDefaults

class CEntityInstance;

class CEntityIdentity
{
public:
	CS_CLASS_NO_INITIALIZER(CEntityIdentity);

	// @note: handle index is not entity index
	SCHEMA_ADD_OFFSET(std::uint32_t, GetIndex, 0x10);
	SCHEMA_ADD_FIELD(const char*, GetDesignerName, "CEntityIdentity->m_designerName");
	SCHEMA_ADD_FIELD(std::uint32_t, GetFlags, "CEntityIdentity->m_flags");

	[[nodiscard]] bool IsValid()
	{
		return GetIndex() != INVALID_EHANDLE_INDEX;
	}

	[[nodiscard]] int GetEntryIndex()
	{
		if (!IsValid())
			return ENT_ENTRY_MASK;

		return GetIndex() & ENT_ENTRY_MASK;
	}

	[[nodiscard]] int GetSerialNumber()
	{
		return GetIndex() >> NUM_SERIAL_NUM_SHIFT_BITS;
	}

	CEntityInstance* pInstance; // 0x00
};

class CEntityInstance
{
public:
	CS_CLASS_NO_INITIALIZER(CEntityInstance);

	void GetSchemaClassInfo(SchemaClassInfoData_t** pReturn)
	{
		return MEM::CallVFunc<void, 38U>(this, pReturn);
	}

	// cHoca
	[[nodiscard]] std::uint32_t GetEntityHandle()
	{
		using function_t = std::uint32_t(CS_FASTCALL*)(void*);
		static function_t fn = reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, CXOR("48 85 C9 74 32 48 8B 41 10 48 85 C0 74 29 44")));
		CS_ASSERT(fn != nullptr);
		return fn(this);
	}

	[[nodiscard]] CBaseHandle GetRefEHandle()
	{
		CEntityIdentity* pIdentity = GetIdentity();
		if (pIdentity == nullptr)
			return CBaseHandle();

		return CBaseHandle(pIdentity->GetEntryIndex(), pIdentity->GetSerialNumber() - (pIdentity->GetFlags() & 1));
	}


	template <typename T>
	T* As() {
		return reinterpret_cast<T*>(this);
	}

	SCHEMA_ADD_FIELD(CEntityIdentity*, GetIdentity, "CEntityInstance->m_pEntity");
	SCHEMA_ADD_FIELD(bool, VisibleinPVS, "CEntityInstance->m_bVisibleinPVS");
};


enum EEconItemQuality {
	IQ_UNDEFINED = -1,
	IQ_NORMAL,
	IQ_GENUINE,
	IQ_VINTAGE,
	IQ_UNUSUAL,
	IQ_UNIQUE,
	IQ_COMMUNITY,
	IQ_DEVELOPER,
	IQ_SELFMADE,
	IQ_CUSTOMIZED,
	IQ_STRANGE,
	IQ_COMPLETED,
	IQ_HAUNTED,
	IQ_TOURNAMENT,
	IQ_FAVORED
};

enum EEconItemRarity {
	IR_DEFAULT,
	IR_COMMON,
	IR_UNCOMMON,
	IR_RARE,
	IR_MYTHICAL,
	IR_LEGENDARY,
	IR_ANCIENT,
	IR_IMMORTAL
};

// https://gitlab.com/KittenPopo/csgo-2018-source/-/blob/main/game/shared/econ/econ_item_constants.h#L39
enum EEconTypeID {
	k_EEconTypeItem = 1,
	k_EEconTypePersonaDataPublic = 2,
	k_EEconTypeGameAccountClient = 7,
	k_EEconTypeGameAccount = 8,
	k_EEconTypeEquipInstance = 31,
	k_EEconTypeDefaultEquippedDefinitionInstance = 42,
	k_EEconTypeDefaultEquippedDefinitionInstanceClient = 43,
	k_EEconTypeCoupon = 45,
	k_EEconTypeQuest = 46,
};

// MGetKV3ClassDefaults
class SkeletonBoneBounds_t
{
public:
	Vector_t m_vecCenter; // 0x0	
	Vector_t m_vecSize; // 0xc	
};

// Registered binary: animationsystem.dll (project 'modellib')
// Alignment: 8
// Size: 0x60
// 
// MGetKV3ClassDefaults
class RenderSkeletonBone_t
{
public:
	CS_CLASS_NO_INITIALIZER(RenderSkeletonBone_t);

	SCHEMA_ADD_OFFSET(const char*, m_boneName, 0x0);
	SCHEMA_ADD_OFFSET(const char*, m_parentName, 0x8);
	SCHEMA_ADD_OFFSET(SkeletonBoneBounds_t, m_bbox, 0x40);
	SCHEMA_ADD_OFFSET(float, m_flSphereRadius, 0x58);

};

// Registered binary: animationsystem.dll (project 'modellib')
// Alignment: 8
// Size: 0x50
// 
// MGetKV3ClassDefaults
class CRenderSkeleton
{
public:
	CS_CLASS_NO_INITIALIZER(CRenderSkeleton);

	SCHEMA_ADD_OFFSET(CUtlVector<RenderSkeletonBone_t>, m_bones, 0x0);
	SCHEMA_ADD_OFFSET(CUtlVector<int32_t>, m_boneParents, 0xB8);
	SCHEMA_ADD_OFFSET(int32_t, m_nBoneWeightCount, 0x48);

};
class CAggregateSceneObjectData {
private:
	char pad_0000[0x38]; // 0x0
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
private:
	char pad_0038[0x9];
};

class CAggregateSceneObject {
private:
	char pad_0000[0x120];
public:
	int count; // 0x120
private:
	char pad_0120[0x4];
public:
	CAggregateSceneObjectData* array; // 0x128
};

enum ModelType_t : int
{
	MODEL_SUN,
	MODEL_CLOUDS,
	MODEL_EFFECTS,
	MODEL_OTHER,
};

class CSceneObject
{
public:
	char pad_0000[184]; //0x0000
	uint8_t r; //0x00B8
	uint8_t g; //0x00B9
	uint8_t b; //0x00BA
	uint8_t a; //0x00BB
	char pad_00BC[196]; //0x00BC
};

class CMaterial2
{
public:
	const char* GetName()
	{
		return MEM::CallVFunc<const char*, 1>(this);
	}
};

class CBaseSceneData
{
public:
	char pad_0000[24]; //0x0000
	CSceneObject* scene_object; //0x0018
	CMaterial2* material; //0x0020 
	CMaterial2* material2; //0x0028
	char pad_0030[0x20]; //0x0030
	uint8_t r; //0x0040
	uint8_t g; //0x0041
	uint8_t b; //0x0042
	uint8_t a; //0x0043
	char pad_0044[0x12]; //0x0044
}; //Size: 0x0068

class c_attachments
{
public:
	char		pad_0x0000[0x10];		  // 0x0000
	const char* m_key;					  // 0x0010
	char		pad_0x0018[0x8];		  // 0x0018
	const char* m_name;					  // 0x0020
	const char* m_influenceNames[3];	  // 0x0028
	char		pad_0x0030[0x10];		  // 0x0030
	Vector4D_t	m_vInfluenceRotations[3]; // 0x0040
	Vector_t		m_vInfluenceOffsets[3];	  // 0x0070
	float		m_influenceWeights[3];	  // 0x0094
	char		pad_0x00A0[0x10];		  // 0x00A0
};

class c_drawcalls
{
public:
	__int32 m_nPrimitiveType; // 0x0000
	__int32 m_nBaseVertex;	  // 0x0004
	__int32 m_nVertexCount;	  // 0x0008
	__int32 m_nStartIndex;	  // 0x000C
	__int32 m_nIndexCount;	  // 0x0010
	float	m_flUvDensity;	  // 0x0014
	float	m_vTintColor[3];  // 0x0018
	float	m_flAlpha;		  // 0x0024
	char	pad_0x0028[0xC0]; // 0x0028

}; // Size=0x00E8

class c_bones
{
public:
	const char* m_boneName;		   // 0x0000
	const char* m_parentName;	   // 0x0008
	float		m_invBindPose[12]; // 0x0010
	Vector_t		m_vecCenter;	   // 0x0040
	Vector_t		m_vecSize;		   // 0x004C
	float		m_flSphereradius;  // 0x0058
	char		pad_0x005C[0x4];   // 0x005C

}; // Size=0x0060


//class CRenderMesh
//{
//
//public:
//
//	char		   pad_0x0000[40];	// 0x0000
//	Vector_t		   m_vMinBounds;		// 0x0028
//	Vector_t		   m_vMaxBounds;		// 0x0034
//	c_drawcalls* m_drawCalls;			// 0x0048
//	char		   pad_0x0050[104];	// 0x0050
//	__int32		   m_skeletoncount;		// 0x00B8
//	char		   pad_0x00BC[4];		// 0x00BC
//	c_bones* m_skeleton;			// 0x00C0
//	char		   pad_0x00C8[112];		// 0x00C8
//
//
//};


class c_hitbox
{
public:
	const char* m_name; // 0x0	
	const char* m_sSurfaceProperty; // 0x8	
	const char* m_sBoneName; // 0x10	
	Vector_t m_vMinBounds; // 0x18	
	Vector_t m_vMaxBounds; // 0x24	
	float m_flShapeRadius; // 0x30	
	uint32_t m_nBoneNameHash; // 0x34	
	int32_t m_nGroupId; // 0x38	
	uint8_t m_nShapeType; // 0x3c	
	bool m_bTranslationOnly; // 0x3d	
private:
	MEM_PAD(0x2); // 0x3e
public:
	uint32_t m_CRC; // 0x40	
	MEM_PAD(0x4); // 0x44	
	uint16_t m_nHitBoxIndex; // 0x48
	MEM_PAD(0x26);
};

class c_hitboxsets
{
public:
	MEM_PAD(0x20); // 0x0000
	uint32_t  m_nNameHash;		// 0x0020
	MEM_PAD(0x4);	// 0x0024
	__int32	  m_nHitboxCount;	// 0x0028
	MEM_PAD(0x4);	// 0x002C
	c_hitbox* m_hitbox;			// 0x0030
	MEM_PAD(0x18); // 0x0038
};

//class CRenderMesh
//{
//public:
//	MEM_PAD(0x18);	// 0x0000
//	Vector_t		   m_vMinBounds;		// 0x0028
//	Vector_t		   m_vMaxBounds;		// 0x0034
//	__int32		   m_drawCallsCount;	// 0x0040
//	MEM_PAD(0x4);		// 0x0044
//	c_drawcalls* m_drawCalls;			// 0x0048
//	MEM_PAD(0x68);	// 0x0050
//	__int32		   m_skeletoncount;		// 0x00B8
//	MEM_PAD(0x4);		// 0x00BC
//	c_bones* m_skeleton;			// 0x00C0
//	MEM_PAD(0x8);		// 0x00C8
//	__int32		   m_hashescount;		// 0x00D0
//	MEM_PAD(0x4);		// 0x00D4
//	uint32_t* m_hashes;			// 0x00D8
//	MEM_PAD(0x20);;	// 0x00E0
//	__int32		   m_nBoneWeightCount;	// 0x0100
//	MEM_PAD(0xC);;		// 0x0104
//	c_attachments* m_attachments;		// 0x0110
//	__int32		   m_attachments_count; // 0x0118
//	MEM_PAD(0x1C);	// 0x011C
//	c_hitboxsets* m_hitboxsets;		// 0x0138
//	__int32		   m_nHitboxSets;		// 0x0140
//};

class CRenderMesh
{
public:
	MEM_PAD(0x108);                // 0x0000
	c_hitboxsets* m_hitboxsets;    // 0x108
	int32_t m_nHitboxSets;        // 0x110
};

class CRenderM {
public:
	CRenderMesh* mesh;
};
struct VPhysicsCollisionAttribute_t
{

public:
	CS_CLASS_NO_INITIALIZER(VPhysicsCollisionAttribute_t);

	SCHEMA_ADD_FIELD(uint8_t, m_nCollisionGroup, "VPhysicsCollisionAttribute_t->m_nCollisionGroup");
	SCHEMA_ADD_FIELD(uint8_t, m_nHierarchyId, "VPhysicsCollisionAttribute_t->m_nHierarchyId");
	SCHEMA_ADD_FIELD(uint8_t, m_nOwnerId, "VPhysicsCollisionAttribute_t->m_nOwnerId");
	SCHEMA_ADD_FIELD(uint8_t, m_nEntityId, "VPhysicsCollisionAttribute_t->m_nEntityId");
	SCHEMA_ADD_FIELD(uint8_t, m_nCollisionFunctionMask, "VPhysicsCollisionAttribute_t->m_nCollisionFunctionMask");

};
class CCollisionProperty
{
public:
	CS_CLASS_NO_INITIALIZER(CCollisionProperty);
	SCHEMA_ADD_FIELD(Vector_t, GetMins, "CCollisionProperty->m_vecMins");
	SCHEMA_ADD_FIELD(Vector_t, GetMaxs, "CCollisionProperty->m_vecMaxs");
	SCHEMA_ADD_FIELD(uint8_t, m_solidFlags, "CCollisionProperty->m_usSolidFlags");
	SCHEMA_ADD_FIELD(VPhysicsCollisionAttribute_t, m_collisionAttribute, "CCollisionProperty->m_collisionAttribute");

	std::uint16_t get_collision_mask() {
		return *reinterpret_cast<std::uint16_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x38);
	}
};

class CAnimBone
{
public:
	CS_CLASS_NO_INITIALIZER(CAnimBone);
	SCHEMA_EXTENDED(Vector_t, m_pos, "animationsystem.dll", "CAnimBone", "m_pos", 0);


};
class CPhysModel;

class CModelSkeleton
{
public:
	CModelSkeleton() = delete;
	CModelSkeleton(CModelSkeleton&&) = delete;
	CModelSkeleton(const CModelSkeleton&) = delete;

public:
	SCHEMA_ADD_OFFSET(CUtlVector<const char*>, vecBoneNames, 0x4);
	SCHEMA_ADD_OFFSET(CUtlVector<std::uint16_t>, vecBoneParent, 0x18);
	SCHEMA_ADD_OFFSET(CUtlVector<float>, m_boneSphere, 0x30);
	SCHEMA_ADD_OFFSET(CUtlVector<std::uint32_t>, m_nFlag, 0x48);
	SCHEMA_ADD_OFFSET(CUtlVector<Vector_t>, m_bonePosParent, 0x60);
	SCHEMA_ADD_OFFSET(CUtlVector<QuaternionAligned_t>, m_boneRotParent, 0x78);
	SCHEMA_ADD_OFFSET(CUtlVector<float>, m_boneScaleParent, 0x90);

};


// Alignment: 10
// Size: 0x58
struct PermModelInfo_t
{
public:
	int m_nFlags; // 0x0	
	Vector_t m_vHullMin; // 0x4	
	Vector_t m_vHullMax; // 0x10	
	Vector_t m_vViewMin; // 0x1c	
	Vector_t m_vViewMax; // 0x28	
	float m_flMass; // 0x34	
	Vector_t m_vEyePosition; // 0x38	
	float m_flMaxEyeDeflection; // 0x44	
	const char* m_sSurfaceProperty; // 0x48	
	const char* m_keyValueText; // 0x50	
};
struct PermModelExtPart_t
{
public:
	CTransform m_Transform; // 0x0	
	const char* m_Name; // 0x20	
	int32_t m_nParent; // 0x28		
};
// Alignment: 0
// Size: 0x1
class InfoForResourceTypeCRenderMesh
{
private:
	[[maybe_unused]] uint8_t __pad0000[0x1]; // 0x0
public:
	// No members available
};
// Alignment: 0
// Size: 0x1
class InfoForResourceTypeCModel
{
private:
	[[maybe_unused]] uint8_t __pad0000[0x1]; // 0x0
public:
	// No members available
};
// Alignment: 0
// Size: 0x1
class InfoForResourceTypeCAnimationGroup
{
private:
	[[maybe_unused]] uint8_t __pad0000[0x1]; // 0x0
public:
	// No members available
};
// Alignment: 0
// Size: 0x1
class InfoForResourceTypeCSequenceGroupData
{
private:
	[[maybe_unused]] uint8_t __pad0000[0x1]; // 0x0
public:
	// No members available
};

// Alignment: 0
// Size: 0x1
class InfoForResourceTypeCPhysAggregateData
{
private:
	[[maybe_unused]] uint8_t __pad0000[0x1]; // 0x0
public:
	// No members available
};
// Alignment: 0
// Size: 0x1
class InfoForResourceTypeIMaterial2
{
private:
	[[maybe_unused]] uint8_t __pad0000[0x1]; // 0x0
public:
	// No members available
};
// Alignment: 2
// Size: 0x20
struct MaterialGroup_t
{
public:

	CUtlString m_name; // 0x0	
	CUtlVector<CStrongHandle<InfoForResourceTypeIMaterial2>> m_materials; // 0x8	
};
// Alignment: 4
// Size: 0x4
enum class ModelBoneFlexComponent_t : uint32_t
{
	MODEL_BONE_FLEX_INVALID = 0xffffffffffffffff,
	MODEL_BONE_FLEX_TX = 0x0,
	MODEL_BONE_FLEX_TY = 0x1,
	MODEL_BONE_FLEX_TZ = 0x2,
};
// Alignment: 5
// Size: 0x20
struct ModelBoneFlexDriverControl_t
{
public:
	ModelBoneFlexComponent_t m_nBoneComponent; // 0x0	
	CUtlString m_flexController; // 0x8	
	uint32_t m_flexControllerToken; // 0x10	
	float m_flMin; // 0x14	
	float m_flMax; // 0x18	
};

// Alignment: 3
// Size: 0x28

struct ModelBoneFlexDriver_t
{
public:
	const char* m_boneName; // 0x0	
	uint32_t m_boneNameToken; // 0x8	
private:
	[[maybe_unused]] uint8_t __pad000c[0x4]; // 0xc
public:
	CUtlVector<ModelBoneFlexDriverControl_t> m_controls; // 0x10	
};
// Alignment: 2
// Size: 0x48
class CModelConfigElement
{
private:
	[[maybe_unused]] uint8_t __pad0000[0x8]; // 0x0
public:
	CUtlString m_ElementName; // 0x8	
	CUtlVector<CModelConfigElement*> m_NestedElements; // 0x10	
};
// Alignment: 3
// Size: 0x28
class CModelConfig
{
public:
	CUtlString m_ConfigName; // 0x0	
	CUtlVector<CModelConfigElement*> m_Elements; // 0x8	
	bool m_bTopLevel; // 0x20	
};
// Alignment: 3
// Size: 0x20
class CModelConfigList
{
public:
	bool m_bHideMaterialGroupInTools; // 0x0	
	bool m_bHideRenderColorInTools; // 0x1	
private:
	[[maybe_unused]] uint8_t __pad0002[0x6]; // 0x2
public:
	CUtlVector<CModelConfig*> m_Configs; // 0x8	
};
// Alignment: 2
// Size: 0x10
struct PermModelDataAnimatedMaterialAttribute_t
{
public:
	CUtlString m_AttributeName; // 0x0	
	int32_t m_nNumChannels; // 0x8	
};
#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#define PAD(SIZE) BYTE MACRO_CONCAT(_pad, __COUNTER__)[SIZE];
enum bone_flags : uint32_t {
	FLAG_NO_BONE_FLAGS = 0x0,
	FLAG_BONEFLEXDRIVER = 0x4,
	FLAG_CLOTH = 0x8,
	FLAG_PHYSICS = 0x10,
	FLAG_ATTACHMENT = 0x20,
	FLAG_ANIMATION = 0x40,
	FLAG_MESH = 0x80,
	FLAG_HITBOX = 0x100,
	FLAG_BONE_USED_BY_VERTEX_LOD0 = 0x400,
	FLAG_BONE_USED_BY_VERTEX_LOD1 = 0x800,
	FLAG_BONE_USED_BY_VERTEX_LOD2 = 0x1000,
	FLAG_BONE_USED_BY_VERTEX_LOD3 = 0x2000,
	FLAG_BONE_USED_BY_VERTEX_LOD4 = 0x4000,
	FLAG_BONE_USED_BY_VERTEX_LOD5 = 0x8000,
	FLAG_BONE_USED_BY_VERTEX_LOD6 = 0x10000,
	FLAG_BONE_USED_BY_VERTEX_LOD7 = 0x20000,
	FLAG_BONE_MERGE_READ = 0x40000,
	FLAG_BONE_MERGE_WRITE = 0x80000,
	FLAG_ALL_BONE_FLAGS = 0xfffff,
	BLEND_PREALIGNED = 0x100000,
	FLAG_RIGIDLENGTH = 0x200000,
	FLAG_PROCEDURAL = 0x400000,
};

struct alignas(16) bone_data {
	Vector_t pos;
	float scale;
	Vector4D_t rot;
};

#include "../sdk/datatypes/stronghandle.hpp"
#include "const.h"
class CModel {
public:
	CModel() = delete;
	CModel(CModel&&) = delete;
	CModel(const CModel&) = delete;
public:

	SCHEMA_ADD_OFFSET(const char*, szName, 0x8);
	SCHEMA_ADD_OFFSET(CModelSkeleton, m_modelSkeleton, 0x188);
	SCHEMA_ADD_OFFSET(CRenderM*, m_meshes, 0x78);
	uint32_t GetHitboxFlags(uint32_t index);
	const char* GetHitboxName(uint32_t index);
	uint32_t GetHitboxParent(uint32_t index);
	uint32_t GetHitboxesNum();

	c_hitbox* get_hitbox(int index)
	{
		if (!this)
			return nullptr;

		auto mesh = m_meshes()->mesh;
		if (!mesh)
			return nullptr;

		auto hithoxsets = mesh->m_hitboxsets;
		if (!hithoxsets)
			return nullptr;

		if (hithoxsets[0].m_nHitboxCount <= 0 || index > hithoxsets[0].m_nHitboxCount)
			return nullptr;

		auto hitbox = hithoxsets[0].m_hitbox;
		if (!hitbox)
			return nullptr;

		return &hitbox[index];
	}

};

class C_CSGOViewModelV {
public:
	SCHEMA_ADD_FIELD(QAngle_t, m_vLoweredWeaponOffset, "C_CSGOViewModel->m_vLoweredWeaponOffset ");
};

class CModelState
{
public:
	CModelState() = delete;
	CModelState(CModelState&&) = delete;
	CModelState(const CModelState&) = delete;

	SCHEMA_ADD_FIELD(CStrongHandle< CModel >, m_hModel, "CModelState->m_hModel");
	SCHEMA_ADD_FIELD(stronghandle< CModel >, m_hModel2, "CModelState->m_hModel");

	SCHEMA_ADD_OFFSET(CTransform*, BoneTransform, 0x80);


	bone_data* GetHitboxData() noexcept {
		bone_data* boneDataPtr = *reinterpret_cast<bone_data**>(this + 0x80);

		if (boneDataPtr == nullptr)
			boneDataPtr = *reinterpret_cast<bone_data**>(reinterpret_cast<uintptr_t>(this) + 0x80);

		if (boneDataPtr == nullptr)
			return boneDataPtr;
	}


	Vector_t GetHitboxPos(uint32_t index) {
		auto hitbox = this->GetHitboxData();
		if (!hitbox)
			return nullptr;

		if (!(this->m_hModel()->GetHitboxFlags(index) & bone_flags::FLAG_HITBOX))
			return nullptr;

		auto parent_index = this->m_hModel()->GetHitboxParent(index);
		if (parent_index == -1)
			return nullptr;

		return hitbox[index].pos;
	}

	Vector4D_t GetHitboxRotation(uint32_t index) {
		auto hitbox = this->GetHitboxData();
		if (!hitbox)
			return 0;

		if (!(this->m_hModel()->GetHitboxFlags(index) & bone_flags::FLAG_HITBOX))
			return 0;

		auto parent_index = this->m_hModel()->GetHitboxParent(index);
		if (parent_index == -1)
			return 0;

		return hitbox[index].rot;
	}

	const char* GetHitboxName(uint32_t index) {
		auto hitbox = this->GetHitboxData();
		if (!hitbox)
			return nullptr;

		if (!(this->m_hModel()->GetHitboxFlags(index) & bone_flags::FLAG_HITBOX))
			return nullptr;

		auto parent_index = this->m_hModel()->GetHitboxParent(index);
		if (parent_index == -1)
			return nullptr;

		return this->m_hModel()->GetHitboxName(index);
	}
};

class CSkeletonInstance
{
public:
	CSkeletonInstance() = delete;
	CSkeletonInstance(CSkeletonInstance&&) = delete;
	CSkeletonInstance(const CSkeletonInstance&) = delete;

	MEM_PAD(0x1CC); //0x0000
	int m_nBones; //0x01BC 
	MEM_PAD(0x18); //0x01C0
	int m_nMask; //0x01D8 
	MEM_PAD(0x4); //0x01DC
	Matrix2x4_t* m_pBoneCache;

	SCHEMA_ADD_FIELD(CModelState, GetModel, "CSkeletonInstance->m_modelState");
	SCHEMA_ADD_FIELD(bool, m_bIsAnimationEnabled, "CSkeletonInstance->m_bIsAnimationEnabled");
	SCHEMA_ADD_FIELD(bool, m_bUseParentRenderBounds, "CSkeletonInstance->m_bUseParentRenderBounds");
	SCHEMA_ADD_FIELD(bool, m_bDisableSolidCollisionsForHierarchy, "CSkeletonInstance->m_bDisableSolidCollisionsForHierarchy");

	std::uint32_t get_bone_count()
	{
		return *reinterpret_cast<std::uint32_t*>(std::uintptr_t(this) + 0x1BC);
	}

	SCHEMA_ADD_FIELD(bool, m_bDirtyMotionType, "CSkeletonInstance->m_bDirtyMotionType");
	SCHEMA_ADD_FIELD(bool, m_bIsGeneratingLatchedParentSpaceState, "CSkeletonInstance->m_bIsGeneratingLatchedParentSpaceState");
	SCHEMA_ADD_FIELD(uint8_t, m_nHitboxSet, "CSkeletonInstance->m_nHitboxSet");
	
	void get_bone_data(bone_data& data, int index);
	void CS_FASTCALL calc_world_space_bones(uint32_t parent, uint32_t mask);
	void CS_FASTCALL spoofed_calc_world_space_bones(uint32_t mask);
};

class CGameSceneNode
{
public:
	CS_CLASS_NO_INITIALIZER(CGameSceneNode);

	void SetMeshGroupMask(uint64_t meshGroupMask);

	SCHEMA_ADD_FIELD(CTransform, GetNodeToWorld, "CGameSceneNode->m_nodeToWorld");
	SCHEMA_ADD_FIELD(CEntityInstance*, GetOwner, "CGameSceneNode->m_pOwner");
	SCHEMA_ADD_FIELD(Vector_t, m_vecOrigin, "CGameSceneNode->m_vecOrigin");
	SCHEMA_ADD_FIELD(Vector_t, GetAbsOrigin, "CGameSceneNode->m_vecAbsOrigin");
	SCHEMA_ADD_FIELD(Vector_t, GetRenderOrigin, "CGameSceneNode->m_vRenderOrigin");

	SCHEMA_ADD_FIELD(QAngle_t, GetAngleRotation, "CGameSceneNode->m_angRotation");
	SCHEMA_ADD_FIELD(QAngle_t, GetAbsAngleRotation, "CGameSceneNode->m_angAbsRotation");
	SCHEMA_ADD_FIELD(CTransform, GetTransform, "CGameSceneNode->m_nodeToWorld");

	SCHEMA_ADD_FIELD(bool, IsDormant, "CGameSceneNode->m_bDormant");

	CSkeletonInstance* GetSkeletonInstance()
	{
		return MEM::CallVFunc<CSkeletonInstance*, 8U>(this);
	}
};

class EntSubClassVDataBase {
public:
	template <typename T> requires std::derived_from<T, EntSubClassVDataBase>
	inline T* as() {
		return (T*)this;
	}
};

class BasePlayerWeaponVData : public EntSubClassVDataBase {
public:
	SCHEMA_ADD_FIELD(int32_t, m_iMaxClip1, "CBasePlayerWeaponVData->m_iMaxClip1");

};
enum CSWeaponCategory : std::uint32_t {
	WEAPONCATEGORY_OTHER = 0,
	WEAPONCATEGORY_MELEE = 1,
	WEAPONCATEGORY_SECONDARY = 2,
	WEAPONCATEGORY_SMG = 3,
	WEAPONCATEGORY_RIFLE = 4,
	WEAPONCATEGORY_HEAVY = 5,
	WEAPONCATEGORY_COUNT = 6,
};

using firing_float_t = float[2];

class CCSWeaponBaseVData : public BasePlayerWeaponVData {
public:
	SCHEMA_ADD_FIELD(const char*, m_szName, "CCSWeaponBaseVData->m_szName");
	SCHEMA_ADD_FIELD(int, m_WeaponType, "CCSWeaponBaseVData->m_WeaponType");
	SCHEMA_ADD_OFFSET(CSWeaponCategory, m_WeaponCategory, 0x34C);
	SCHEMA_ADD_FIELD(float, m_flSpread, "CCSWeaponBaseVData->m_flSpread");
	SCHEMA_ADD_FIELD(float, m_flHeadshotMultiplier, "CCSWeaponBaseVData->m_flHeadshotMultiplier");
	SCHEMA_ADD_FIELD(float, m_flArmorRatio, "CCSWeaponBaseVData->m_flArmorRatio");
	SCHEMA_ADD_FIELD(float, m_flPenetration, "CCSWeaponBaseVData->m_flPenetration");
	SCHEMA_ADD_FIELD(float, m_flRange, "CCSWeaponBaseVData->m_flRange");
	SCHEMA_ADD_FIELD(float, m_flRangeModifier, "CCSWeaponBaseVData->m_flRangeModifier");
	SCHEMA_ADD_OFFSET(bool, m_bIsFullAuto, 0xD3C);

	SCHEMA_ADD_FIELD(int, m_nDamage, "CCSWeaponBaseVData->m_nDamage");
	SCHEMA_ADD_FIELD(firing_float_t, m_flInaccuracyCrouch, "CCSWeaponBaseVData->m_flInaccuracyCrouch");
	SCHEMA_ADD_FIELD(firing_float_t, m_flInaccuracyStand, "CCSWeaponBaseVData->m_flInaccuracyStand");

	SCHEMA_ADD_OFFSET(CFiringModeFloat, m_flCycleTime, 0xD44);
	SCHEMA_ADD_FIELD(firing_float_t, m_flMaxSpeed, "CCSWeaponBaseVData->m_flMaxSpeed");
};

class C_BaseEntity : public CEntityInstance
{
public:
	CS_CLASS_NO_INITIALIZER(C_BaseEntity);

	SCHEMA_ADD_FIELD(CGameSceneNode*, GetGameSceneNode, "C_BaseEntity->m_pGameSceneNode");
	SCHEMA_ADD_FIELD(CCollisionProperty*, GetCollision, "C_BaseEntity->m_pCollision");
	SCHEMA_ADD_FIELD(std::uint8_t, GetTeam, "C_BaseEntity->m_iTeamNum");
	SCHEMA_ADD_FIELD(CBaseHandle, GetOwnerHandle, "C_BaseEntity->m_hOwnerEntity");
	SCHEMA_ADD_FIELD(Vector_t, GetBaseVelocity, "C_BaseEntity->m_vecBaseVelocity");
	SCHEMA_ADD_FIELD(Vector_t, GetAbsVelocity, "C_BaseEntity->m_vecAbsVelocity");
	SCHEMA_ADD_FIELD(float, m_flSimulationTime, "C_BaseEntity->m_flSimulationTime");

	SCHEMA_ADD_FIELD(Vector_t, m_vecVelocity, "C_BaseEntity->m_vecVelocity");
	SCHEMA_ADD_FIELD(bool, IsTakingDamage, "C_BaseEntity->m_bTakesDamage");
	SCHEMA_ADD_FIELD(std::uint32_t, GetFlags, "C_BaseEntity->m_fFlags");
	SCHEMA_ADD_FIELD(std::int32_t, GetEflags, "C_BaseEntity->m_iEFlags");
	SCHEMA_ADD_FIELD(std::int32_t, GetMoveType, "C_BaseEntity->m_MoveType");
	SCHEMA_ADD_FIELD(std::int32_t, GetActualMoveType, "C_BaseEntity->m_nActualMoveType");
	SCHEMA_ADD_FIELD(std::uint8_t, GetLifeState, "C_BaseEntity->m_lifeState");
	SCHEMA_ADD_FIELD(std::int32_t, GetHealth, "C_BaseEntity->m_iHealth");
	SCHEMA_ADD_FIELD(std::int32_t, GetMaxHealth, "C_BaseEntity->m_iMaxHealth");
	SCHEMA_ADD_FIELD(float, GetWaterLevel, "C_BaseEntity->m_flWaterLevel");
	SCHEMA_ADD_FIELD_OFFSET(EntSubClassVDataBase*, m_pVDataBase, "C_BaseEntity->m_nSubclassID", 0x8);
	SCHEMA_ADD_FIELD(Vector_t, m_vecViewOffset, "C_BaseModelEntity->m_vecViewOffset");
	SCHEMA_ADD_FIELD(std::uint64_t, m_nSubclassID, "C_BaseEntity->m_nSubclassID");

	void SetSceneOrigin(Vector_t origin);
	void SetModel(const char* name);

	bool ComputeHitboxSurroundingBox(Vector_t& min, Vector_t& max);

	static C_BaseEntity* GetLocalPlayer();
	// get entity origin on scene
	[[nodiscard]] const Vector_t& GetSceneOrigin();

	bool is_alive()
	{
		return GetHealth() > 0;
	}

	// @note: we shouldn't use this, just use GetSchemaClassInfo(...) and compare the class name instead
	[[nodiscard]] bool IsBasePlayerController()
	{
		return MEM::CallVFunc<bool, 147U>(this);
	}

	// same with this
	[[nodiscard]] bool IsWeapon()
	{
		return MEM::CallVFunc<bool, 158U>(this);
	}

	// same with this
	[[nodiscard]] bool IsViewModel()
	{
		//fix me
		return MEM::CallVFunc<bool, 245U>(this);
	}

	[[nodiscard]] bool IsValidMoveType() noexcept {
		if (!this)
			return false;

		const auto move_type = this->GetMoveType();
		return move_type != MOVETYPE_NOCLIP
			&& move_type != MOVETYPE_LADDER;
	}

	CSkeletonInstance* get_skeleton()
	{
		return *reinterpret_cast<CSkeletonInstance**>(reinterpret_cast<std::uintptr_t>(this) + 0x318);
	}
};

class CGlowProperty;
class C_BaseModelEntity : public C_BaseEntity
{
public:
	CS_CLASS_NO_INITIALIZER(C_BaseModelEntity);

	SCHEMA_ADD_FIELD(CCollisionProperty, GetCollisionInstance, "C_BaseModelEntity->m_Collision");
	SCHEMA_ADD_FIELD(CGlowProperty, GetGlowProperty, "C_BaseModelEntity->m_Glow");
	SCHEMA_ADD_FIELD(Vector_t, GetViewOffset, "C_BaseModelEntity->m_vecViewOffset");
	SCHEMA_ADD_FIELD(GameTime_t, GetCreationTime, "C_BaseModelEntity->m_flCreateTime");
	SCHEMA_ADD_FIELD(GameTick_t, GetCreationTick, "C_BaseModelEntity->m_nCreationTick");
	SCHEMA_ADD_FIELD(CBaseHandle, GetMoveParent, "C_BaseModelEntity->m_hOldMoveParent");
	SCHEMA_ADD_FIELD(std::float_t, GetAnimTime, "C_BaseModelEntity->m_flAnimTime");
	SCHEMA_ADD_FIELD(std::float_t, GetSimulationTime, "C_BaseModelEntity->m_flSimulationTime");
};
enum ESOCacheEvent {
	/// Dummy sentinel value
	eSOCacheEvent_None = 0,

	/// We received a our first update from the GC and are subscribed
	eSOCacheEvent_Subscribed = 1,

	/// We lost connection to GC or GC notified us that we are no longer
	/// subscribed. Objects stay in the cache, but we no longer receive updates
	eSOCacheEvent_Unsubscribed = 2,

	/// We received a full update from the GC on a cache for which we were
	/// already subscribed. This can happen if connectivity is lost, and then
	/// restored before we realized it was lost.
	eSOCacheEvent_Resubscribed = 3,

	/// We received an incremental update from the GC about specific object(s)
	/// being added, updated, or removed from the cache
	eSOCacheEvent_Incremental = 4,

	/// A lister was added to the cache
	/// @see CGCClientSharedObjectCache::AddListener
	eSOCacheEvent_ListenerAdded = 5,

	/// A lister was removed from the cache
	/// @see CGCClientSharedObjectCache::RemoveListener
	eSOCacheEvent_ListenerRemoved = 6,
};

class CCSPlayerBase_CameraServices {};

class C_PostProcessingVolume
{
public:
	SCHEMA_ADD_FIELD(bool, m_bExposureControl, CXOR("C_PostProcessingVolume->m_bExposureControl"));
	SCHEMA_ADD_FIELD(float, m_flMinExposure, CXOR("C_PostProcessingVolume->m_flMinExposure"));
	SCHEMA_ADD_FIELD(float, m_flMaxExposure, CXOR("C_PostProcessingVolume->m_flMaxExposure"));
	SCHEMA_ADD_FIELD(float, m_flTonemapPercentBrightPixels, CXOR("C_PostProcessingVolume->m_flTonemapPercentBrightPixels"));
	SCHEMA_ADD_FIELD(float, m_flExposureFadeSpeedUp, CXOR("C_PostProcessingVolume->m_flExposureFadeSpeedUp"));
	SCHEMA_ADD_FIELD(float, m_flExposureFadeSpeedDown, CXOR("C_PostProcessingVolume->m_flExposureFadeSpeedDown"));
};

class C_EnvSky {
public:
	SCHEMA_ADD_FIELD(bool, m_bStartDisabled, "C_EnvSky->m_bStartDisabled");
	SCHEMA_ADD_FIELD(Color_t, m_vTintColor, "C_EnvSky->m_vTintColor");
	SCHEMA_ADD_FIELD(Color_t, m_vTintColorLightingOnly, "C_EnvSky->m_vTintColorLightingOnly");
	SCHEMA_ADD_FIELD(float, m_flBrightnessScale, "C_EnvSky->m_flBrightnessScale");
	SCHEMA_ADD_FIELD(int, m_nFogType, "C_EnvSky->m_nFogType");
	SCHEMA_ADD_FIELD(float, m_flFogMinStart, "C_EnvSky->m_flFogMinStart");
	SCHEMA_ADD_FIELD(float, m_flFogMinEnd, "C_EnvSky->m_flFogMinEnd");
	SCHEMA_ADD_FIELD(float, m_flFogMaxStart, "C_EnvSky->m_flFogMaxStart");
	SCHEMA_ADD_FIELD(float, m_flFogMaxEnd, "C_EnvSky->m_flFogMaxEnd");
	SCHEMA_ADD_FIELD(bool, m_bEnabled, "C_EnvSky->m_bEnabled");
};

class CSceneLightObject {
public:
	char pad_0000[0xE4]; // 0x0
	float r, g, b; // 0xE4
};

class CPlayer_CameraServices : public CCSPlayerBase_CameraServices
{
public:
	SCHEMA_ADD_FIELD(C_PostProcessingVolume*, m_active_post_processing_volume, CXOR("CPlayer_CameraServices->m_hActivePostProcessingVolume"));
	SCHEMA_ADD_FIELD(C_PostProcessingVolume*, m_post_processing_volumes, CXOR("CPlayer_CameraServices->m_PostProcessingVolumes"));



};


class CPlayer_WeaponServices : public C_BaseModelEntity
{
public:
	CS_CLASS_NO_INITIALIZER(CPlayer_WeaponServices);

	SCHEMA_ADD_FIELD(GameTime_t, m_flNextAttack, "CPlayer_WeaponServices->m_flNextAttack");
	SCHEMA_ADD_FIELD(CBaseHandle, m_hActiveWeapon, "CPlayer_WeaponServices->m_hActiveWeapon");
	SCHEMA_ADD_FIELD(uint16_t[32], m_iAmmo, "CPlayer_WeaponServices->m_iAmmo");

	Vector_t get_shoot_pos() {
		Vector_t temp;
		MEM::CallVFunc<void, 24>(this, &temp);
		return temp;
	}

};

class CCSPlayer_ViewModelServices {
public:

	SCHEMA_ADD_FIELD(CBaseHandle, m_hViewModel, "CCSPlayer_ViewModelServices->m_hViewModel");

};
class CSharedObject;

class CAnimGraphNetworkedVariables;

class CAnimationGraphInstance {
public:
	char pad_0x0000[0x2E0]; //0x0000
	CAnimGraphNetworkedVariables* pAnimGraphNetworkedVariables; //0x02E0 
};

class C_BaseViewModel : public C_BaseModelEntity {
public:
	SCHEMA_ADD_FIELD(CBaseHandle, m_hWeapon, "C_BaseViewModel->m_hWeapon");

};

class C_CSGOViewModel : public C_BaseViewModel {
public:
	char pad_0x0000[0xD00]; //0x0000
	CAnimationGraphInstance* pAnimationGraphInstance; //0x0D08 
};
class CGCClientSharedObjectTypeCache {
public:
	auto AddObject(CSharedObject* pObject) {
		return MEM::CallVFunc<bool, 1u>(this, pObject);
	}

	auto RemoveObject(CSharedObject* soIndex) {
		return MEM::CallVFunc<CSharedObject*, 3u>(this, soIndex);

	}

	template <typename T>
	auto& GetVecObjects() {
		return *reinterpret_cast<CUtlVector<T>*>((uintptr_t)(this) + 0x8);
	}
};

class CCSInputMessage
{
public:
	int32_t m_frame_tick_count; //0x0000
	float m_frame_tick_fraction; //0x0004
	int32_t m_player_tick_count; //0x0008
	float m_player_tick_fraction; //0x000C
	Vector_t m_view_angles; //0x0010
	Vector_t m_shoot_position; //0x001C
	int32_t m_target_index; //0x0028
	Vector_t m_target_head_position; //0x002C
	Vector_t m_target_abs_origin; //0x0038
	Vector_t m_target_angle; //0x0044
	int32_t m_sv_show_hit_registration; //0x0050
	int32_t m_entry_index_max; //0x0054
	int32_t m_index_idk; //0x0058
}; //Size: 0x005C


class CGCClientSharedObjectCache {
public:
	CGCClientSharedObjectTypeCache* CreateBaseTypeCache(int nClassID);
};
struct SOID_t {
	uint64_t m_id;
	uint32_t m_type;
	uint32_t m_padding;
};

class CGCClient {
public:
	CGCClientSharedObjectCache* FindSOCache(SOID_t ID,
		bool bCreateIfMissing = true);
};

class CEconItem;

class C_EconItemView {
public:
	CS_CLASS_NO_INITIALIZER(C_EconItemView);
	auto GetCustomPaintKitIndex() { return MEM::CallVFunc<int, 2u>(this); }

	SCHEMA_ADD_FIELD(char[161], GetCustomNameOverride, "C_EconItemView->m_szCustomNameOverride");

	SCHEMA_ADD_FIELD(uint32_t, m_iAccountID, "C_EconItemView->m_iAccountID");
	SCHEMA_ADD_FIELD(uint32_t, m_iItemIDHigh, "C_EconItemView->m_iItemIDHigh");
	SCHEMA_ADD_FIELD(bool, m_bDisallowSOC, "C_EconItemView->m_bDisallowSOC");
	SCHEMA_ADD_FIELD(uint32_t, m_iItemIDLow, "C_EconItemView->m_iItemIDLow");
	SCHEMA_ADD_FIELD(bool, m_bInitialized, "C_EconItemView->m_bInitialized");
	SCHEMA_ADD_OFFSET(bool, m_bIsStoreItem, 0x1EA);
	SCHEMA_ADD_OFFSET(bool, m_bIsTradeItem, 0x1EA);
	SCHEMA_ADD_FIELD(uint64_t, m_iItemID, "C_EconItemView->m_iItemID");
	SCHEMA_ADD_FIELD(uint64_t, m_iItemDefinitionIndex, "C_EconItemView->m_iItemDefinitionIndex");
	SCHEMA_ADD_FIELD(const char*, m_szCustomName, "C_EconItemView->m_szCustomName");
	CEconItem* GetSOCData(CCSPlayerInventory* sdfsdf);

	auto GetStaticData()
	{
		return MEM::CallVFunc<CEconItemDefinition*, 13u>(this);
	}

};

class CAttributeManager {
public:
	CS_CLASS_NO_INITIALIZER(CAttributeManager);

	SCHEMA_ADD_PFIELD(C_EconItemView, m_Item, "C_AttributeContainer->m_Item");
};

class C_EconEntity : public C_BaseModelEntity {
public:
	CS_CLASS_NO_INITIALIZER(C_EconEntity);
	/* offset */
	SCHEMA_ADD_PFIELD(CAttributeManager, m_AttributeManager, "C_EconEntity->m_AttributeManager");
	SCHEMA_ADD_FIELD(uint32_t, m_OriginalOwnerXuidLow, "C_EconEntity->m_OriginalOwnerXuidLow");
	SCHEMA_ADD_FIELD(uint32_t, m_OriginalOwnerXuidHigh, "C_EconEntity->m_OriginalOwnerXuidHigh");
	SCHEMA_ADD_OFFSET(bool, m_bShouldGlow, 0x15B8);

	uint64_t GetOriginalOwnerXuid() {
		return ((uint64_t)(m_OriginalOwnerXuidHigh()) << 32) |
			m_OriginalOwnerXuidLow();
	}
	auto IsBasePlayerWeapon()
	{
		return MEM::CallVFunc<bool, 156u>(this);
	}
};


class CBasePlayerWeapon : public C_EconEntity {
public:
	CS_CLASS_NO_INITIALIZER(CBasePlayerWeapon);

	SCHEMA_ADD_FIELD(GameTick_t, m_nNextPrimaryAttackTick, "C_BasePlayerWeapon->m_nNextPrimaryAttackTick");
	SCHEMA_ADD_FIELD(float, m_flNextPrimaryAttackTickRatio, "C_BasePlayerWeapon->m_flNextPrimaryAttackTickRatio");
	SCHEMA_ADD_FIELD(GameTick_t, m_nNextSecondaryAttackTick, "C_BasePlayerWeapon->m_nNextSecondaryAttackTick");
	SCHEMA_ADD_FIELD(float, m_flNextSecondaryAttackTickRatio, "C_BasePlayerWeapon->m_flNextSecondaryAttackTickRatio");
	SCHEMA_ADD_OFFSET(float, m_flRecoilIndex, 0x172C);

	SCHEMA_ADD_FIELD(int32_t, clip1, "C_BasePlayerWeapon->m_iClip1");
	BasePlayerWeaponVData* GetVData() noexcept {
		return m_pVDataBase()->as<BasePlayerWeaponVData>();
	}
};

enum CSWeaponMode : int /* it works atleast */
{
	Primary_Mode = 0,
	Secondary_Mode = 1,
	WeaponMode_MAX = 2,
};
//xored
// Created with ReClass.NET by KN4CK3R

class unknknownpointer
{
public:
	char pad_0000[328]; //0x0000
}; //Size: 0x0148

class N000001D5
{
public:
	char pad_0000[72]; //0x0000
}; //Size: 0x0048

class N0000008F
{
public:
	char pad_0000[72]; //0x0000
}; //Size: 0x0048

class N000001E0
{
public:
	char pad_0000[1352]; //0x0000
}; //Size: 0x0548

class N0000028B
{
public:
	char pad_0000[1096]; //0x0000
}; //Size: 0x0448

class IPrediction
{
public:
	void* N00000001; //0x0000
	char pad_0008[32]; //0x0008
	void* N00000006; //0x0028
	char pad_0030[152]; //0x0030
	float LocalPlayerUnkPos; //0x00C8
	float timesinceinair; //0x00CC
	char pad_00D0[24]; //0x00D0
	float N0000001E; //0x00E8
	char pad_00EC[220]; //0x00EC
	void* buttonstates; //0x01C8
	void* N0000003B; //0x01D0
	void* N0000003C; //0x01D8
	char pad_01E0[240]; //0x01E0
	Vector_t parentidxorigin; //0x02D0
	char pad_02DC[96]; //0x02DC
	uint64_t parentindex; //0x033C
	char pad_0344[96]; //0x0344
	float m_flRadius; //0x03A4
	char pad_03A8[4252]; //0x03A8
}; //Size: 0x1444


class CNetInputMessage;

class C_CSWeaponBase : public CBasePlayerWeapon {
public:
	void AddStattrakEntity();
	void AddNametagEntity();

	SCHEMA_ADD_OFFSET(CCSWeaponBaseVData*, datawep, 0x380);
	SCHEMA_ADD_OFFSET(int32_t, m_zoomLevel, 0x1B20);
	SCHEMA_ADD_OFFSET(bool, m_bInReload, 0x17F8);
	SCHEMA_ADD_FIELD(float, m_fAccuracyPenalty, "C_CSWeaponBase->m_fAccuracyPenalty");
	SCHEMA_ADD_FIELD(CSWeaponMode, m_weaponMode, CXOR("C_CSWeaponBase->m_weaponMode"));
	SCHEMA_ADD_FIELD(int, m_iOriginalTeamNumber, CXOR("C_CSWeaponBase->m_iOriginalTeamNumber"));
	SCHEMA_ADD_FIELD(int, m_nPostponeFireReadyTicks, CXOR("C_CSWeaponBase->m_nPostponeFireReadyTicks"));
	SCHEMA_ADD_FIELD(bool, m_bUIWeapon, CXOR("C_CSWeaponBase->m_bUIWeapon"));
	SCHEMA_ADD_FIELD(float, GetWatTickOffset, "C_CSWeaponBase->m_flWatTickOffset");

	SCHEMA_ADD_OFFSET(int, poop_game, 0x5C1);

	__forceinline float get_spread() {
		using fn_get_spread_t = float(CS_FASTCALL*)(void*);
		static fn_get_spread_t fn = reinterpret_cast<fn_get_spread_t>(MEM::FindPattern(CLIENT_DLL, CXOR("48 83 EC ? 48 63 91")));

		return fn(this);
	}
	void UpdateSubClass()
	{
		using fnGetPlayerPawn = void(__fastcall*)(void*);
		static auto GetPlayerPawnf = reinterpret_cast<fnGetPlayerPawn>(MEM::FindPattern(CLIENT_DLL, ("40 53 48 83 EC 30 48 8B 41 10 48 8B D9 8B 50 30")));
		GetPlayerPawnf(this);
	}
	__forceinline float get_inaccuracy() {
		using fn_get_inaccuracy_t = float(CS_FASTCALL*)(void*, float*, float*);
		static fn_get_inaccuracy_t fn = reinterpret_cast<fn_get_inaccuracy_t>(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 44 0F 29 84 24")));
		float x{}, y{};
		return fn(this, &x, &y);
	}
	void UpdateVData()
	{
		MEM::CallVFunc<void*, 182>(this);
	}
	void update_accuracy_penality() {
		using fn_update_accuracy_penality_t = void(CS_FASTCALL*)(void*);
		static auto fn = reinterpret_cast<fn_update_accuracy_penality_t>(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 48 8B D8 48 85 C0 0F 84 ? ? ? ? 44 0F 29 44 24")));
		return fn(this);
	}

	__forceinline float get_max_speed() {
		using original_fn = float(CS_FASTCALL*)(void*);
		return (*(original_fn**)this)[338](this);
	}
};

class C_BaseCSGrenade : public C_CSWeaponBase
{
public:
	SCHEMA_ADD_FIELD(bool, IsHeldByPlayer, "C_BaseCSGrenade->m_bIsHeldByPlayer");
	SCHEMA_ADD_FIELD(bool, IsPinPulled, "C_BaseCSGrenade->m_bPinPulled");
	SCHEMA_ADD_FIELD(float, GetThrowTime, "C_BaseCSGrenade->m_fThrowTime");
	SCHEMA_ADD_FIELD(float, GetThrowStrength, "C_BaseCSGrenade->m_flThrowStrength");
};

class C_CSPlayerPawnBase : public C_BaseEntity
{
public:
	CS_CLASS_NO_INITIALIZER(C_CSPlayerPawnBase);

	SCHEMA_ADD_FIELD(CCSPlayer_ViewModelServices*, GetViewModelServices, CXOR("C_CSPlayerPawnBase->m_pViewModelServices"));
	SCHEMA_ADD_FIELD(bool, IsScoped, "C_CSPlayerPawn->m_bIsScoped");
	SCHEMA_ADD_FIELD(bool, IsDefusing, "C_CSPlayerPawn->m_bIsDefusing");
	SCHEMA_ADD_FIELD(bool, IsGrabbingHostage, CXOR("C_CSPlayerPawn->m_bIsGrabbingHostage"));
	SCHEMA_ADD_FIELD(float, GetLowerBodyYawTarget, CXOR("C_CSPlayerPawnBase->m_flLowerBodyYawTarget"));
	SCHEMA_ADD_FIELD(float, GetFlashMaxAlpha, CXOR("C_CSPlayerPawnBase->m_flFlashMaxAlpha"));
	SCHEMA_ADD_FIELD(float, GetFlashDuration, CXOR("C_CSPlayerPawnBase->m_flFlashDuration"));
	SCHEMA_ADD_FIELD(Vector_t, GetLastSmokeOverlayColor, CXOR("C_CSPlayerPawnBase->m_vLastSmokeOverlayColor"));
	SCHEMA_ADD_FIELD(int, GetSurvivalTeam, CXOR("C_CSPlayerPawnBase->m_nSurvivalTeam")); // danger zone
	SCHEMA_ADD_OFFSET(bool, m_bInLanding, 0x17C0);
	SCHEMA_ADD_FIELD(float, m_flLandingTime, CXOR("C_CSPlayerPawnBase->m_flLandingTime"));
};

class CBaseCSGrenade /* "server" */ : public C_CSPlayerPawnBase /* "server" */
{
public:

	SCHEMA_ADD_FIELD(float, m_fThrowTime, CXOR("CBaseCSGrenade->m_fThrowTime"));
};

class CCSPlayer_MovementServices
{
public:
	CS_CLASS_NO_INITIALIZER(CCSPlayer_MovementServices);

	SCHEMA_ADD_FIELD(float, m_flStamina, CXOR("CCSPlayer_MovementServices->m_flStamina"));
};

class CPlayer_MovementServices
{
public:
	CS_CLASS_NO_INITIALIZER(CPlayer_MovementServices);

	SCHEMA_ADD_FIELD(float, m_flMaxspeed, CXOR("CPlayer_MovementServices->m_flMaxspeed"));
	SCHEMA_ADD_FIELD(float, m_flForwardMove, CXOR("CPlayer_MovementServices->m_flForwardMove"));
	SCHEMA_ADD_FIELD(float, m_flLeftMove, CXOR("CPlayer_MovementServices->m_flLeftMove"));
	SCHEMA_ADD_FIELD(bool, m_bDucking, CXOR("CPlayer_MovementServices_Humanoid->m_bDucking"));
	SCHEMA_ADD_FIELD(bool, m_flSurfaceFriction, CXOR("CPlayer_MovementServices_Humanoid->m_flSurfaceFriction"));

	void set_prediction_command(CUserCmd* cmd)
	{
		return MEM::CallVFunc<void, 36U>(this, cmd);
	}

	void reset_prediction_command()
	{
		return MEM::CallVFunc<void, 37U>(this);
	}
};

class CPlayer_ItemServices {
public:
	SCHEMA_ADD_FIELD(bool, m_bHasDefuser, CXOR("CCSPlayer_ItemServices->m_bHasDefuser"));
	SCHEMA_ADD_FIELD(bool, m_bHasHelmet, CXOR("CCSPlayer_ItemServices->m_bHasHelmet"));
	SCHEMA_ADD_FIELD(bool, m_bHasHeavyArmor, CXOR("CCSPlayer_ItemServices->m_bHasHeavyArmor"));
};

class PostProcessingTonemapParameters_t
{
public:
	CS_CLASS_NO_INITIALIZER(PostProcessingTonemapParameters_t);

	SCHEMA_ADD_OFFSET(float, m_flExposureBias, 0x0);
	SCHEMA_ADD_OFFSET(float, m_flShoulderStrength, 0x4);
	SCHEMA_ADD_OFFSET(float, m_flLinearStrength, 0x8);
	SCHEMA_ADD_OFFSET(float, m_flLinearAngle, 0xC);
	SCHEMA_ADD_OFFSET(float, m_flToeStrength, 0x10);
	SCHEMA_ADD_OFFSET(float, m_flToeNum, 0x14);
	SCHEMA_ADD_OFFSET(float, m_flToeDenom, 0x18);
	SCHEMA_ADD_OFFSET(float, m_flWhitePoint, 0x1C);
	SCHEMA_ADD_OFFSET(float, m_flLuminanceSource, 0x20);
	SCHEMA_ADD_OFFSET(float, m_flExposureBiasShadows, 0x24);
	SCHEMA_ADD_OFFSET(float, m_flExposureBiasHighlights, 0x28);
	SCHEMA_ADD_OFFSET(float, m_flMinShadowLum, 0x2C);
	SCHEMA_ADD_OFFSET(float, m_flMaxShadowLum, 0x30);
	SCHEMA_ADD_OFFSET(float, m_flMinHighlightLum, 0x34);
	SCHEMA_ADD_OFFSET(float, m_flMaxHighlightLum, 0x38);

};

class PostProcessingResource_t {
public:
	CS_CLASS_NO_INITIALIZER(PostProcessingResource_t);

	SCHEMA_ADD_OFFSET(PostProcessingTonemapParameters_t, m_toneMapParams, 0x4);

};

struct bullet_info_t
{

	bullet_info_t()
	{
		position = Vector_t(0, 0, 0);
		time_stamp = 0;
		expire_time = -1;
	}

	bullet_info_t(Vector_t position, float time_stamp, float expire_time)
	{
		position = position;
		time_stamp = time_stamp;
		expire_time = expire_time;
	}

	Vector_t	position{};
	float	time_stamp{};
	float	expire_time{};
};

class CPlayer_BulletServices
{
public:
	int m_bullet_count()
	{
		return *reinterpret_cast<int*>(std::uintptr_t(this) + 0x48);
	}
	CUtlVector<bullet_info_t>& m_bullet_data()
	{
		return *reinterpret_cast<CUtlVector<bullet_info_t>*>(std::uintptr_t(this) + 0x48);
	}
};

class C_CSPlayerPawn : public C_CSPlayerPawnBase
{
public:
	CS_CLASS_NO_INITIALIZER(C_CSPlayerPawn);


	SCHEMA_ADD_OFFSET(bool, m_bNeedToReApplyGloves, 0x1670);
	SCHEMA_ADD_OFFSET(C_EconItemView, m_EconGloves, 0x1678);
	SCHEMA_ADD_FIELD(uint8_t, m_nEconGlovesChanged, CXOR("C_CSPlayerPawnBase->m_nEconGlovesChanged"));
	SCHEMA_ADD_FIELD(bool, m_bGunGameImmunity, CXOR("C_CSPlayerPawnBase->m_bGunGameImmunity"));
	SCHEMA_ADD_FIELD(bool, m_bIsWalking, CXOR("C_CSPlayerPawnBase->m_bIsWalking"));
	SCHEMA_ADD_FIELD(CBaseHandle, GetController, CXOR("C_BasePlayerPawn->m_hController"));
	SCHEMA_ADD_FIELD(std::int32_t, GetArmorValue, CXOR("C_CSPlayerPawn->m_ArmorValue"));
	SCHEMA_ADD_FIELD(CPlayer_WeaponServices*, GetWeaponServices, CXOR("C_BasePlayerPawn->m_pWeaponServices"));
	SCHEMA_ADD_FIELD(CPlayer_BulletServices*, GetBulletServices, CXOR("C_CSPlayerPawn->m_pBulletServices"));
	//SCHEMA_ADD_OFFSET(CPlayer_BulletServices*, GetBulletServices, 0x1468);
	SCHEMA_ADD_FIELD(CPlayer_ItemServices*, GetItemServices, CXOR("C_BasePlayerPawn->m_pItemServices"));
	SCHEMA_ADD_FIELD(CPlayer_CameraServices*, GetCameraServices, CXOR("C_BasePlayerPawn->m_pCameraServices"));
	SCHEMA_ADD_FIELD(CPlayer_MovementServices*, m_pMovementServices, CXOR("C_BasePlayerPawn->m_pMovementServices"));
	SCHEMA_ADD_FIELD(bool, m_bWaitForNoAttack, CXOR("C_CSPlayerPawn->m_bWaitForNoAttack"));
	SCHEMA_ADD_OFFSET(CUtlVectorCS2< QAngle_t >, m_aimPunchCache, 0x1598);
	SCHEMA_ADD_FIELD(QAngle_t, GetAimPuchAngle, "C_CSPlayerPawn->m_aimPunchAngle");
	SCHEMA_ADD_FIELD(int, GetShotsFired, CXOR("C_CSPlayerPawn->m_iShotsFired"));

	[[nodiscard]] Vector_t GetBonePosition(int out);
	[[nodiscard]] bool hasArmour(const int hitgroup);
	[[nodiscard]] bool CalculateBoundingBox(ImVec4& bbox, bool compute_surrounding_box);
	[[nodiscard]] bool IsOtherEnemy(C_CSPlayerPawn* pOther);
	[[nodiscard]] int GetAssociatedTeam();
	[[nodiscard]] void CalculateHitboxData(C_CSPlayerPawn* ent, uint32_t idx, Vector_t& pos, Vector4D_t& rot, float& scale, bool predict = false);
	Vector_t extrapolate(C_CSPlayerPawn* ent, Vector_t pos, Vector_t vel);
	float GetProperAccuracy();
	float GetProperSpread();
	uint32_t GetHitGroup(int idx);
	[[nodiscard]] Vector_t GetEyePosition()
	{
		Vector_t vecEyePosition = Vector_t(0.0f, 0.0f, 0.0f);
		// Credit: https://www.unknowncheats.me/forum/4258133-post6228.html
		MEM::CallVFunc<void, 169U>(this, &vecEyePosition);
		return vecEyePosition;
	}

	float speed_move()
	{
		return *(float*)(std::uintptr_t(this) + 0x3D8);
	}

	int get_bone_index(const char* name)
	{
		using function_t = int(CS_FASTCALL*)(void*, const char*);
		static auto fn = (function_t)MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, CXOR("E8 ? ? ? ? 45 33 FF 89 47")), 0x1);

		if (!fn)
			return 0;
		else
			return fn(this, name);
	}

	unsigned int GetEntHandle(C_CSPlayerPawn* ent)
	{
		using function_t = unsigned int(CS_FASTCALL*)(C_CSPlayerPawn*);
		static function_t fn = reinterpret_cast<function_t>(MEM::FindPattern(CLIENT_DLL, CXOR("48 85 C9 74 32 48 8B 41 10 48 85 C0 74 29 44")));
		CS_ASSERT(fn != nullptr);
		return fn(ent);
	}

	bool is_throwing() {
		auto* active_weapon = this->ActiveWeapon();

		if (!active_weapon)
			return false;

		C_BaseCSGrenade* grenade = reinterpret_cast<C_BaseCSGrenade*>(active_weapon);
		auto* weapon_data = active_weapon->datawep();

		if (!grenade || !weapon_data)
			return false;

		if (weapon_data->m_WeaponType() == WEAPONTYPE_GRENADE && grenade->GetThrowTime() != 0.f)
			return true;

		return false;
	}

	short get_active_item_definition() {
		auto* active_weapon = this->ActiveWeapon();
		if (active_weapon) {
			CAttributeManager* attribute_manager = active_weapon->m_AttributeManager();
			if (!attribute_manager)
				return -1;

			C_EconItemView* item = attribute_manager->m_Item();
			if (!item)
				return -1;

			return item->m_iItemDefinitionIndex();
		}

		return -1;
	}

	[[nodiscard]] bool CanShoot(float time) const;
	[[nodiscard]] bool CanNextAttack(float time);

	uint32_t GetHitbox(int idx);

	[[nodiscard]] C_CSWeaponBase* ActiveWeapon();
};

class CGlowProperty {
private:
	char pad_0000[0x18]; // 0x0
public:
	CEntityInstance* m_hOwner;

	SCHEMA_ADD_FIELD(Color_t, m_glowColorOverride, "CGlowProperty->m_glowColorOverride");
	SCHEMA_ADD_FIELD(bool, m_bGlowing, "CGlowProperty->m_bGlowing");
};

struct CachedEntity_t {
	enum Type { UNKNOWN = 0, PLAYER_CONTROLLER, BASE_WEAPON, CHICKEN };

	CBaseHandle m_handle;
	Type m_type;
	int index;
	bool reset;
	// std::unordered_map<int, std::unique_ptr<backtrack_entity>> record{ };
	ImVec4 m_bbox;
	bool m_draw;  // If the player is not visible, it will be false.
	Vector_t hitboxpos[30];
};

// typedef for entity callback
using entity_callback = void(*)(int index, const CBaseHandle handle) noexcept;
struct entity_callbacks {
	entity_callback add;
	entity_callback remove;
};

extern std::vector<CachedEntity_t> g_cachedEntities;
extern std::mutex g_cachedEntitiesMutex;
extern std::unordered_multimap<CachedEntity_t::Type, entity_callbacks> g_Callbacks;

namespace EntCache {
	void CacheCurrentEntities();
	void OnAddEntity(CEntityInstance* pEntity, CBaseHandle handle);
	void OnRemoveEntity(CEntityInstance* pEntity, CBaseHandle handle);
	void RegisterCallback(CachedEntity_t::Type type, entity_callback add, entity_callback remove) noexcept;
}  // namespace EntCache

class CBasePlayerController : public C_BaseModelEntity {
public:
	CS_CLASS_NO_INITIALIZER(CBasePlayerController);

	SCHEMA_ADD_FIELD(std::uint64_t, GetSteamId, CXOR("CBasePlayerController->m_steamID"));
	SCHEMA_ADD_FIELD(CBaseHandle, GetPawnHandle, CXOR("CBasePlayerController->m_hPawn"));
	SCHEMA_ADD_FIELD(bool, IsLocalPlayerController, CXOR("CBasePlayerController->m_bIsLocalPlayerController"));
};


class CCSPlayerController : public CBasePlayerController {
public:
	CS_CLASS_NO_INITIALIZER(CCSPlayerController);

	[[nodiscard]] static CCSPlayerController* GetLocalPlayerController();

	bool IsThrowingGrenade(C_CSWeaponBase* pBaseWeapon);

	// @note: always get origin from pawn not controller
	[[nodiscard]] const Vector_t& GetPawnOrigin();

	[[nodiscard]] C_CSWeaponBase* GetPlayerWeapon(C_CSPlayerPawn* pPlayer);
	C_CSPlayerPawn* get_player_pawn();

	SCHEMA_ADD_FIELD(std::uint32_t, GetPing, CXOR("CCSPlayerController->m_iPing"));
	SCHEMA_ADD_FIELD(const char*, GetPlayerName, CXOR("CCSPlayerController->m_sSanitizedPlayerName"));
	SCHEMA_ADD_FIELD(std::int32_t, GetPawnHealth, CXOR("CCSPlayerController->m_iPawnHealth"));
	SCHEMA_ADD_FIELD(std::int32_t, GetPawnArmor, CXOR("CCSPlayerController->m_iPawnArmor"));
	SCHEMA_ADD_FIELD(bool, m_bPawnHasDefuser, CXOR("CCSPlayerController->m_bPawnHasDefuser"));
	SCHEMA_ADD_FIELD(bool, m_bPawnHasHelmet, CXOR("CCSPlayerController->m_bPawnHasHelmet"));
	SCHEMA_ADD_FIELD(bool, IsPawnAlive, CXOR("CCSPlayerController->m_bPawnIsAlive"));
	SCHEMA_ADD_FIELD(CBaseHandle, GetPlayerPawnHandle, CXOR("CCSPlayerController->m_hPlayerPawn"));
	SCHEMA_ADD_FIELD(CBaseHandle, m_hPawn, CXOR("CBasePlayerController->m_hPawn"));
	SCHEMA_ADD_FIELD(uint32_t, m_nTickBase, CXOR("CBasePlayerController->m_nTickBase"));
	SCHEMA_ADD_FIELD(CBaseHandle, m_hPredictedPawn, CXOR("CBasePlayerController->m_hPredictedPawn"));

	CUserCmd*& m_current_command() {
		return *(CUserCmd**)(std::uintptr_t(this) + 0x6B8);
	}

	void physics_run_think() {
		static auto physics_run_think_ = reinterpret_cast<void* (CS_FASTCALL*)(void*)>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, CXOR("E8 ? ? ? ? 49 8B D6 48 8B CE E8 ? ? ? ? 48 8B 06")), 0x1));
		physics_run_think_(this);
	}
};

class CC_GradientFog {
public:
	SCHEMA_ADD_FIELD(bool, m_bIsEnabled, CXOR("C_GradientFog->m_bIsEnabled"));
	SCHEMA_ADD_FIELD(float, m_flFogStartDistance, CXOR("C_GradientFog->m_flFogStartDistance"));
	SCHEMA_ADD_FIELD(float, m_flFogEndDistance, CXOR("C_GradientFog->m_flFogEndDistance"));
	SCHEMA_ADD_FIELD(float, m_flFogStrength, CXOR("C_GradientFog->m_flFogStrength"));
	SCHEMA_ADD_FIELD(float, m_flFogFalloffExponent, CXOR("C_GradientFog->m_flFogFalloffExponent"));
	SCHEMA_ADD_FIELD(Color_t, m_fogColor, CXOR("C_GradientFog->m_fogColor"));
	SCHEMA_ADD_FIELD(float, m_flFogMaxOpacity, CXOR("C_GradientFog->m_flFogMaxOpacity"));
};