#pragma once

// used: mem_pad
#include "../../utilities/memory.h"

class IGlobalVars
{
public:
    float m_realtime; //0x0000
    int32_t m_frame_count; //0x0004
    float m_absolute_frame_time; //0x0008
    float m_absolute_frame_start_time_std_dev; //0x000C
    int32_t m_max_clients; //0x0010
    char pad_0014[8]; //0x0014
    float m_interval_per_tick; //0x001C
    char pad_0020[8]; //0x0020
    void* m_current_time_update; //0x0028
    float m_frame_time; //0x0030
    float m_current_time; //0x0034
    float m_player_time; //0x0038
    char pad_003C[12]; //0x003C
    int m_tick_count; //0x0048
    char pad_004C[4]; //0x004C
    float m_tick_fraction; //0x0050
    char pad_0054[4]; //0x0054
    void* m_net_channel; //0x0058
    char pad_0060[112]; //0x0060
    void* m_loop_back; //0x00D0
    char pad_00D8[8]; //0x00D8
    void* m_storage_entities; //0x00E0
    char pad_00E8[72]; //0x00E8
    void* m_network_string_table; //0x0130
    char pad_0138[64]; //0x0138
    char* m_current_map; //0x0178
    char* m_current_map_name; //0x0180
};