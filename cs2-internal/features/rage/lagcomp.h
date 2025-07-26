#pragma once

#include "../../utilities/common.h"

#include <memory>
#include <deque>
#include <iostream>
#include <array>
#include <unordered_map>

#include "../../sdk/datatypes/vector.h"
#include "../../sdk/datatypes/transform.h"
#include "../../sdk/datatypes/qangle.h"
#include "../../sdk/interfaces/iglobalvars.h"
#include "../../sdk/interfaces/iengineclient.h"
#include "../../sdk/interfaces/cgameentitysystem.h"

#include "../../sdk/interfaces/ienginecvar.h"

#include "../../core/hooks.h"

namespace F::LAGCOMP
{
    class c_bones
    {
    public:
        void copy();
        void set();
    protected:
        bone_data matrix_[128];
    };

    class c_lagrecord
    {
    private:
        float m_simtime_;
    public:
        bone_data matrix_[128];
    private:
        bone_data backup_matrix_[128];
        int bone_count;
        Vector_t origin_;
    public:
        void setup(C_CSPlayerPawn* player_pawn);
        void apply(C_CSPlayerPawn* player_pawn, bool extrapolation = false);
        void restore(C_CSPlayerPawn* player_pawn);
        bool record_of_bounds();
    public:
        float get_sim_time() { return m_simtime_; }
    };

    class c_animation_table
    {
    public:
        std::deque<c_lagrecord> m_lag_records_{};
        c_lagrecord* latest_record{};
        c_lagrecord* oldest_record{};
    };

    const auto lagrecords = std::make_unique<c_lagrecord>();

    class c_lagcompensation
    {
    public:
        void run(C_CSPlayerPawn* player_pawn);
    public:
        float get_delta_time(int tick_entity);
    public:
        c_animation_table* get_animation(C_CSPlayerPawn* player_pawn);
    private:
        std::unordered_map<unsigned long, c_animation_table> m_animation{};
    public:
        int level;
    };

    const auto lagcomp = std::make_unique<c_lagcompensation>();

}
