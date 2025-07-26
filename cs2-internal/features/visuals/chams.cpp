#include "chams.h"

#include "../../sdk/interfaces/imaterialsystem.h"
#include "../../sdk/interfaces/igameresourceservice.h"
#include "../../sdk/interfaces/cgameentitysystem.h"
#include "../../sdk/entity.h"
#include "../../sdk/datatypes/resourceutils.h"
#include "../../sdk/datatypes/buf/utlbuffer.h"
#include "../../sdk/interfaces/imaterialsystem.h"

#include <iostream>
#include <stdexcept>

#include "../../core/hooks.h"
#include "../../core/sdk.h"
#include "../../core/interfaces.h"

#include "../../features/configs/configs_var.h"

struct custom_material_data_t {
	material2_t* m_material;
	material2_t* m_material_z;
};

enum material_list_t {
	material_white,
	material_default,
	material_illum,
	material_size
};

static custom_material_data_t array_materials[VISUAL_MATERIAL_MAX];

bool ams::chams_t::initialize() 
{
	if (this->m_initialized)
		return this->m_initialized;

	array_materials[VISUAL_MATERIAL_SOLID] = custom_material_data_t{
		.m_material = CreateMaterial(CXOR("materials/dev/primary_white.vmat"), szVMatBufferSolidVisible),
		.m_material_z = CreateMaterial(CXOR("materials/dev/primary_white.vmat"), szVMatBufferSolidInvisible)
	};

	array_materials[VISUAL_MATERIAL_FLAT] = custom_material_data_t{
		.m_material = CreateMaterial(CXOR("materials/dev/primary_white.vmat"), szVMatBufferWhiteVisible),
		.m_material_z = CreateMaterial(CXOR("materials/dev/primary_white.vmat"), szVMatBufferWhiteInvisible)
	};

	array_materials[VISUAL_MATERIAL_GLOW] = custom_material_data_t{
		.m_material = CreateMaterial(CXOR("materials/dev/glow.vmat"), szVMatBufferIlluminateVisible),
		.m_material_z = CreateMaterial(CXOR("materials/dev/glow_invisible.vmat"), szVMatBufferIlluminateInvisible)
	};

	array_materials[VISUAL_MATERIAL_LATEX] = custom_material_data_t{
		.m_material = CreateMaterial(CXOR("materials/dev/primary_white.vmat"), szVMatBufferLatexVisible),
		.m_material_z = CreateMaterial(CXOR("materials/dev/primary_white.vmat"), szVMatBufferLatexInvisible)
	};

	array_materials[VISUAL_MATERIAL_STAR] = custom_material_data_t{
		.m_material = CreateMaterial(CXOR("materials/dev/star.vmat"), szVMatBufferStarVisible),
		.m_material_z = CreateMaterial(CXOR("materials/dev/star_invisible.vmat"), szVMatBufferStarInvisible)
	};

	array_materials[VISUAL_MATERIAL_ELECTRIC] = custom_material_data_t{
		.m_material = CreateMaterial(CXOR("materials/dev/electric.vmat"), szVMatBufferElectricVisible),
		.m_material_z = CreateMaterial(CXOR("materials/dev/electric_invisible.vmat"), szVMatBufferElectricInvisible)
	};

	this->m_initialized = true;

	for (auto& [m_material, m_material_z] : array_materials)
	{
		if (m_material == nullptr || m_material_z == nullptr)
			this->m_initialized = false;
	}

	return this->m_initialized;
}

