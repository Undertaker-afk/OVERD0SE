#include "../../utilities/memory.h"
#include "../datatypes/color.h"
#include "../entity_handle.h"

#include "..\..\core\sdk.h"
#include "../../sdk/datatypes/stronghandle.hpp"

class color_t;
class Vector_t;

class CParticleManager
{
public:
    void create_snapshot(stronghandle<c_particle_snapshot>* out_particle_snapshot) {
        __int64 nUnknown = 0;
        return MEM::CallVFunc<void, 42U>(this, out_particle_snapshot, &nUnknown);
    }

    void draw(stronghandle<c_particle_snapshot>* particle_snapshot, int count, void* data) {
        return MEM::CallVFunc<void, 43U>(this, particle_snapshot, count, data);
    }
};

class c_particle_snapshot
{
public:
    void draw(int count, void* data)
    {
        return MEM::CallVFunc<void, 1U>(this, count, data);
    }
};

class particle_effect
{
public:
    const char* name{};
    char pad[0x30]{};
};

class particle_information
{
public:
    float time{};
    float width{};
    float unk2{};
};

// This class changed slightly when molotov's where changed.
// I did not reverse this and getting positions and times as pointers like this is most certainly not a big brain move.
// credits: epoxi [https://www.unknowncheats.me/forum/members/5188035.html]
class particle_data
{
public:
    Vector_t* positions{};
    char pad[0x74]{};
    float* times{};
    char pad2[0x28];
    float* times2{};
    char pad3[0x98];
}; // Size: 0x0150

struct particle_color
{
    float r;
    float g;
    float b;
};

class CGameParticleManagerSystem
{
public:
    void create_effect_index(unsigned int* effect_index, particle_effect* effect_data)
    {
        static auto create_effect_index = reinterpret_cast<void(CS_FASTCALL*)(void*, unsigned int*, void*)>(MEM::FindPattern(CLIENT_DLL, CXOR("40 57 48 83 EC 20 49 8B 00")));

        create_effect_index(this, effect_index, effect_data);
    }
    int* cache_particle_effect(unsigned int* pEffectIndex, const char* szName)
    {
        static auto cache_particle_effect = reinterpret_cast<int*(CS_FASTCALL*)(void*, unsigned int*, const char*, int, __int64, __int64, __int64, int)>(MEM::FindPattern(CLIENT_DLL, CXOR("4C 8B DC 53 48 83 EC ? 48 8B 84 24")));

        return cache_particle_effect(this, pEffectIndex, szName, 8, 0ll, 0ll, 0ll, 0);
    }
    bool create_effect(unsigned int effect_index, int unk, void* clr, int unk2)
    {
        static auto create_effect = reinterpret_cast<bool(CS_FASTCALL*)(void*, unsigned int, int, void*, int)>(MEM::FindPattern(CLIENT_DLL, CXOR("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? F3 0F 10 1D ? ? ? ? 41 8B F8 8B DA 4C 8D 05")));

        return create_effect(this, effect_index, unk, clr, unk2);
    }
    bool unk_function(int effect_index, unsigned int unk, const stronghandle<c_particle_snapshot>* particle_snapshot)
    {
        static auto unk_function = reinterpret_cast<bool(CS_FASTCALL*)(void*, int, unsigned int, const stronghandle<c_particle_snapshot>*)>(MEM::FindPattern(CLIENT_DLL, CXOR("40 57 48 83 EC 20 49 8B 00")));

        return unk_function(this, effect_index, unk, particle_snapshot);
    }
};