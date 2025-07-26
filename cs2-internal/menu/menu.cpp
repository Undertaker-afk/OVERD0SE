#include "menu.h"

#include "../core/interfaces.h"

#include "../sdk/entity.h"
#include "../sdk/interfaces/iengineclient.h"
#include "../sdk/interfaces/inetworkclientservice.h"
#include "../sdk/interfaces/iglobalvars.h"
#include "../sdk/interfaces/ienginecvar.h"

#include "../features/visuals/overlay.h"
#include "../features/skins/modelchanger.h"
#include "../features/configs/configs.h"
#include "../features/configs/configs_var.h"

#include "widgets/widgets.h"
#include "../sdk/inventory/inventory.h"
#include "../features/skins/inventorychanger.h"

inline int tab = 0;
static int mode;

void render_subtabs(const char* render_subtab_items[], int* render_subtab_tab, int render_subtab_count)
{
    ImGui::SetCursorPos(ImVec2(194, 10));
    ImGui::BeginGroup();
    {
        for (int i = 0; i < render_subtab_count; i++)
        {
            if (widgets::subtab(render_subtab_items[i], *render_subtab_tab == i)) 
                *render_subtab_tab = i; 

            ImGui::SameLine(0, 10);
        }
    }
    ImGui::EndGroup();
}

