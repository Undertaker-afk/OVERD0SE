#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_internal.h"
#include "../../imgui/imgui_impl_dx11.h"
#include "../../imgui/imgui_impl_win32.h"

#include "../../sdk/datatypes/color.h"

#include "IconsFontAwesome6.h"

#include <map>
#include <vector>
#include <iostream>

#define ALPHA ( ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)

inline ImVec2 wsize = ImVec2(700, 500);

inline ImVec2 BeginChildPos = ImVec2(180, 60);
inline ImVec2 BeginChildSize = ImVec2(wsize.x + 1, wsize.y - 24) - BeginChildPos;

namespace widgets 
{
    struct NotifyStruct
    {
        std::string msg;
        float time;
    };

    float float_lerp(ImGuiID id, bool cond, float speed = 0.15f);
    
    ImColor color_lerp(ImColor start, ImColor end, float stage);
    
    bool tab(const char* icon, const char* label, bool selected);
    
    void separator(const char* label);
   
    bool subtab(const char* label, bool selected);
    bool child(const char* icon, const char* label, const ImVec2& size_arg = ImVec2((ImGui::GetWindowWidth() - 36) / 2, 0), ImGuiWindowFlags flags = NULL);
    bool checkbox(const char* label, bool* v);
    bool selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
    bool selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
    bool begin_combo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
    bool combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    bool combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items = -1);
    
    void multi_combo(const char* label, const std::vector<int>& current_items, const char* items[], int items_count, std::string preview = "None");
    
    bool color_picker4(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col);
    bool color_edit4(const char* label, float col[4], ImGuiColorEditFlags flags);
   
    void color(const std::string name, Color_t& colValue);
    
    bool slider_scalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
    bool slider_int(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool slider_float(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags);
    bool keybind(const char* label, int* key, int* mode);

    void add_log( const std::string& msg );
    void render_log( );

    inline std::vector< NotifyStruct > cur_logs;
}