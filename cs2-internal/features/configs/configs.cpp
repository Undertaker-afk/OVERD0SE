#include "configs.h"

#include "../utilities/common.h"

#define SDK_CFG_ID_OBJECT CXOR("data")
#define SDK_CFG_ID_PREFIX CXOR(".json")

namespace config_sdk
{
#ifdef _DEBUG
    std::filesystem::path c_cfg::cfg_folder()
    {
        auto path = std::filesystem::current_path() / CXOR("cfgs");

        // if there is a pahc

        if (!std::filesystem::exists(path))
        {
            // if the path to the res is not found

            if (!std::filesystem::create_directory(path))
                return CXOR("");
        }

        return path;
    }
#endif

    void c_cfg::add_var(detail::base_cfg_var_t* var)
    {
        // skip not founded var
        if (!var)
            return;

        m_vars.emplace_back(var);
    }

    detail::base_cfg_var_t* c_cfg::find_var(std::size_t hash)
    {
        for (auto var : m_vars)
        {
            // skip not founded var
            if (!var)
                continue;

            if (var->hash() == hash)
                return var;
        }

        return nullptr;
    }

    bool c_cfg::save(const std::string& name)
    {
        // cfg empty
        if (name.empty())
            return false;

        try 
        {
            nlohmann::json config;
            for (auto var : m_vars)
            {            
                // skip not founded var
                if (!var)
                    continue;

                var->save(config[SDK_CFG_ID_OBJECT]);
            }

            auto path = cfg_folder() / (name + SDK_CFG_ID_PREFIX);
            if (path.empty()) 
                return false;

            std::ofstream file(path);
            if (!file.is_open())
                return false;

            // dump json
            file << config.dump(4);
            file.close();

            return true;
        }
        catch (const std::exception& e) 
        {
            printf(CXOR("exception during cfg save: %s.\n"), e.what());
            return false;
        }
        catch (...) 
        {
            printf(CXOR("unknown error during cfg save.\n"));
            return false;
        }
    }

    bool c_cfg::load(const std::string& name)
    {
        // cfg empty

        if (name.empty())
            return false;

        try
        {
            auto path = cfg_folder() / (name + SDK_CFG_ID_PREFIX);
            if (!std::filesystem::exists(path))
                return false;

            std::ifstream file(path);
            if (!file.is_open())
                return false;

            nlohmann::json config;
            file >> config;
            file.close();

            if (config.contains(SDK_CFG_ID_OBJECT))
            {
                auto& cfg_data = config[SDK_CFG_ID_OBJECT];
                for (auto var : m_vars)
                {
                    // skip not founded var
                    if (!var)
                        continue;

                    var->load(cfg_data);
                }
            }

            return true;
        }
        catch (const std::exception& e) 
        {
            printf(CXOR("exception during cfg load: %s.\n"), e.what());
            return false;
        }
        catch (...) 
        {
            printf(CXOR("unknown error during cfg load.\n"));
            return false;
        }
    }

    std::vector<std::string> c_cfg::get_configs()
    {
        std::vector<std::string> cfg;

        try 
        {
            auto folder = cfg_folder();
            if (folder.empty()) 
                return cfg;

            for (const auto& entry : std::filesystem::directory_iterator(folder))
            {
                if (entry.path().extension() == SDK_CFG_ID_PREFIX)
                    cfg.push_back(entry.path().stem().string());
            }

            if (cfg.empty())
                printf(CXOR("cfg folder empty.\n"));
        }
        catch (const std::exception& e) {
            printf(CXOR("exception during cfg list: %s.\n"), e.what());
        }
        catch (...) {
            printf(CXOR("unknown error during cfg list.\n"));
        }

        return cfg;
    }

    bool c_cfg::remove(const std::string& name)
    {
        if (name.empty())
            return false;

        try
        {
            auto path = cfg_folder() / (name + SDK_CFG_ID_PREFIX);
            if (!std::filesystem::exists(path))
                return false;

            bool res = std::filesystem::remove(path);

            if (!res)
                printf(CXOR("fail during cfg removing: %s.\n"), path.string().c_str());

            return res;
        }
        catch (const std::exception& e) 
        {
            printf(CXOR("exception during cfg removing: %s.\n"), e.what());
            return false;
        }
        catch (...) 
        {
            printf(CXOR("unknown error during cfg removing.\n"));
            return false;
        }
    }
}