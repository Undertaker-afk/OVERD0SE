#pragma once

#include "../../libs/json/jsonv2.hpp"

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

namespace config_sdk
{
    namespace detail
    {
        struct base_cfg_var_t
        {
        protected:
            std::size_t m_hash{};
        public:
            constexpr base_cfg_var_t() = default;
            constexpr base_cfg_var_t(std::size_t hash) : m_hash(hash) {}

            virtual void save(nlohmann::json& object) const = 0;
            virtual void load(const nlohmann::json& object) = 0;

            std::size_t hash() const { return m_hash; }
        };
    }

    class c_cfg
    {
    private:
        static constexpr std::uint8_t k_byte_xor = 0xffu;
        std::vector<detail::base_cfg_var_t*> m_vars{};

    public:
        void add_var(detail::base_cfg_var_t* var);
        detail::base_cfg_var_t* find_var(std::size_t hash);

        std::vector<std::string> get_configs();

#ifdef _DEBUG
        std::filesystem::path cfg_folder();
#endif

        bool save(const std::string& name);
        bool load(const std::string& name);
        bool remove(const std::string& name);
    };

    inline c_cfg* g_cfg = new c_cfg();

    template<typename _value_t>
    struct cfg_var_t final : public detail::base_cfg_var_t {
    private:
        _value_t m_value{};
    public:
        __forceinline constexpr cfg_var_t() = default;
        __forceinline cfg_var_t(std::size_t hash, const _value_t& value) : detail::base_cfg_var_t(hash), m_value(value) 
        {
            if (g_cfg)
                g_cfg->add_var(this);
        }

        __forceinline _value_t* operator&() { return &m_value; }
        __forceinline operator _value_t() const { return m_value; }
        __forceinline _value_t* operator->() { return &m_value; }
        __forceinline const _value_t* operator->() const { return &m_value; }
        __forceinline _value_t& value() { return m_value; }
        __forceinline const _value_t& value() const { return m_value; }

        void save(nlohmann::json& object) const override 
        {
            auto& arr = object[std::to_string(m_hash)];

            if constexpr (std::is_same_v<_value_t, std::string>) 
            {
                for (auto chr : m_value) 
                    arr.emplace_back(chr);

                return;
            }

            for (size_t i = 0; i < sizeof(_value_t); ++i)
                arr.emplace_back(reinterpret_cast<const uint8_t*>(&m_value)[i]);
        }

        void load(const nlohmann::json& object) override 
        {
            auto arr = object.find(std::to_string(m_hash));
            if (arr == object.end()) 
                return;

            auto& bytes = arr.value();
            if (!bytes.is_array() || bytes.size() != sizeof(_value_t)) 
                return;

            if constexpr (std::is_same_v<_value_t, std::string>) 
            {
                m_value.clear();

                for (auto& chr : bytes.items())
                    m_value.push_back(chr.value().template get<char>());

                return;
            }

            for (size_t i = 0; i < sizeof(_value_t); ++i)
                reinterpret_cast<uint8_t*>(&m_value)[i] = bytes.at(i).template get<uint8_t>();
        }
    };
}