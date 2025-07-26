#include "modelchanger.h"

#include <iostream>
#include <filesystem>

#include "../../sdk/interfaces/iresourcesystem.h"
#include "../../sdk/interfaces/iengineclient.h"
#include "../../sdk/entity.h"

// @credits: makstv8888

void c_model_changer::update_player_models()
{
    // clear
    if (!player_models.empty())
        player_models.clear();

    for (const auto& entry : std::filesystem::recursive_directory_iterator(parser::GetDirectory() + CXOR("csgo\\characters\\models\\")))
    {
        if (!entry.is_regular_file())
            continue;

        std::string file_name = entry.path().filename().string();

        if (file_name.size() < 6)
            continue;

        if (file_name.substr(file_name.size() - 7) != CXOR(".vmdl_c"))
            continue;

        if (file_name.find(CXOR("arm")) != std::string::npos)
            continue;

        // set path
        std::string model_path = entry.path().string();
        model_path = model_path.substr(0, model_path.size() - 2);
        std::replace(model_path.begin(), model_path.end(), '\\', '/');

        size_t pos = model_path.find(CXOR("characters/models/"));
        if (pos != std::string::npos)
            model_path = model_path.substr(pos);

        // GGOOOOOOOOOOOOOOOOOOOOOOOOOO
        Model_t pModel;

        pModel.name = file_name.substr(0, file_name.size() - 8);
        pModel.path = model_path;

        // set models
        player_models.push_back(pModel);
    }
}

void c_model_changer::set_player_model()
{
    if (!I::Engine->IsInGame() || !I::Engine->IsConnected())
        return;

    if (!SDK::LocalPawn || !SDK::LocalPawn->is_alive())
        return;

    if (cur_player_model == ~1U)
        return;

    if (player_models.size() <= cur_player_model)
        return;

    Model_t pModel = player_models[cur_player_model];
    const char* model_path = pModel.path.c_str();

    auto model_hash = FNV1A::Hash(model_path);
    if (last_player_model_hash == model_hash)
        return;

    last_player_model_hash = model_hash;

    I::ResourceSystem->PreCache(model_path);
    SDK::LocalPawn->SetModel(model_path);

    return;
}