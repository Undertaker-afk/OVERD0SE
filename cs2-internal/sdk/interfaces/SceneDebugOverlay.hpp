#include "../../utilities/memory.h"
#include "../datatypes/color.h"

#include "../entity_handle.h"
#include "..\..\core\sdk.h"

class c_scene_debug_overlay {
public:
    void add_line(Vector_t* end, Vector_t* mins, int clr, bool depth, double dur)
    {
        return MEM::CallVFunc<void, 12U>(this, &end, &mins, &clr, depth, dur);
    }

    void add_box(const Vector_t& end, const Vector_t& mins, const Vector_t& maxs, const Vector_t& a3, Color_t clr)
    {
        using function_t = void(CS_THISCALL*)(c_scene_debug_overlay*, const Vector_t&, const Vector_t&, const Vector_t&, const Vector_t&, int, int, int, int, double);
        (*reinterpret_cast<function_t**>(std::uintptr_t(this)))[48](this, end, mins, maxs, a3, clr.r, clr.g, clr.b, clr.a, static_cast<double>(4.f));
    }
};