#include "pred.h"

#include "../../core/config.h"
#include "../../core/sdk.h"

#include "../../sdk/datatypes/usercmd.h"
#include "../../sdk/entity.h"
#include "../../sdk/interfaces/iengineclient.h"
#include "../../sdk/interfaces/iglobalvars.h"
#include "../../sdk/interfaces/cgameentitysystem.h"
#include "../../sdk/interfaces/inetworkclientservice.h"
#include "../../sdk/datatypes/qangle.h"
#include "../../sdk/datatypes/vector.h"
#include "../../sdk/interfaces/ccsgoinput.h"
#include "../../sdk/interfaces/ienginecvar.h"
#include "../../sdk/interfaces/events.h"

void c_eng_pred::run()
{
    if (!I::Engine->IsInGame() || !I::Engine->IsConnected())
        return;

    auto* network_client = I::NetworkClientService->GetNetworkClient();
    if (!network_client)
        return;

    m_pred_data.m_absolute_frame_start_time_std_dev = I::GlobalVars->m_absolute_frame_start_time_std_dev;
    m_pred_data.m_absolute_frame_time = I::GlobalVars->m_absolute_frame_time;
    m_pred_data.m_current_time = I::GlobalVars->m_current_time;
    m_pred_data.m_tick_base = SDK::LocalController->m_nTickBase();

    if (const auto active_weapon = SDK::LocalPawn->ActiveWeapon())
    {
        active_weapon->update_accuracy_penality();

        m_pred_data.m_spread = active_weapon->get_spread();
        m_pred_data.m_inaccuracy = active_weapon->get_inaccuracy();
    }

    this->predict();
}

void c_eng_pred::predict()
{
    auto* client_info = I::Engine->get_networked_client_info();
    if (!client_info || !client_info->m_local_data)
        return;

    m_pred_data.m_eye_pos = client_info->m_local_data->m_eye_pos;
    m_pred_data.m_player_tick = client_info->m_player_tick_count;
    m_pred_data.m_player_tick_fraction = client_info->m_player_tick_fraction;
    m_pred_data.m_render_tick = client_info->m_render_tick;
    m_pred_data.m_render_tick_fraction = client_info->m_render_tick_fraction;

    if (m_pred_data.m_player_tick_fraction > (1.f - 0.0099999998f))
        m_pred_data.m_tick_base++;

    if (auto movement = SDK::LocalPawn->m_pMovementServices())
        movement->set_prediction_command(SDK::Cmd);

    SDK::LocalController->physics_run_think();
    m_initialized = true;
}

void c_eng_pred::end()
{
    if (!m_initialized || !I::Engine->IsInGame() || !I::Engine->IsConnected())
        return;

    auto* network_client = I::NetworkClientService->GetNetworkClient();
    if (!network_client)
        return;

    auto* client_info = I::Engine->get_networked_client_info();
    if (!client_info || !client_info->m_local_data)
        return;

    if (auto movement = SDK::LocalPawn->m_pMovementServices())
        movement->reset_prediction_command();

    I::GlobalVars->m_absolute_frame_start_time_std_dev = m_pred_data.m_absolute_frame_start_time_std_dev;
    I::GlobalVars->m_absolute_frame_time = m_pred_data.m_absolute_frame_time;
    I::GlobalVars->m_current_time = m_pred_data.m_current_time;

    m_pred_data.m_shoot_tick = SDK::LocalController->m_nTickBase() - 1;
    SDK::LocalController->m_nTickBase() = m_pred_data.m_tick_base;
}