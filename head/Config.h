#pragma once

#include <nlohmann/json.hpp>
#include "Vector2.h"
#include "platform.h"

class Config
{
    static Config* instance;
    static Config* default_config;
    nlohmann::json config_json;
    std::string config_path = "config.json";
    bool is_default = false;
    Config();
    Config(bool isDefault) : is_default(isDefault) {}
    ~Config() = default;
    bool tryRepair(std::string& content);
    static Config* getDefault(){
        if(default_config==nullptr){
            default_config=new Config(true);
        }
        return default_config;
    }
    void setDefaultConfig();
public:
    static Config* getInstance(){
        if(instance==nullptr){
            instance=new Config();
        }
        return instance;
    }
    nlohmann::json getJson(const std::string& key="")const;
    Vector2 getVector2(const std::string& key) const;
    int getInt(const std::string& key) const;
    float getFloat(const std::string& key) const;
    long long getLongLong(const std::string& key) const;
    bool getBool(const std::string& key) const;
    std::string getString(const std::string& key) const;
    double getDouble(const std::string& key) const;
    long getLong(const std::string& key) const;
    unsigned long getUnsignedLong(const std::string& key) const;
    unsigned long long getUnsignedLongLong(const std::string& key) const;

    void set(const std::string& key, const nlohmann::json& value);
    void set(const std::string& key, const Vector2& value);
    void set(const std::string& key, int value);
    void set(const std::string& key, float value);
    void set(const std::string& key, long long value);
    void set(const std::string& key, bool value);
    void set(const std::string& key, const std::string& value);
    void set(const std::string& key, double value);
    void set(const std::string& key, long value);
    void set(const std::string& key, unsigned long value);
    void set(const std::string& key, unsigned long long value);

    void setifno(const std::string& key, const nlohmann::json& value);
    void setifno(const std::string& key, const Vector2& value);
    void setifno(const std::string& key, int value);
    void setifno(const std::string& key, float value);
    void setifno(const std::string& key, long long value);
    void setifno(const std::string& key, bool value);
    void setifno(const std::string& key, const std::string& value);
    void setifno(const std::string& key, double value);
    void setifno(const std::string& key, long value);
    void setifno(const std::string& key, unsigned long value);
    void setifno(const std::string& key, unsigned long long value);

    std::vector<Platform> getPlatforms() const;
    void addPlatform(const Platform& value);
    void addDefaultPlatforms(const std::vector<Platform>& value);

    bool saveToFile() const;
};