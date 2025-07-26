#include "lagcomp.h"

namespace F::LAGCOMP
{
    void c_lagcompensation::run(C_CSPlayerPawn* player)
    {
        if (!player)
            return;

        auto game_scene_node = player->GetGameSceneNode();
        if (!game_scene_node)
            return;

        auto skeleton = game_scene_node->GetSkeletonInstance();
        if (!skeleton) 
            return;

        auto model_state = &skeleton->GetModel();
        CStrongHandle<CModel> model = model_state->m_hModel();
        auto model_skelet = &model->m_modelSkeleton();

        if (!model_skelet)
            return;

        skeleton->calc_world_space_bones(0, bone_flags::FLAG_ALL_BONE_FLAGS);

        if (m_animation.find(player->GetEntHandle(player)) != m_animation.end())
        {
            for (auto it = m_animation[player->GetEntHandle(player)].m_lag_records_.rbegin(); it != m_animation[player->GetEntHandle(player)].m_lag_records_.rend();)
            {
                if (!it->record_of_bounds())
                {
                    it = decltype(it){m_animation[player->GetEntHandle(player)].m_lag_records_.erase(std::next(it).base())};
                    continue;
                }

                it = next(it);
            }

            if (m_animation[player->GetEntHandle(player)].m_lag_records_.size() > 32)
                m_animation[player->GetEntHandle(player)].m_lag_records_.pop_back();
        }

        auto& lag_ = m_animation[player->GetEntHandle(player)];
        auto& record = lag_.m_lag_records_.emplace_front();

        record.setup(player);

        lag_.latest_record = &record;
        lag_.oldest_record = &lag_.m_lag_records_.back();
    }

    float c_lagcompensation::get_delta_time(int tick_entity)
    {
        static auto sv_maxunlag = I::Cvar->Find(FNV1A::HashConst("sv_maxunlag"));
        float maxunlag_val = std::min(sv_maxunlag->get_float(), 0.2f);
        float unlag_ticks = maxunlag_val * 64.0f;

        int tick_diff = I::GlobalVars->m_tick_count - tick_entity;
        return tick_diff > unlag_ticks ? 0.0f : tick_diff * 0.015625f;
    }

    c_animation_table* c_lagcompensation::get_animation(C_CSPlayerPawn* player_pawn)
    {
        auto it = m_animation.find(player_pawn->GetEntHandle(player_pawn));
        return it != m_animation.end() ? &it->second : nullptr;
    }

    void c_lagrecord::setup(C_CSPlayerPawn* player_pawn)
    {
        if (!player_pawn) return;

        auto skeleton = player_pawn->GetGameSceneNode()->GetSkeletonInstance();
        if (!skeleton || !skeleton->get_bone_count()) return;

        auto model_state = &skeleton->GetModel();
        auto model = model_state->m_hModel();

        m_simtime_ = player_pawn->m_flSimulationTime();
        origin_ = player_pawn->GetGameSceneNode()->GetAbsOrigin();
        bone_count = model->GetHitboxesNum();
        memcpy(matrix_, model_state->GetHitboxData(), bone_count * sizeof(bone_data));
    }

    void c_lagrecord::apply(C_CSPlayerPawn* player_pawn, bool extrapolation)
    {
        if (!player_pawn) return;

        auto skeleton = player_pawn->GetGameSceneNode()->GetSkeletonInstance();
        if (!skeleton) return;

        auto model_state = &skeleton->GetModel();
        auto model = model_state->m_hModel();

        memcpy(backup_matrix_, model_state->GetHitboxData(), model->GetHitboxesNum() * sizeof(bone_data));
        memcpy(model_state->GetHitboxData(), matrix_, model->GetHitboxesNum() * sizeof(bone_data));
    }

    void c_lagrecord::restore(C_CSPlayerPawn* player_pawn)
    {
        if (!player_pawn) return;

        auto skeleton = player_pawn->GetGameSceneNode()->GetSkeletonInstance();
        if (!skeleton) return;

        auto model_state = &skeleton->GetModel();
        auto model = model_state->m_hModel();

        memcpy(model_state->GetHitboxData(), backup_matrix_, model->GetHitboxesNum() * sizeof(bone_data));
    }

    bool c_lagrecord::record_of_bounds()
    {
        static auto sv_maxunlag = I::Cvar->Find(FNV1A::HashConst("sv_maxunlag"));
        float max_unlag = std::min(sv_maxunlag->get_float(), 0.2f);
        auto nci = I::Engine->GetNetChannelInfo(0);
        if (!nci) return false;

        float latency = nci->get_latency(FLOW_OUTGOING) * 0.5f + nci->get_latency(FLOW_INCOMING);
        float delta_time = lagcomp->get_delta_time(get_sim_time() * 64.f) + latency;

        return std::fabsf(delta_time) < max_unlag * 0.5f;
    }
}