bool ams::chams_t::draw_object(void* animatable_object, void* dx11, material_data_t* arr_material_data, int data_count, void* scene_view, void* scene_layer, void* unk1, void* unk2) 
{
	if (!this->m_initialized)
		return false;

	if (!SDK::LocalPawn)
		return false;

	if (arr_material_data == nullptr)
		return false;

	if (arr_material_data->m_scene_animable == nullptr)
		return false;

	CBaseHandle m_owner = arr_material_data->m_scene_animable->m_owner;

	auto m_entity = I::GameResourceService->pGameEntitySystem->Get< C_BaseEntity >(m_owner);
	if (m_entity == nullptr)
		return false;

	SchemaClassInfoData_t* m_class_info;
	m_entity->GetSchemaClassInfo(&m_class_info);
	if (m_class_info == nullptr)
		return false;

	if (CRT::StringCompare(m_class_info->szName, CXOR("C_CSPlayerPawn")) == 0)
	{
		auto m_pawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(m_owner);
		if (m_pawn == nullptr)
			return false;

		bool bIsLocalPlayer = (m_pawn == SDK::LocalPawn);

		if (!cfg_var::m_cfg->g_visuals.chams_local.chamLocal && bIsLocalPlayer)
			return false;

		if (!bIsLocalPlayer && !m_pawn->IsOtherEnemy(SDK::LocalPawn))
			return false;

		return this->override_material(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
	}

	if (cfg_var::m_cfg->g_visuals.chams_local.weapcham_local && m_entity->IsWeapon())
	{
		auto pOwnerEntity = I::GameResourceService->pGameEntitySystem->Get<C_BaseEntity>(m_entity->GetOwnerHandle());
		if (pOwnerEntity == nullptr)
			return false;

		if (pOwnerEntity && pOwnerEntity == SDK::LocalPawn)
			return this->override_material(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
	}

	if (cfg_var::m_cfg->g_visuals.chams_local.weapcham_local && m_entity->IsViewModel())
	{
		auto pOwnerEntity = I::GameResourceService->pGameEntitySystem->Get<C_BaseEntity>(m_entity->GetOwnerHandle());
		if (pOwnerEntity == nullptr)
			return false;

		if (pOwnerEntity && pOwnerEntity == SDK::LocalPawn)
			return this->override_material(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
	}

	return false;
}

material2_t* ams::chams_t::CreateMaterial(const char* szMaterialName, const char szVmatBuffer[])
{
	CKeyValues3* pKeyValues3 = CKeyValues3::CreateMaterialResource();
	pKeyValues3->LoadFromBuffer(szVmatBuffer);

	material2_t** pCustomMaterial = {};
	MEM::fnCreateMaterial(nullptr, &pCustomMaterial, szMaterialName, pKeyValues3, 0, 1);

	return *pCustomMaterial;
}

bool ams::chams_t::override_material(void* animatable_object, void* dx11, material_data_t* arr_material_data, int data_count, void* scene_view, void* scene_layer, void* unk1, void* unk2) 
{

	const auto original = H::hkDrawObject.GetOriginal();
	const custom_material_data_t customMaterialEnem = array_materials[cfg_var::m_cfg->g_visuals.chams_enemie.chamstype];
	const custom_material_data_t customMaterialLocal = array_materials[cfg_var::m_cfg->g_visuals.chams_local.chamstype];
	const custom_material_data_t customMaterialWeap = array_materials[cfg_var::m_cfg->g_visuals.chams_local.chamstypeWeap];

	auto pEntity = I::GameResourceService->pGameEntitySystem->Get<C_BaseEntity>(arr_material_data->m_scene_animable->m_owner);
	if (pEntity == nullptr)
		return false;

	SchemaClassInfoData_t* pClassInfo;
	pEntity->GetSchemaClassInfo(&pClassInfo);
	if (pClassInfo == nullptr)
		return false;

	if (CRT::StringCompare(pClassInfo->szName, CXOR("C_CSPlayerPawn")) == 0)
	{
		auto pPlayerPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(arr_material_data->m_scene_animable->m_owner);
		if (pPlayerPawn == nullptr)
			return false;

		bool bIsLocalPlayer = (pPlayerPawn == SDK::LocalPawn);

		if (cfg_var::m_cfg->g_visuals.chams_enemie.invischam && !bIsLocalPlayer)
		{
			arr_material_data->m_material = customMaterialEnem.m_material_z;
			arr_material_data->m_color = cfg_var::m_cfg->g_visuals.chams_enemie.colInvisCham.colValue;
			I::MaterialSystem2->set_color(arr_material_data, cfg_var::m_cfg->g_visuals.chams_enemie.colInvisCham.colValue);

			original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
		}
		if (cfg_var::m_cfg->g_visuals.chams_enemie.vischam && !bIsLocalPlayer)
		{
			arr_material_data->m_material = customMaterialEnem.m_material;
			arr_material_data->m_color = cfg_var::m_cfg->g_visuals.chams_enemie.colVisCham.colValue;
			I::MaterialSystem2->set_color(arr_material_data, cfg_var::m_cfg->g_visuals.chams_enemie.colVisCham.colValue);
			original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
		}
		if (cfg_var::m_cfg->g_visuals.chams_local.chamLocal && bIsLocalPlayer)
		{
			arr_material_data->m_material = customMaterialLocal.m_material_z;
			arr_material_data->m_color = cfg_var::m_cfg->g_visuals.chams_local.colLocalCham.colValue;
			I::MaterialSystem2->set_color(arr_material_data, cfg_var::m_cfg->g_visuals.chams_local.colLocalCham.colValue);

			original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
		}
		if (cfg_var::m_cfg->g_visuals.chams_local.chamLocal && bIsLocalPlayer)
		{
			arr_material_data->m_material = customMaterialLocal.m_material;
			arr_material_data->m_color = cfg_var::m_cfg->g_visuals.chams_local.colLocalCham.colValue;
			I::MaterialSystem2->set_color(arr_material_data, cfg_var::m_cfg->g_visuals.chams_local.colLocalCham.colValue);

			original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
		}
	}
	else if (cfg_var::m_cfg->g_visuals.chams_local.weapcham_local && pEntity->IsWeapon())
	{
		auto pOwnerEntity = I::GameResourceService->pGameEntitySystem->Get<C_BaseEntity>(pEntity->GetOwnerHandle());
		if (pOwnerEntity && pOwnerEntity == SDK::LocalPawn)
		{
			arr_material_data->m_material = customMaterialWeap.m_material;
			arr_material_data->m_color = cfg_var::m_cfg->g_visuals.chams_local.colWeapCham.colValue;
		}

	}
	else if (cfg_var::m_cfg->g_visuals.chams_local.weapcham_local && pEntity->IsViewModel())
	{
		auto pOwnerEntity = I::GameResourceService->pGameEntitySystem->Get<C_BaseEntity>(pEntity->GetOwnerHandle());
		if (pOwnerEntity && pOwnerEntity == SDK::LocalPawn)
		{
			arr_material_data->m_material = customMaterialWeap.m_material;
			arr_material_data->m_color = cfg_var::m_cfg->g_visuals.chams_local.colWeapCham.colValue;
		}
	}

	original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);

	return true;
}