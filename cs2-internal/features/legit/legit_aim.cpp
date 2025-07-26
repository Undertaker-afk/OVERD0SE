#include "legit_aim.h"
#include "../../sdk/entity.h"
#include "../../sdk/interfaces/cgameentitysystem.h"
#include "../../sdk/interfaces/iengineclient.h"
#include "../../sdk/datatypes/usercmd.h"
#include "../../sdk/interfaces/itrace.h"
#include "../../utilities/inputsystem.h"
#include "../../utilities/draw.h"
#include "../../core/sdk.h"
#include "../../features/configs/configs_var.h"


// Функция для получения углов отдачи
QAngle_t GetRecoil(C_CSPlayerPawn* pLocal)
{
    static QAngle_t old_punch;

    if (pLocal->GetShotsFired() > 1)
    {
        // Вычисляем разницу между старым и новым углом отдачи
        QAngle_t punch = pLocal->GetAimPuchAngle() * 2.0f;
        QAngle_t delta = old_punch - punch;
        old_punch = punch;
        return delta;
    }

    old_punch = QAngle_t();
    return QAngle_t();
}

// Функция для получения угловой разницы между текущим взглядом и целью
QAngle_t GetAngularDifference(Vector_t vecTarget, C_CSPlayerPawn* pLocal)
{
    Vector_t vecEyePos = pLocal->GetEyePosition();
    QAngle_t angDelta = (vecTarget - vecEyePos).ToAngles();
    angDelta.Normalize();
    angDelta -= cheat->viewangles;
    return angDelta;
}

// Основная функция обработки движения
void c_legit_bot::on_create_move(CBaseUserCmdPB* pBaseCmd, CCSPlayerController* pLocalController, C_CSPlayerPawn* pLocalPawn)
{
    if (!cfg_var::m_cfg->g_legit_aim.enablelegit)
        return;

    if (!pLocalController->IsPawnAlive())
        return;

    process_assist(pBaseCmd, pLocalPawn, pLocalController);
}

// Функция помощи при прицеливании
void c_legit_bot::process_assist(CBaseUserCmdPB* pUserCmd, C_CSPlayerPawn* pLocalPawn, CCSPlayerController* pLocalController)
{
    // Проверяем нажата ли клавиша активации
    if (!IPT::IsKeyDown(cfg_var::m_cfg->g_legit_aim.legit_key))
        return;

    CCSPlayerController* pTarget = nullptr;
    Vector_t vecBestPos = Vector_t();
    float flBestDistance = FLT_MAX;

    // Перебираем всех игроков
    for (int nIndex = 1; nIndex <= I::GameResourceService->pGameEntitySystem->GetHighestEntityIndex(); nIndex++)
    {
        C_BaseEntity* pEntity = I::GameResourceService->pGameEntitySystem->Get(nIndex);
        if (!pEntity)
            continue;

        // Проверяем что это контроллер игрока
        SchemaClassInfoData_t* pClassInfo = nullptr;
        pEntity->GetSchemaClassInfo(&pClassInfo);
        if (!pClassInfo || FNV1A::Hash(pClassInfo->szName) != FNV1A::HashConst("CCSPlayerController"))
            continue;

        CCSPlayerController* pPlayer = reinterpret_cast<CCSPlayerController*>(pEntity);
        if (!pPlayer || pPlayer == pLocalController)
            continue;

        // Получаем пешку игрока
        C_CSPlayerPawn* pPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(pPlayer->GetPawnHandle());
        if (!pPawn || !pPlayer->IsPawnAlive() || !pLocalPawn->IsOtherEnemy(pPawn))
            continue;

        // Проверяем что пешка не спит и не в иммунитете
        CGameSceneNode* pGameSceneNode = pPawn->GetGameSceneNode();
        if (!pGameSceneNode || pGameSceneNode->IsDormant())
            continue;

        // Получаем позицию хитбокса (головы)
        CSkeletonInstance* pSkeleton = pGameSceneNode->GetSkeletonInstance();
        if (!pSkeleton || !pSkeleton->m_pBoneCache)
            continue;

        Vector_t vecBonePos = pSkeleton->m_pBoneCache->get_origin();

        // Проверка видимости
        ray_t ray;
        game_trace_t trace;
        c_trace_filter filter(0x1C3003, pLocalPawn, nullptr, 4);
        I::Trace->clip_ray_to_entity2(&ray, pLocalPawn->GetEyePosition(), vecBonePos, pPawn, &filter, &trace);

        if (trace.HitEntity != pPawn || trace.Fraction < 0.97f)
            continue;

        // Вычисляем угловое расстояние
        float flDistance = GetAngularDifference(vecBonePos, pLocalPawn).Length2D();
        if (flDistance > cfg_var::m_cfg->g_legit_aim.aimrange)
            continue;

        // Выбираем ближайшую цель
        if (pTarget && flDistance >= flBestDistance)
            continue;

        pTarget = pPlayer;
        vecBestPos = vecBonePos;
        flBestDistance = flDistance;
    }

    if (!pTarget)
        return;

    // Применяем сглаживание и контроль отдачи
    QAngle_t* pViewAngles = &(cheat->viewangles);
    QAngle_t angDelta = GetAngularDifference(vecBestPos, pLocalPawn);

    // Контроль отдачи
    QAngle_t angRecoil = GetRecoil(pLocalPawn);
    if (pLocalPawn->GetShotsFired() > 1)
    {
        angDelta.x -= angRecoil.x;
        angDelta.y -= angRecoil.y;
    }

    // Применяем сглаживание
    float flSmoothing = std::max(1.0f, cfg_var::m_cfg->g_legit_aim.aimsmooth);
    pViewAngles->x += angDelta.x / flSmoothing;
    pViewAngles->y += angDelta.y / flSmoothing;
    pViewAngles->Normalize();
}

// Отрисовка FOV круга
void c_legit_bot::drawfov()
{
    if (!cfg_var::m_cfg->g_legit_aim.aimfov)
        return;

    if (!SDK::LocalPawn)
        return;

    float scale = ImGui::GetIO().DisplaySize.y / (2.0f * tanf(/*max range*/15.0f * 0.5f * (3.14159265f / 180.0f)));
    float rad = tanf(cfg_var::m_cfg->g_legit_aim.aimrange * 0.5f * (3.14159265f / 180.0f)) * scale;

    rad *= 0.35f;

    ImGui::GetForegroundDrawList()->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f), rad,
        cfg_var::m_cfg->g_legit_aim.aimfovcolor.colValue.GetU32(), 100);
}