void tab_ragebot()
{
    const char* rage_items[] = { ("General"), ("Anti-aims") };
    render_subtabs(rage_items, &MENU::rage_subtab, IM_ARRAYSIZE(rage_items));

    ImGui::SetCursorPos(BeginChildPos);
    ImGui::BeginChild(CXOR("##Ragebot_tab"), BeginChildSize, true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
    {
        auto& cfg = cfg_var::m_cfg->g_aim;
        auto& cfg_hit = cfg_var::m_cfg->g_antiaim;

        ImGui::BeginGroup();
        {
            switch (MENU::rage_subtab) {
            case 0: {
                widgets::child(ICON_FA_SLIDERS, CXOR("Main"));
                {
                    const char* weapons[7]{ ("Default"), ("Pistols"), ("Heavy Pistols"), ("Assult Rifles"), ("Auto"), ("Scout"), ("Awp") };
                    const char* targets_select[3]{ ("None"), ("Normalize"), ("Maximum") };

                    widgets::checkbox(CXOR("Enable"), &cfg.enablerage);
                    widgets::checkbox(CXOR("Auto fire"), &cfg.auto_fire);

                    widgets::combo(CXOR("Weapon"), &cfg.cur_wep, weapons, IM_ARRAYSIZE(weapons), sizeof weapons);
                }
                ImGui::EndChild();
            } break;
            case 1:
                widgets::child(ICON_FA_SLIDERS, CXOR("Main"));
                {
                    const char* pitch_types[6] { ("Off"), ("Down"), ("Up"), ("Zero"), ("Random"), ("Custom") };
                    widgets::combo(CXOR("Pitch"), &cfg_hit.pitchtype, pitch_types, IM_ARRAYSIZE(pitch_types), sizeof pitch_types);

                    if (cfg_hit.pitchtype == 5)
                        widgets::slider_int(CXOR("Pitch"), &cfg_hit.custompitch, -89, 89, CXOR("%d%"));

                    widgets::separator(CXOR(""));

                    const char* yaw_types[4] { ("Off"), ("Backwards"), ("Random"), ("Custom") };
                    widgets::combo(CXOR("Yaw"), &cfg_hit.baseyaw, yaw_types, IM_ARRAYSIZE(yaw_types), sizeof yaw_types);

                    if (cfg_hit.baseyaw == 3)
                        widgets::slider_int(CXOR("Yaw"), &cfg_hit.customyaw, -180, 180, CXOR("%d%"));
                }
                ImGui::EndChild();
                break;
            }
        }
        ImGui::EndGroup();
        ImGui::SameLine(0, 12);
        ImGui::BeginGroup();
        {
            switch (MENU::rage_subtab) {
            case 0: {
                widgets::child(ICON_FA_GUN, CXOR("Weapons"));
                {
                    auto cur_weapon = cfg_var::m_cfg->g_aim.cur_wep;
                    const char* targets_select[3]{ ("None"), ("Normal"), ("Max") };

                    widgets::combo(CXOR("Target selection"), &cfg.weapon[cur_weapon].rage_target_select, targets_select, IM_ARRAYSIZE(targets_select), sizeof targets_select);

                    if (widgets::begin_combo(CXOR("Hitbox"), CXOR("Select hitbox"), 9))
                    {
                        widgets::checkbox(CXOR("Head"), &cfg.weapon[cur_weapon].m_head);
                        widgets::checkbox(CXOR("Chest"), &cfg.weapon[cur_weapon].m_chest);
                        widgets::checkbox(CXOR("Stomach"), &cfg.weapon[cur_weapon].m_stomach);
                        widgets::checkbox(CXOR("Arms"), &cfg.weapon[cur_weapon].m_arms);
                        widgets::checkbox(CXOR("Legs"), &cfg.weapon[cur_weapon].m_legs);
                        widgets::checkbox(CXOR("Foot"), &cfg.weapon[cur_weapon].m_foot);

                        ImGui::EndCombo();
                    }

                    if (widgets::begin_combo(CXOR("Points"), CXOR("Select points"), 9))
                    {
                        widgets::checkbox(CXOR("Head"), &cfg.weapon[cur_weapon].p_head);
                        widgets::checkbox(CXOR("Chest"), &cfg.weapon[cur_weapon].p_chest);
                        widgets::checkbox(CXOR("Stomach"), &cfg.weapon[cur_weapon].p_stomach);
                        widgets::checkbox(CXOR("Arms"), &cfg.weapon[cur_weapon].p_arms);
                        widgets::checkbox(CXOR("Legs"), &cfg.weapon[cur_weapon].p_legs);
                        widgets::checkbox(CXOR("Foot"), &cfg.weapon[cur_weapon].p_foot);

                        ImGui::EndCombo();
                    }

                    widgets::separator(CXOR(""));

                    if (cur_weapon > 3)
                        widgets::checkbox(CXOR("Auto Scope"), &cfg.weapon[cur_weapon].autoscope);

                    widgets::checkbox(CXOR("Auto stop"), &cfg.weapon[cur_weapon].autostop);

                    widgets::slider_int(CXOR("Hitchance"), &cfg.weapon[cur_weapon].chance, 0, 100, CXOR("%d%"));
                    widgets::keybind(CXOR("Hitchance override"), &cfg.weapon[cur_weapon].is_hitchan_override, &mode);

                    if (cfg.weapon[cur_weapon].is_hitchan_override)
                        widgets::slider_int(CXOR("Hitchance"), &cfg.weapon[cur_weapon].hitchan_override_val, 0, 100, CXOR("%d%"));

                    widgets::slider_int(CXOR("Point scale"), &cfg.weapon[cur_weapon].point_scale, 1, 100, CXOR("%d%"));

                    widgets::slider_int(CXOR("Min dmg"), &cfg.weapon[cur_weapon].mindmg, 0, 100, CXOR("%d%"));
                    widgets::keybind(CXOR("Min dmg override"), &cfg.weapon[cur_weapon].is_mindmg_override, &mode);

                    if (cfg.weapon[cur_weapon].is_mindmg_override)
                        widgets::slider_int(CXOR("Min damage"), &cfg.weapon[cur_weapon].mindmg_override_val, 0, 100, CXOR("%d%"));
                }
                ImGui::EndChild();
            } break;
            case 1:
                widgets::child(ICON_FA_ARROWS_ROTATE, CXOR("Manuals"));
                {
                    widgets::keybind(CXOR("Manual left"), &cfg_hit.left_anti, &mode);
                    widgets::keybind(CXOR("Manual right"), &cfg_hit.right_anti, &mode);
                    widgets::keybind(CXOR("Manual back"), &cfg_hit.backward_anti, &mode);

                    widgets::checkbox(CXOR("Hide shots"), &cfg_hit.hideshots);
                }
                ImGui::EndChild();
                break;
            }
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
}

void tab_legitbot()
{
    const char* legit_items[] = { ("General") };
    render_subtabs(legit_items, &MENU::legit_subtab, IM_ARRAYSIZE(legit_items));

    ImGui::SetCursorPos(BeginChildPos);
    ImGui::BeginChild(CXOR("##Legitbot_tab"), BeginChildSize, true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
    {
        auto& cfg = cfg_var::m_cfg->g_legit_aim;

        ImGui::BeginGroup();
        {
            switch (MENU::legit_subtab) {
            case 0: {
                widgets::child(ICON_FA_SLIDERS, CXOR("Main"));
                {
                    widgets::checkbox(CXOR("Enable"), &cfg.enablelegit);
                    widgets::checkbox(CXOR("Fov visualization"), &cfg.aimfov);

                    if (cfg.aimfov)
                        widgets::color(CXOR("Visualization color"), cfg.aimfovcolor.colValue);

                    widgets::slider_float(CXOR("Aim range"), &cfg.aimrange, 1.f, 90.f, CXOR("%.3f"), NULL);
                    widgets::slider_float(CXOR("Aim smooth"), &cfg.aimsmooth, 1.f, 100.f, CXOR("%.3f"), NULL);

                }
                ImGui::EndChild();
            } break;
            }
        }
        ImGui::EndGroup();
        ImGui::SameLine(0, 12);
        ImGui::BeginGroup();
        {
            switch (MENU::legit_subtab) {
            case 0: {
                widgets::child(ICON_FA_GUN, CXOR("Binds")); {
                    widgets::keybind(CXOR("Aim key"), &cfg.legit_key, &mode);
                }
                ImGui::EndChild();
            } break;
            }
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
}

void tab_visuals()
{
    const char* visuals_items[] = { ("ESP"), ("World") };
    render_subtabs(visuals_items, &MENU::visuals_subtab, IM_ARRAYSIZE(visuals_items));

    ImGui::SetCursorPos(BeginChildPos);
    ImGui::BeginChild(CXOR("##Visuals_tab"), BeginChildSize, true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
    {
        auto& cfg = cfg_var::m_cfg->g_visuals;
        auto& cfg_world = cfg_var::m_cfg->g_misc;

        ImGui::BeginGroup();
        {
            switch (MENU::visuals_subtab) {
            case 0: {
                widgets::child(ICON_FA_SLIDERS, CXOR("Main")); 
                {
                    widgets::checkbox(CXOR("Enable"), &cfg.enablevis);

                    widgets::separator(CXOR(""));

                    { // ESP Box
                        widgets::checkbox(CXOR("Box"), &cfg.box);

                        if (cfg.box)
                        {
                            widgets::color(CXOR("[ Box ] Color"), cfg.box_sets.col.colValue);

                            widgets::slider_float(CXOR("[ Box ] Rounding"), &cfg.box_sets.rounding, 0.0f, 10.f, CXOR("%.1f"), NULL);
                            widgets::slider_float(CXOR("[ Box ] Thickness"), &cfg.box_sets.thickness, 0.5f, 3.f, CXOR("%.1f"), NULL);

                            widgets::checkbox(CXOR("[ Box ] Outline"), &cfg.box_sets.outlined);
                        }

                        widgets::separator(CXOR(""));
                    }

                    { // ESP Name
                        widgets::checkbox(CXOR("Name"), &cfg.name);

                        if (cfg.name)
                            widgets::color(CXOR("[ Name ] Color"), cfg.name_sets.col.colValue);

                        widgets::separator(CXOR(""));
                    }

                    { // ESP Health bar
                        widgets::checkbox(CXOR("Health bar"), &cfg.hbar);

                        if (cfg.hbar)
                        {
                            widgets::color(CXOR("[ Health bar ] Health bar color"), cfg.hbar_sets.col.colValue);

                            widgets::slider_float(CXOR("[ HB ] Thickness"), &cfg.hbar_sets.thickness, 1.0f, 3.f, CXOR("%.1f"), NULL);

                            widgets::checkbox(CXOR("[ Health bar ] Outline"), &cfg.hbar_sets.outlined);
                        }

                        widgets::separator(CXOR(""));
                    }

                    { // ESP Ammo bar
                        widgets::checkbox(CXOR("Ammo bar"), &cfg.abar);

                        if (cfg.abar)
                        {
                            widgets::color(CXOR("[ Ammo bar ] Color"), cfg.abar_sets.col.colValue);

                            widgets::slider_float(CXOR("[ AB ] Thickness"), &cfg.abar_sets.thickness, 1.0f, 3.f, CXOR("%.1f"), NULL);

                            widgets::checkbox(CXOR("[ Ammo bar ] Outline"), &cfg.abar_sets.outlined);
                        }

                        widgets::separator(CXOR(""));
                    }

                    { // ESP Weapon
                        widgets::checkbox(CXOR("Weapon"), &cfg.textweap);

                        if (cfg.textweap)
                            widgets::color(CXOR("[ Weapon ] Color"), cfg.weap_sets.col.colValue);

                        widgets::separator(CXOR(""));
                    }

                    widgets::checkbox(CXOR("Skeleton"), &cfg.skeleton);

                    if (cfg.skeleton)
                    {
                        widgets::color(CXOR("[ Skeleton ] Color"), cfg.skelet_sets.col.colValue);
                        widgets::checkbox(CXOR("[ Skeleton ] Outline"), &cfg.skelet_sets.outlined);
                    }

                    if (widgets::begin_combo(CXOR("Flags"), CXOR("Select flags"), 4))
                    {
                        widgets::checkbox(CXOR("Armor"), &cfg.Flags[0]);
                        widgets::checkbox(CXOR("Defuser"), &cfg.Flags[1]);
                        widgets::checkbox(CXOR("Scoped"), &cfg.Flags[2]);
                        widgets::checkbox(CXOR("Defusing"), &cfg.Flags[3]);
                        widgets::checkbox(CXOR("GBB hostage"), &cfg.Flags[4]);

                        ImGui::EndCombo();
                    }

                    widgets::separator(CXOR(""));

                    widgets::checkbox(CXOR("Enable arrows"), &cfg.enablevisarrows);

                    if (cfg.enablevisarrows)
                        widgets::color(CXOR("[ Arrows ] color"), cfg.arrows_color.colValue);
                }
                ImGui::EndChild();
            } break;

            case 1: {
                widgets::child(ICON_FA_SLIDERS, CXOR("Main"));
                {
                    widgets::checkbox(CXOR("Grenade camera"), &cfg_world.granade_cam);
                    widgets::checkbox(CXOR("Remove smoke"), &cfg_world.smokeremove);
                }
                ImGui::EndChild();
            } break;
            }
        }
        ImGui::EndGroup();
        ImGui::SameLine(0, 12);
        ImGui::BeginGroup();
        {
            switch (MENU::visuals_subtab) {
            case 0: {
                widgets::child(ICON_FA_SEEDLING, CXOR("Chams")); 
                {
                    const char* chams[6] { ("Solid"), ("Flat"), ("Glow"), ("Smile"), ("Star"), ("Electric") };

                    widgets::checkbox(CXOR("Local glow"), &cfg.chams_local.LocalGlow);

                    if (cfg.chams_local.LocalGlow)
                        widgets::color(CXOR("[ Local glow ] Color"), cfg.chams_local.colLocalGlow.colValue);

                    widgets::separator(CXOR(""));

                    widgets::checkbox(CXOR("Local chams"), &cfg.chams_local.chamLocal);
                    
                    if (cfg.chams_local.chamLocal) 
                    {
                        widgets::color(CXOR("[ Local chams ] Color"), cfg.chams_local.colLocalCham.colValue);
                        widgets::combo(CXOR("[ L Chams type ]"), &cfg.chams_local.chamstype, chams, IM_ARRAYSIZE(chams), sizeof chams);
                    }

                    widgets::separator(CXOR(""));

                    widgets::checkbox(CXOR("Local attach chams"), &cfg.chams_local.weapcham_local);
                 
                    if (cfg.chams_local.weapcham_local) 
                    {
                        widgets::color(CXOR("[ Local attach ] Color"), cfg.chams_local.colWeapCham.colValue);
                        widgets::combo(CXOR("[ A Chams type ]"), &cfg.chams_local.chamstypeWeap, chams, IM_ARRAYSIZE(chams), sizeof chams);
                    }

                    widgets::separator(CXOR(""));

                    widgets::checkbox(CXOR("Enemy visible"), &cfg.chams_enemie.vischam);

                    if (cfg.chams_enemie.vischam)
                        widgets::color(CXOR("[ Enemy visible chams ] Color"), cfg.chams_enemie.colVisCham.colValue);

                    widgets::checkbox(CXOR("Enemy invisible"), &cfg.chams_enemie.invischam);

                    if (cfg.chams_enemie.invischam)
                        widgets::color(CXOR("[ Enemy invisible chams ] Color"), cfg.chams_enemie.colInvisCham.colValue);

                    widgets::combo(CXOR("[ E Chams type ]"), &cfg.chams_enemie.chamstype, chams, IM_ARRAYSIZE(chams), sizeof chams);
                }
                ImGui::EndChild();
            } break;

            case 1: {
                widgets::child(ICON_FA_WORM, CXOR("Modulation"));
                {
                    widgets::checkbox(CXOR("Night mode"), &cfg_world.nightmode);

                    if (cfg_world.nightmode)
                    {
                        widgets::color(CXOR("Night mode color"), cfg_world.colNightmode.colValue);
                        widgets::color(CXOR("Lighting color"), cfg_world.colLighting.colValue);
                        widgets::color(CXOR("Sky color"), cfg_world.colSkyBox.colValue);

                        widgets::slider_float(CXOR("Map tone"), &cfg_world.map_tone, 0, 100, CXOR("%.3f"), NULL);
                    }
                }
                ImGui::EndChild();
            } break;
            }
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
}














void tab_skins()
{
    struct DumpedSkin_t
    {
        std::string m_name = "";
        int m_ID = 0;
        int m_rarity = 0;
    };

    struct DumpedItem_t
    {
        std::string m_name = "";
        uint16_t m_defIdx = 0;
        int m_rarity = 0;
        void* m_image = nullptr;
        ImTextureID m_textureID = nullptr;
        bool m_unusualItem = false;
        std::vector<DumpedSkin_t> m_dumpedSkins{};
        DumpedSkin_t* pSelectedSkin = nullptr;
    };

    static std::vector<DumpedItem_t> vecDumpedItems;
    static DumpedItem_t* pSelectedItem = nullptr;

    CEconItemSchema* pItemSchema =
        I::Client->GetEconItemSystem()->GetEconItemSchema();


    const char* skins_items[] = { ("General"), ("Models") };
    render_subtabs(skins_items, &MENU::skins_subtab, IM_ARRAYSIZE(skins_items));

    ImGui::SetCursorPos(BeginChildPos);
    ImGui::BeginChild(CXOR("##Skins_tab"), BeginChildSize, true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
    {
        auto& cfg = cfg_var::m_cfg->g_skins;
        static constexpr float windowWidth = 540.f;
        ImGui::BeginGroup();
        {
            switch (MENU::skins_subtab) {
            case 0: {
                widgets::child(ICON_FA_SLIDERS, CXOR("Main")); {
                    if (vecDumpedItems.empty() && changers::dumpitems)
                    {
                        const CUtlMap2<int, CEconItemDefinition*>& vecItems =
                            pItemSchema->GetSortedItemDefinitionMap();
                        CUtlMap2<int, CPaintKit*>& vecPaintKits =
                            pItemSchema->GetPaintKits();
                        static const CUtlMap2<uint64_t, AlternateIconData_t>& vecAlternateIcons =
                            pItemSchema->GetAlternateIconsMap();

                        for (const auto& it : vecItems)
                        {
                            CEconItemDefinition* pItem = it.m_value;
                            if (!pItem)
                                continue;

                            const bool isWeapon = pItem->IsWeapon();
                            const bool isKnife = pItem->IsKnife(true);
                            const bool isGloves = pItem->IsGlove(true);
                            const bool IsAgent = pItem->IsAgent();

                            const char* itemBaseName = pItem->m_pszItemBaseName;

                            if (!itemBaseName || itemBaseName[0] == '\0')
                                continue;

                            const uint16_t defIdx = pItem->m_nDefIndex;

                            DumpedItem_t dumpedItem;
                            dumpedItem.m_name = I::Localize->FindSafe(pItem->GetName());

                            //dumpedItem.m_image = pItem->m_pKVItem;
                            dumpedItem.m_defIdx = defIdx;
                            dumpedItem.m_rarity = pItem->m_nItemRarity;
                            if (isKnife || isGloves || isWeapon || IsAgent)
                            {
                                dumpedItem.m_unusualItem = true;
                            }
                            dumpedItem.m_dumpedSkins.emplace_back("Vanilla", 0, IR_ANCIENT);
                            // Load the image and set the texture ID.
                            //if (dumpedItem.m_image)
                            //{
                            //	dumpedItem.m_textureID = CreateTextureFromMemory(dumpedItem.m_image, 120, 280);
                            //}

                            // We filter skins by guns.
                            for (const auto& it : vecPaintKits)
                            {
                                CPaintKit* pPaintKit = it.m_value;
                                if (!pPaintKit || pPaintKit->PaintKitId() == 0 || pPaintKit->PaintKitId() == 9001)
                                    continue;

                                const uint64_t skinKey =
                                    Helper_GetAlternateIconKeyForWeaponPaintWearItem(
                                        defIdx, pPaintKit->PaintKitId(), 0);
                                if (vecAlternateIcons.FindByKey(skinKey))
                                {
                                    DumpedSkin_t dumpedSkin;
                                    dumpedSkin.m_name = I::Localize->FindSafe(
                                        pPaintKit->PaintKitDescriptionTag());
                                    dumpedSkin.m_ID = pPaintKit->PaintKitId();
                                    dumpedSkin.m_rarity = std::clamp(pItem->GetRarity() + pPaintKit->PaintKitRarity() - 1, 0, (pPaintKit->PaintKitRarity() == 7) ? 7 : 6) + 1;
                                    dumpedItem.m_dumpedSkins.emplace_back(dumpedSkin);
                                }
                            }

                            //Sort skins by rarity.
                            if (!dumpedItem.m_dumpedSkins.empty() && isWeapon)
                            {
                                std::sort(dumpedItem.m_dumpedSkins.begin(),
                                    dumpedItem.m_dumpedSkins.end(),
                                    [](const DumpedSkin_t& a, const DumpedSkin_t& b)
                                    {
                                        return a.m_rarity > b.m_rarity;
                                    });
                            }

                            vecDumpedItems.emplace_back(dumpedItem);
                        }
                    }
                    static char IconFilterText[128] = "";

                    if (!vecDumpedItems.empty())
                    {
                        if (ImGui::Button("Add all items", ImVec2(120, 50)))
                        {
                            for (const auto& item : vecDumpedItems)
                            {
                                for (const auto& skin : item.m_dumpedSkins)
                                {
                                    CEconItem* pItem = CEconItem::CreateInstance();
                                    if (pItem)
                                    {
                                        CCSPlayerInventory* pInventory = GSF::pInventoryManager->GetLocalInventory();
                                        auto highestIDs = pInventory->GetHighestIDs();
                                        pItem->m_ulID = highestIDs.first + 1;
                                        pItem->m_unInventory = highestIDs.second + 1;
                                        pItem->m_unAccountID =
                                            uint32_t(pInventory->GetOwner().m_id);
                                        pItem->m_unDefIndex = item.m_defIdx;
                                        if (item.m_unusualItem)
                                            pItem->m_nQuality = IQ_UNUSUAL;
                                        pItem->m_nRarity =
                                            std::clamp(item.m_rarity + skin.m_rarity - 1, 0,
                                                (skin.m_rarity == 7) ? 7 : 6);

                                        pItem->SetPaintKit((float)skin.m_ID);
                                        pItem->SetPaintSeed(1.f);
                                        if (pInventory->AddEconItem(pItem))
                                            changers::AddEconItemToList(pItem);
                                    }
                                }
                            }
                        }
                    }

                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("Will cause lag on weaker computers.");
                    if (!vecDumpedItems.empty())
                    {
                        static ImGuiTextFilter itemFilter;
                        itemFilter.Draw("Type here to filter Items...", windowWidth);

                        // ...

                        // Modify the loop for items to check against the item filter.
                        if (ImGui::BeginListBox("##items", { windowWidth, 110.f }))
                        {
                            for (auto& item : vecDumpedItems)
                            {
                                if (!itemFilter.PassFilter(item.m_name.c_str()))
                                    continue;

                                ImGui::PushID(&item);
                                if (ImGui::Selectable(item.m_name.c_str(), pSelectedItem == &item))
                                {
                                    if (pSelectedItem == &item)
                                        pSelectedItem = nullptr;
                                    else
                                        pSelectedItem = &item;
                                }
                                ImGui::PopID();
                            }
                            ImGui::EndListBox();
                        }
                        static char skinFilterText[128] = "";

                        if (pSelectedItem)
                        {
                            if (!pSelectedItem->m_dumpedSkins.empty())
                            {
                                static ImGuiTextFilter skinFilter;
                                skinFilter.Draw("Type here to filter Skins...", windowWidth);
                                if (ImGui::BeginListBox("##skins", { windowWidth, 110.f }))
                                {
                                    for (auto& skin : pSelectedItem->m_dumpedSkins)
                                    {
                                        if (!skinFilter.PassFilter(skin.m_name.c_str()))
                                            continue;

                                        ImGui::PushID(&skin);
                                        if (ImGui::Selectable(
                                            skin.m_name.c_str(),
                                            pSelectedItem->pSelectedSkin == &skin))
                                        {
                                            if (pSelectedItem->pSelectedSkin == &skin)
                                                pSelectedItem->pSelectedSkin = nullptr;
                                            else
                                                pSelectedItem->pSelectedSkin = &skin;
                                        }
                                        ImGui::PopID();
                                    }
                                    ImGui::EndListBox();
                                }
                            }

                            char buttonLabel[128];
                            snprintf(buttonLabel, 128, "Add every %s skin",
                                pSelectedItem->m_name.c_str());

                            if (ImGui::Button(buttonLabel, ImVec2(120, 55)))
                            {
                                for (const auto& skin : pSelectedItem->m_dumpedSkins)
                                {
                                    CEconItem* pItem = CEconItem::CreateInstance();
                                    if (pItem)
                                    {
                                        CCSPlayerInventory* pInventory = GSF::pInventoryManager->GetLocalInventory();

                                        auto highestIDs = pInventory->GetHighestIDs();

                                        pItem->m_ulID = highestIDs.first + 1;
                                        pItem->m_unInventory = highestIDs.second + 1;
                                        pItem->m_unAccountID =
                                            uint32_t(pInventory->GetOwner().m_id);
                                        pItem->m_unDefIndex = pSelectedItem->m_defIdx;
                                        if (pSelectedItem->m_unusualItem)
                                            pItem->m_nQuality = IQ_UNUSUAL;
                                        pItem->m_nRarity = std::clamp(
                                            pSelectedItem->m_rarity + skin.m_rarity - 1, 0,
                                            (skin.m_rarity == 7) ? 7 : 6);

                                        pItem->SetPaintKit((float)skin.m_ID);
                                        pItem->SetPaintSeed(1.f);
                                        if (pInventory->AddEconItem(pItem))
                                            changers::AddEconItemToList(pItem);
                                    }
                                }
                            }
                            ImGui::SameLine();
                            if (pSelectedItem->pSelectedSkin)
                            {
                                static float kitWear = 0.f;
                                static int kitSeed = 1;
                                static int gunKills = -1;
                                static char gunName[32];

                                bool vanillaSkin = pSelectedItem->pSelectedSkin->m_ID == 0;
                                snprintf(
                                    buttonLabel, 128, "Add %s%s%s",
                                    pSelectedItem->m_name.c_str(), vanillaSkin ? "" : " | ",
                                    vanillaSkin ? "" : pSelectedItem->pSelectedSkin->m_name.c_str());

                                if (ImGui::Button(buttonLabel, ImVec2(120, 55)))
                                {
                                    CEconItem* pItem = CEconItem::CreateInstance();
                                    if (pItem)
                                    {
                                        CCSPlayerInventory* pInventory = GSF::pInventoryManager->GetLocalInventory();

                                        auto highestIDs = pInventory->GetHighestIDs();

                                        pItem->m_ulID = highestIDs.first + 1;
                                        pItem->m_unInventory = highestIDs.second + 1;
                                        pItem->m_unAccountID =
                                            uint32_t(pInventory->GetOwner().m_id);
                                        pItem->m_unDefIndex = pSelectedItem->m_defIdx;

                                        if (pSelectedItem->m_unusualItem)
                                            pItem->m_nQuality = IQ_UNUSUAL;

                                        // I don't know nor do care why the rarity is calculated
                                        // like this. [Formula]
                                        pItem->m_nRarity = std::clamp(
                                            pSelectedItem->m_rarity +
                                            pSelectedItem->pSelectedSkin->m_rarity - 1,
                                            0,
                                            (pSelectedItem->pSelectedSkin->m_rarity == 7) ? 7 : 6);

                                        pItem->SetPaintKit(
                                            (float)pSelectedItem->pSelectedSkin->m_ID);
                                        pItem->SetPaintSeed((float)kitSeed);
                                        pItem->SetPaintWear(kitWear);

                                        if (gunKills >= 0)
                                        {
                                            pItem->SetStatTrak(gunKills);
                                            pItem->SetStatTrakType(0);

                                            // Applied automatically on knives.
                                            if (pItem->m_nQuality != IQ_UNUSUAL)
                                                pItem->m_nQuality = IQ_STRANGE;
                                        }

                                        if (pInventory->AddEconItem(pItem))
                                            changers::AddEconItemToList(pItem);

                                        kitWear = 0.f;
                                        kitSeed = 1;
                                        gunKills = -1;
                                        memset(gunName, '\0', IM_ARRAYSIZE(gunName));
                                        pItem->SetCustomName(gunName);
                                    }
                                }

                                ImGui::Dummy({ 0, 8 });
                                ImGui::SeparatorText("Extra settings");

                                ImGui::TextUnformatted("Wear Rating");
                                ImGui::SetNextItemWidth(windowWidth);
                                ImGui::SliderFloat("##slider1", &kitWear, 0.f, 1.f, "%.9f",
                                    ImGuiSliderFlags_Logarithmic);

                                ImGui::TextUnformatted("Pattern Template");
                                ImGui::SetNextItemWidth(windowWidth);
                                ImGui::SliderInt("##slider2", &kitSeed, 1, 1000);

                                ImGui::TextUnformatted("StatTrak Count");
                                ImGui::SetNextItemWidth(windowWidth);
                                ImGui::SliderInt("##slider3", &gunKills, -1, INT_MAX / 2,
                                    gunKills == -1 ? "Not StatTrak" : "%d",
                                    ImGuiSliderFlags_Logarithmic);

                                ImGui::TextUnformatted("Custom Name");
                                ImGui::SetNextItemWidth(windowWidth);
                                ImGui::InputTextWithHint("##input1", "Default", gunName,
                                    IM_ARRAYSIZE(gunName));
                            }
                        }
                    }
                }
                ImGui::EndChild();
            } break;

            case 1: {
                widgets::child(ICON_FA_SLIDERS, CXOR("Main")); 
                {
                    widgets::checkbox(CXOR("Model changer"), &cfg.model_changer);

                    if (ImGui::Button(CXOR("Refresh"), ImVec2(ImGui::GetWindowWidth() - 25, 25)))
                        g_model_changer->update_player_models();
                }
                ImGui::EndChild();
            } break;
            }
        }
        ImGui::EndGroup();
        ImGui::SameLine(0, 12);
        ImGui::BeginGroup();
        {
            switch (MENU::skins_subtab) {
            case 0: {
                widgets::child(ICON_FA_SORT, CXOR("Items")); {
                    ImGui::Text(CXOR("null"));
                }
                ImGui::EndChild();
            } break;

            case 1: {
                widgets::child(ICON_FA_SORT, CXOR("Items")); 
                {
                    for (size_t i = 0U; i < g_model_changer->player_models.size(); i++)
                    {
                        Model_t pModel = g_model_changer->player_models[i];

                        if (widgets::selectable(pModel.name.c_str(), (g_model_changer->cur_player_model == i)))
                            g_model_changer->cur_player_model = i;
                    }
                }
                ImGui::EndChild();
            } break;
            }
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
}

void tab_main()
{
    const char* misc_items[] = { ("General") };
    render_subtabs(misc_items, &MENU::misc_subtab, IM_ARRAYSIZE(misc_items));

    ImGui::SetCursorPos(BeginChildPos);
    ImGui::BeginChild(CXOR("##Misc_tab"), BeginChildSize, true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
    {
        auto& cfg = cfg_var::m_cfg->g_misc;

        ImGui::BeginGroup();
        {
            switch (MENU::misc_subtab) {
            case 0: {
                widgets::child(ICON_FA_SLIDERS, CXOR("Main"));
                {
                    widgets::checkbox(CXOR("Anti untrusted"), &cfg.antiuntrusted);
                    widgets::checkbox(CXOR("Bunny hop"), &cfg.bunny);
                    widgets::checkbox(CXOR("Auto strafe"), &cfg.strafe);

                    if (cfg.strafe)
                        widgets::slider_float(CXOR("Strafe smooth"), &cfg.strafesmootch, 0.f, 100.f, CXOR("%.2f"), NULL);

                    widgets::separator(CXOR(""));

                    widgets::slider_float(CXOR("Fov override"), &cfg.ifov, 60, 160, CXOR("%.3f"), NULL);
                    widgets::checkbox(CXOR("View model"), &cfg.viewover);

                    if (cfg.viewover)
                    {
                        widgets::slider_float(CXOR("View model fov"), &cfg.viewfov, -10, 120, CXOR("%.3f"), NULL);
                        widgets::slider_float(CXOR("View model X"), &cfg.viewX, -10, 10, CXOR("%.3f"), NULL);
                        widgets::slider_float(CXOR("View model Y"), &cfg.viewY, -10, 10, CXOR("%.3f"), NULL);
                        widgets::slider_float(CXOR("View model Z"), &cfg.viewZ, -10, 10, CXOR("%.3f"), NULL);
                    }

                    if (ImGui::Button(CXOR("Unload"), ImVec2(ImGui::GetWindowWidth() - 25, 25)))
                        cheat->unload = true;
                }
                ImGui::EndChild();
            } break;
            }
        }
        ImGui::EndGroup();
        ImGui::SameLine(0, 12);
        ImGui::BeginGroup();
        {
            const char* buy_list1[] { ("None"), ("Auto"), ("Awp"), ("SSG 08") };
            const char* buy_list2[] { ("None"), ("Elite"), ("Five/Tec"), ("Heavy") };
            const char* buy_list3[] { ("Armor"), ("Taser"), ("All grenades"), ("Defuse") };
            const char* custom_scope[] { ("None"), ("Static"), ("Gradient") };

            switch (MENU::misc_subtab) {
            case 0: {
                widgets::child(ICON_FA_USERS_VIEWFINDER, CXOR("View"));
                {
                    if (widgets::begin_combo(CXOR("Event logs"), CXOR("Select logs"), 3))
                    {
                        widgets::checkbox(CXOR("Hit"), &cfg.eventlogs[0]);
                        widgets::checkbox(CXOR("Kill"), &cfg.eventlogs[1]);
                        widgets::checkbox(CXOR("Miss"), &cfg.eventlogs[2]);

                        ImGui::EndCombo();
                    }

                    widgets::checkbox(CXOR("Remove legs"), &cfg_var::m_cfg->g_visuals.removelegs);

                    widgets::keybind(CXOR("Force third person"), &cfg.thirdperson, &mode);
                    widgets::slider_int(CXOR("Third person dist"), &cfg.thirdpersondist, 20, 200, CXOR("%d%"));

                    widgets::separator(CXOR(""));

                    widgets::checkbox(CXOR("Aspect ratio"), &cfg.enable_aspectratio);

                    if (cfg.enable_aspectratio)
                        widgets::slider_float(CXOR("Aspect ratio fov"), &cfg.aspectratio, 0.50f, 2.f, CXOR("%.2f"), NULL);

                    widgets::separator(CXOR(""));

                    widgets::combo(CXOR("Custom scope"), &cfg.m_scopelocal, custom_scope, IM_ARRAYSIZE(custom_scope), sizeof custom_scope);

                    if (cfg.m_scopelocal == 2)
                    {
                        widgets::slider_int(CXOR("Scope size"), &cfg.scope_size, 2, 300, CXOR("%d%"));
                        widgets::slider_int(CXOR("Scope thickness"), &cfg.scope_thickness, 1, 5, CXOR("%d%"));
                        widgets::slider_int(CXOR("Scope gap"), &cfg.scope_gap, 0, 100, CXOR("%d%"));

                        widgets::separator(CXOR(""));

                        widgets::color(CXOR("Scope color-in"), cfg.colCustomScope1.colValue);
                        widgets::color(CXOR("Scope color-out"), cfg.colCustomScope2.colValue);
                    }
                }
                ImGui::EndChild();
            } break;
            }
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
}

void tab_cloud()
{
    const char* cloud_items[] = { ("Configs"), ("Scripts") };
    render_subtabs(cloud_items, &MENU::cloud_subtab, IM_ARRAYSIZE(cloud_items));

    static char name[64u]{};
    static int cur_cfg = -1;
    static auto configs = config_sdk::g_cfg->get_configs();

    ImGui::SetCursorPos(BeginChildPos);
    ImGui::BeginChild(CXOR("##Cloud_tab"), BeginChildSize, true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
    {
        ImGui::BeginGroup();
        {
            switch (MENU::cloud_subtab) {
            case 0: {
                widgets::child(ICON_FA_SLIDERS, CXOR("Main")); 
                {
                    ImGui::InputTextWithHint(CXOR("Config name"), CXOR("Enter cfg name"), name, 64u, NULL);

                    if (ImGui::Button(CXOR("Create new"), ImVec2(ImGui::GetWindowWidth() - 25, 25)))
                    {
                        if (strlen(name) == 0)
                            break;

                        if (config_sdk::g_cfg->save(name)) 
                        {
                            configs = config_sdk::g_cfg->get_configs();
                            cur_cfg = -1;
                        }
                    }

                    if (cur_cfg >= 0 && cur_cfg < configs.size())
                    {
                        widgets::separator(CXOR(""));

                        if (ImGui::Button(CXOR("Load"), ImVec2(ImGui::GetWindowWidth() - 25, 25)))
                        {
                            if (config_sdk::g_cfg->load(configs[cur_cfg]))
                                printf(CXOR("loaded cfg: %s.\n"), configs[cur_cfg].c_str());
                        }

                        if (ImGui::Button(CXOR("Save"), ImVec2(ImGui::GetWindowWidth() - 25, 25)))
                        {
                            if (config_sdk::g_cfg->save(configs[cur_cfg])) {
                                printf(CXOR("saved cfg: %s.\n"), configs[cur_cfg].c_str());
                            }
                        }

                        if (ImGui::Button(CXOR("Delete"), ImVec2(ImGui::GetWindowWidth() - 25, 25)))
                        {
                            if (config_sdk::g_cfg->remove(configs[cur_cfg]))
                            {
                                configs = config_sdk::g_cfg->get_configs();
                                cur_cfg = -1;
                            }
                        }
                    }

                    widgets::separator(CXOR(""));

                    if (ImGui::Button(CXOR("Refresh"), ImVec2(ImGui::GetWindowWidth() - 25, 25)))
                    {
                        configs = config_sdk::g_cfg->get_configs();
                        cur_cfg = -1;
                    }

#ifdef _DEBUG
                    if (ImGui::Button(CXOR("Open cfg folder (DEBUG)"), ImVec2(ImGui::GetWindowWidth() - 25, 25)))
                        ShellExecuteA(NULL, CXOR("open"), config_sdk::g_cfg->cfg_folder().string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
#endif
                }
                ImGui::EndChild();
            } break;

            case 1: {
                widgets::child(ICON_FA_SLIDERS, CXOR("Main")); {
                    ImGui::Text(CXOR("Later..."));
                }
                ImGui::EndChild();
            } break;
            }
        }
        ImGui::EndGroup();
        ImGui::SameLine(0, 12);
        ImGui::BeginGroup();
        {
            switch (MENU::cloud_subtab) {
            case 0: {
                widgets::child(ICON_FA_SERVER, CXOR("List")); 
                {
                    for (size_t i = 0; i < configs.size(); i++)
                    {
                        const bool is_selected = (cur_cfg == i);

                        if (widgets::selectable(configs[i].c_str(), is_selected))
                            cur_cfg = i;

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndChild();
            } break;

            case 1: {
                widgets::child(ICON_FA_SERVER, CXOR("List")); {
                    ImGui::Text(CXOR("null"));
                }
                ImGui::EndChild();
            } break;
            }
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
}

void navbar()
{
    ImGui::BeginChild(CXOR("##Navbar"), ImVec2(179, ImGui::GetWindowSize().y - 25), false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
    {
        ImGui::SetCursorPos(ImVec2(15, 0));
        ImGui::BeginGroup();
        {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));

            widgets::separator(CXOR("Aim"));

            if (widgets::tab(ICON_FA_GUN, CXOR("Rage"), !tab)) tab = 0;
            if (widgets::tab(ICON_FA_HAMMER, "Legit", tab == 1)) tab = 1;

            widgets::separator(CXOR("Visuals"));

            if (widgets::tab(ICON_FA_USER, CXOR("General"), tab == 2)) tab = 2;

            widgets::separator(CXOR("Miscellaneous"));

            if (widgets::tab(ICON_FA_GEAR, CXOR("Skins"), tab == 3)) tab = 3;
            if (widgets::tab(ICON_FA_CUBE, CXOR("Main"), tab == 4)) tab = 4;
            if (widgets::tab(ICON_FA_CLOUD, CXOR("Cloud"), tab == 5)) tab = 5;

            ImGui::PopStyleVar();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
}

void MENU::RenderMainWindow()
{
    // animation
    static float menu_alpha = 0.f;
    const float anim_speed = ImGui::GetIO().DeltaTime * 8.f;

    if (cheat->menu_open)
        menu_alpha = ImClamp(menu_alpha + anim_speed, 0.f, 1.f);
    else
        menu_alpha = ImClamp(menu_alpha - anim_speed, 0.f, 1.f);

    if (menu_alpha <= 0.f)
        return;

    // click effect
    static std::vector<ClickData> clicks;

    auto im_io = ImGui::GetIO();
    auto s_size = im_io.DisplaySize;

    if (im_io.MouseClicked[0])
        clicks.push_back({ ImGui::GetMousePos() });

    for (size_t i = 0; i < clicks.size(); ++i)
    {
        clicks[i].value = ImClamp(clicks[i].value + 0.1f, 0.f, 1.f);

        ImGui::GetForegroundDrawList()->AddCircleFilled(
            clicks[i].pos,
            10 * clicks[i].value,
            ImGui::GetColorU32(ImGuiCol_FrameBg, (1 - clicks[i].value) * menu_alpha)
        );

        if (clicks[i].value == 1.f)
            clicks.erase(clicks.begin() + i);
    }

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, menu_alpha);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 12));
    ImGui::SetNextWindowSize(wsize, ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2((s_size.x - wsize.x) / 2.f, (s_size.y - wsize.y) / 2.f), ImGuiCond_FirstUseEver);
    ImGui::Begin(CXOR("##Menu"), NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
    {
        ImVec2 p = ImGui::GetWindowPos();
        ImDrawList* d = ImGui::GetWindowDrawList();

        auto add_alpha = [](ImColor col, float alpha) {
            return ImColor(col.Value.x, col.Value.y, col.Value.z, col.Value.w * alpha);
            };

        d->AddRectFilled(p, ImVec2(p.x + 179, p.y + wsize.y - 25),
            add_alpha(ImColor(14, 14, 16), menu_alpha), ImGui::GetStyle().WindowRounding, ImDrawFlags_RoundCornersTopLeft);

        d->AddRectFilled(ImVec2(p.x + 181, p.y), ImVec2(p.x + wsize.x, p.y + wsize.y - 25),
            add_alpha(ImColor(16, 16, 18), menu_alpha), ImGui::GetStyle().WindowRounding, ImDrawFlags_RoundCornersTopRight);

        d->AddRectFilled(ImVec2(p.x, p.y + wsize.y - 25), p + wsize,
            add_alpha(ImColor(23, 23, 25), menu_alpha), ImGui::GetStyle().WindowRounding, ImDrawFlags_RoundCornersBottom);

        d->AddText(ImVec2(p.x + 15, p.y + wsize.y - 22),
            ImGui::GetColorU32(ImVec4(1, 1, 1, 0.6f * menu_alpha)), CXOR("Your Text"));

        d->AddRectFilled(ImVec2(p.x + 179, p.y), ImVec2(p.x + 181, p.y + wsize.y - 25),
            add_alpha(ImColor(21, 21, 23), menu_alpha));

        d->AddRectFilled(ImVec2(p.x + 181, p.y + 59), ImVec2(p.x + wsize.x, p.y + 61),
            add_alpha(ImColor(23, 23, 25), menu_alpha));

        navbar();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
        switch (tab)
        {
        case 0: tab_ragebot(); break;
        case 1: tab_legitbot(); break;
        case 2: tab_visuals(); break;
        case 3: tab_skins(); break;
        case 4: tab_main(); break;
        case 5: tab_cloud(); break;
        }
        ImGui::PopStyleVar();
    }
    ImGui::End();
    ImGui::PopStyleVar(3);
}