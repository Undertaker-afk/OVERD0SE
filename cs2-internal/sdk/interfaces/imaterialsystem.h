#pragma once

#include <Windows.h>

#include "../../utilities/memory.h"

#include "../datatypes/color.h"
#include "../entity_handle.h"

#include "../datatypes/keyvalue3.h"
#include "../datatypes/vector.h"

class material2_t {
public:
	virtual const char* get_name() = 0;
	virtual const char* get_shader_name() = 0;
};

// material key var
struct material_key_var_t {
	std::uint64_t m_key;
	const char* m_name;

	material_key_var_t(std::uint64_t m_key, const char* m_name) :
		m_key(m_key), m_name(m_name) { }

	material_key_var_t(const char* m_name, bool m_should_find_key = false) :
		m_name(m_name) {
		m_key = m_should_find_key ? find_key(m_name) : 0x0;
	}

	// find key
	std::uint64_t find_key(const char* m_name) {
		// helper ida: CBodyGameSystem::NotifyResourcePreReload
		using function_find_key = std::uint64_t(CS_FASTCALL*)
			(const char*, unsigned int, int);
		static auto find_key_var = reinterpret_cast<function_find_key>(
			MEM::FindPattern(L"particles.dll", CXOR("48 89 5C 24 ? 57 48 81 EC ? ? ? ? 33 C0 8B DA")));

#ifdef CS_PARANOID
		CS_ASSERT(find_key_var != nullptr);
#endif

		return find_key_var(m_name, 0x12, 0x31415926);
	}
};

// object info.
class object_info_t {
	MEM_PAD(0xB0);
	int m_id;
};

class scene_animable_object_t {
	MEM_PAD(0xB8);
	CBaseHandle m_owner;
};

class material_data_t {
public:
	void set_shader_type(const char* szShaderName)
	{
		// @ida: #STR: shader, spritecard.vfx
		using fnSetMaterialShaderType = void(CS_FASTCALL*)(void*, material_key_var_t, const char*, int);
		static auto oSetMaterialShaderType = reinterpret_cast<fnSetMaterialShaderType>(MEM::FindPattern(PARTICLES_DLL, CXOR("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 0F B6 01 45 0F B6 F9 8B 2A 4D 8B E0 4C 8B 72 ? 48 8B F9 C0 E8 ? 24 ? 3C ? 74 ? 41 B0 ? B2 ? E8 ? ? ? ? 0F B6 07 33 DB C0 E8 ? 24 ? 3C ? 75 ? 48 8B 77 ? EB ? 48 8B F3 4C 8D 44 24 ? C7 44 24 ? ? ? ? ? 48 8D 54 24 ? 89 6C 24 ? 48 8B CE 4C 89 74 24 ? E8 ? ? ? ? 8B D0 83 F8 ? 75 ? 45 33 C9 89 6C 24 ? 4C 8D 44 24 ? 4C 89 74 24 ? 48 8B D7 48 8B CE E8 ? ? ? ? 8B D0 0F B6 0F C0 E9 ? 80 E1 ? 80 F9 ? 75 ? 48 8B 4F ? EB ? 48 8B CB 8B 41 ? 85 C0 74 ? 48 8D 59 ? 83 F8 ? 76 ? 48 8B 1B 48 63 C2 4D 85 E4")));

#ifdef CS_PARANOID
		CS_ASSERT(oSetMaterialShaderType != nullptr);
#endif

		material_key_var_t shaderVar(0x162C1777, CXOR("shader"));
		oSetMaterialShaderType(this, shaderVar, szShaderName, 0x18);
	}
	void set_material_function(const char* szFunctionName, int nValue)
	{
		using fnSetMaterialFunction = void(CS_FASTCALL*)(void*, material_key_var_t, int, int);
		static auto oSetMaterialFunction = reinterpret_cast<fnSetMaterialFunction>(MEM::FindPattern(PARTICLES_DLL, CXOR("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 0F B6 01 45 0F B6 F9 8B 2A 48 8B F9")));

#ifdef CS_PARANOID
		CS_ASSERT(oSetMaterialFunction != nullptr);
#endif

		material_key_var_t functionVar(szFunctionName, true);
		oSetMaterialFunction(this, functionVar, nValue, 0x18);
	}

	MEM_PAD(0x18); // 0x0
	scene_animable_object_t* m_scene_animable; // 0x18
	material2_t* m_material; // 0x20
	MEM_PAD(0x28); // 0x28
	Color_t m_color; // 0x40
};

class material_system_t {
public:
	material2_t*** find_or_create_from_resource(material2_t*** m_out_material, const char* m_material_name) {
		return MEM::CallVFunc<material2_t***, 14>(this, m_out_material, m_material_name);
	}

	material2_t*** create_material(material2_t*** m_out_material, const char* m_material_name, material_data_t* m_data) {
		return MEM::CallVFunc<material2_t***, 29>(this, m_out_material, m_material_name, m_data, 0, 0, 0, 0, 0, 1);
	}

	void set_create_data_by_material(const void* m_data, material2_t*** const m_in_material) {
		return MEM::CallVFunc< void, 38 >(this, m_in_material, m_data);
	}

	static void set_color(void* m_data, Color_t m_color) {
		*(byte*)((uintptr_t)m_data + 0x50) = m_color.r;
		*(byte*)((uintptr_t)m_data + 0x51) = m_color.g;
		*(byte*)((uintptr_t)m_data + 0x52) = m_color.b;
		*(byte*)((uintptr_t)m_data + 0x53) = m_color.a;
	}
};