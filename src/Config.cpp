#include "baseItem/Config.h"
#include "baseItem/MessageBox.h"
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>

Config* Config::instance = nullptr;
Config* Config::default_config = nullptr;

Config::Config()
{
    if (std::filesystem::exists(config_path))
    {
        std::filesystem::path configPath(config_path);
        std::cout<<"Loading config from "<<configPath<<std::endl;
        std::ifstream file(configPath);
        if(!file.is_open())
        {
            std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
            msgbox->setActive(true);
            msgbox->setMessage("Failed to open config file");
            msgbox->setTitle("Error");
            boxes.push_back(msgbox);
        }
        try{
            std::string content((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
            file.close();
            nlohmann::json jsonConfig;
            try{
                jsonConfig=nlohmann::json::parse(content);
                config_json = jsonConfig;
            }
            catch(const nlohmann::json::parse_error& e)
            {
                std::string message="Failed to parse config file: "+std::to_string(e.byte);
                std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
                msgbox->setActive(true);
                msgbox->setTitle("Error");
                msgbox->setMessage(message);
                boxes.push_back(msgbox);
                if(!tryRepair(content)){
                    std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
                    msgbox->setActive(true);
                    msgbox->setTitle("Error");
                    msgbox->setMessage("Failed to repair config file");
                    boxes.push_back(msgbox);
                    return;
                }
                try{
                    std::ofstream file(config_path);
                    if(!file.is_open()){
                        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
                        msgbox->setActive(true);
                        msgbox->setTitle("Error");
                        msgbox->setMessage("Failed to open config file");
                        boxes.push_back(msgbox);
                    }
                    jsonConfig=nlohmann::json::parse(content);
                    file<< jsonConfig.dump(4);
                }
                catch(const nlohmann::json::parse_error& e){
                    std::string message="Failed to parse config file after repair: "+std::to_string(e.byte);
                    std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
                    msgbox->setActive(true);
                    msgbox->setTitle("Error");
                    msgbox->setMessage(message);
                    boxes.push_back(msgbox);
                }
            }
        }
        catch(...){
            std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
            msgbox->setActive(true);
            msgbox->setTitle("Error");
            msgbox->setMessage("Unknown error in Config.cpp");
            boxes.push_back(msgbox);
        }
    }
    setDefaultConfig();
}

bool Config::saveToFile()const{
    if(is_default)return true;
    std::ofstream file(config_path);
    if(!file.is_open()){
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Failed to open config file");
        boxes.push_back(msgbox);
        return false;
    }
    file << config_json.dump(4);
    file.flush();
    file.close();
    return true;
}
using namespace std;
void Config::set(const std::string& key, const nlohmann::json& value){
    config_json[key]=value;
}
void Config::set(const std::string& key, const Vector2& value){
    nlohmann::json vec;
    vec["x"]=to_string(value.x);
    vec["y"]=to_string(value.y);
    config_json[key]=vec;
}
void Config::set(const std::string& key, int value){
    config_json[key]=to_string(value);
}
void Config::set(const std::string& key, float value){
    config_json[key]=to_string(value);
}
void Config::set(const std::string& key, long long value){
    config_json[key]=to_string(value);
}
void Config::set(const std::string& key, bool value){
    config_json[key]=to_string(value);
}
void Config::set(const std::string& key, const std::string& value){
    config_json[key]=value;
}
void Config::set(const std::string& key, double value){
    config_json[key]=to_string(value);
}
void Config::set(const std::string& key, long value){
    config_json[key]=to_string(value);
}
void Config::set(const std::string& key, unsigned long value){
    config_json[key]=to_string(value);
}
void Config::set(const std::string& key, unsigned long long value){
    config_json[key]=to_string(value);
}

void Config::setifno(const std::string& key, const nlohmann::json& value){
    getDefault()->set(key,value);
    if(!config_json.contains(key)){
        set(key,value);
    }
}
void Config::setifno(const std::string& key, const Vector2& value){
    getDefault()->set(key,value);
    if(!config_json.contains(key)){
        set(key,value);
    }
}
void Config::setifno(const std::string& key, int value){
    getDefault()->set(key,value);
    if(!config_json.contains(key)){
        set(key,value);
    }
}
void Config::setifno(const std::string& key, float value){
    getDefault()->set(key,value);
    if(!config_json.contains(key)){
        set(key,value);
    }
}
void Config::setifno(const std::string& key, long long value){
    getDefault()->set(key,value);
    if(!config_json.contains(key)){
        set(key,value);
    }
}
void Config::setifno(const std::string& key, bool value){
    getDefault()->set(key,value);
    if(!config_json.contains(key)){
        set(key,value);
    }
}
void Config::setifno(const std::string& key, const std::string& value){
    getDefault()->set(key,value);
    if(!config_json.contains(key)){
        set(key,value);
    }
}
void Config::setifno(const std::string& key, double value){
    getDefault()->set(key,value);
    if(!config_json.contains(key)){
        set(key,value);
    }
}
void Config::setifno(const std::string& key, long value){
    getDefault()->set(key,value);
    if(!config_json.contains(key)){
        set(key,value);
    }
}
void Config::setifno(const std::string& key, unsigned long value){
    getDefault()->set(key,value);
    if(!config_json.contains(key)){
        set(key,value);
    }
}
void Config::setifno(const std::string& key, unsigned long long value){
    getDefault()->set(key,value);
    if(!config_json.contains(key)){
        set(key,value);
    }
}

nlohmann::json Config::getJson(const std::string& key)const{
    if(key.empty())return config_json;
    if(!config_json.contains(key)){
        if(!is_default)
            return getDefault()->getJson(key);
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Config does not contain key: "+key);
        boxes.push_back(msgbox);
        return nlohmann::json();
    }
    return config_json[key];
}
Vector2 Config::getVector2(const std::string& key) const{
    if(!config_json.contains(key)){
        if(!is_default)
            return getDefault()->getVector2(key);
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Config key "+key+" is not a Vector2");
        boxes.push_back(msgbox);
        return Vector2(0,0);
    }
    nlohmann::json vec=config_json[key];
    if(!vec.contains("x")||!vec.contains("y")){
        if(!is_default)
            return getDefault()->getVector2(key);
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Config key "+key+" is not a Vector2");
        boxes.push_back(msgbox);
        return Vector2(0,0);
    }
    return Vector2(stof(vec.value("x","0.0")),stof(vec.value("y","0.0")));
}
int Config::getInt(const std::string& key) const{
    if(!config_json.contains(key)){
        if(!is_default)
            return getDefault()->getInt(key);
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Config does not contain key: "+key);
        boxes.push_back(msgbox);
        return 0;
    }
    return stoi(config_json.value(key, "0"));
}
float Config::getFloat(const std::string& key) const{
    if(!config_json.contains(key)){
        if(!is_default)
            return getDefault()->getFloat(key);
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Config does not contain key: "+key);
        boxes.push_back(msgbox);
        return 0.0f;
    }
    return stof(config_json.value(key, "0.0"));
}
long long Config::getLongLong(const std::string& key) const{
    if(!config_json.contains(key)){
        if(!is_default)
            return getDefault()->getLongLong(key);
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Config does not contain key: "+key);
        boxes.push_back(msgbox);
        return 0;
    }
    return stoll(config_json.value(key, "0"));
}
bool Config::getBool(const std::string& key) const{
    if(!config_json.contains(key)){
        if(!is_default)
            return getDefault()->getBool(key);
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Config does not contain key: "+key);
        boxes.push_back(msgbox);
        return false;
    }
    std::string val=config_json[key].get<string>();
    if(val=="true"||val=="1")return true;
    else if(val=="false"||val=="0")return false;
    else{
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Config key "+key+" is not a bool");
        boxes.push_back(msgbox);
        return false;
    }
}
std::string Config::getString(const std::string& key) const{
    if(!config_json.contains(key)){
        if(!is_default)
            return getDefault()->getString(key);
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Config key "+key+" is not a string");
        boxes.push_back(msgbox);
        return "";
    }
    return config_json[key].get<string>();
}
double Config::getDouble(const std::string& key) const{
    if(!config_json.contains(key)){
        if(!is_default)
            return getDefault()->getDouble(key);
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Config key "+key+" is not a double");
        boxes.push_back(msgbox);
        return 0.0;
    }
    return stod(config_json[key].get<string>());
}
long Config::getLong(const std::string& key) const{
    if(!config_json.contains(key)){
        if(!is_default)
            return getDefault()->getLong(key);
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Config does not contain key: "+key);
        boxes.push_back(msgbox);
        return 0;
    }
    return stol(config_json[key].get<string>());
}
unsigned long Config::getUnsignedLong(const std::string& key) const{
    if(!config_json.contains(key)){
        if(!is_default)
            return getDefault()->getUnsignedLong(key);
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Config does not contain key: "+key);
        boxes.push_back(msgbox);
        return 0;
    }
    return stoul(config_json[key].get<string>());
}
unsigned long long Config::getUnsignedLongLong(const std::string& key) const{
    if(!config_json.contains(key)){
        if(!is_default)
            return getDefault()->getUnsignedLongLong(key);
        std::shared_ptr<MessageBox> msgbox = std::make_shared<MessageBox>();
        msgbox->setActive(true);
        msgbox->setTitle("Error");
        msgbox->setMessage("Config does not contain key: "+key);
        boxes.push_back(msgbox);
        return 0;
    }
    return stoull(config_json[key].get<string>());
}

std::vector<Platform> Config::getPlatforms() const{
    std::vector<Platform> platforms;
    for(const auto& platform : config_json["platforms"]){
        platforms.push_back(Platform::fromJson(platform));
    }
    return platforms;
}

void Config::addPlatform(const Platform& value){
    config_json["platforms"].push_back(value.toJson());
}

void Config::addDefaultPlatforms(const std::vector<Platform>& value){
    if(is_default)return;
    for(const auto& platform : value){
        getDefault()->addPlatform(platform);
        if(!config_json.contains("platforms")||config_json["platforms"].size()<=value.size()){
            config_json["platforms"].push_back(platform.toJson());
        }
    }
}

void Config::setDefaultConfig(){
    if(is_default)return;
    setifno("window.fps",60);
    setifno("player.size",Vector2(96,96));
    setifno("player.gravity",1.6e-3f);
    setifno("player.run_velocity",0.55f);
    setifno("player.jump_velocity",-0.85f);
    setifno("player.max_hp",100.0f);
    setifno("player.default.attack_cd",500);
    setifno("player.default.attack.mp_reward",10);
    setifno("player.default.attackEx.mp_reward",10);
    setifno("player.default.damage",10);
    setifno("player.default.Exdamage",20);
    setifno("player.gloomshroom.attack_cd",700);
    setifno("player.gloomshroom.damage",25);
    setifno("player.gloomshroom.Exdamage",30);
    setifno("player.gloomshroom.attack.mp_reward",35);
    setifno("player.gloomshroom.attackEx.mp_reward",35);
    setifno("player.nut.attack_cd",50);
    setifno("player.nut.Exdamage",50);
    setifno("player.nut.dash_distance",100);
    setifno("player.nut.attackEx.mp_reward",50);
    setifno("player.peashooter.attack_cd",100);
    setifno("player.peashooter.pea.speed",0.75f);
    setifno("player.peashooter.peaEx.speed",1.5f);
    setifno("player.peashooter.peaEx.duration",2500);
    setifno("player.peashooter.damage",10);
    setifno("player.peashooter.attack.mp_reward",25);
    setifno("player.peashooter.attackEx.mp_reward",25);
    setifno("player.sunflower.attack_cd",300);
    setifno("player.sunflower.bullet.velocity",Vector2(0.25f, -0.5f));
    setifno("player.sunflower.bulletEx.speed",0.75f);
    setifno("player.sunflower.damage",20);
    setifno("player.sunflower.Exdamage",40);
    setifno("player.sunflower.attack.mp_reward",35);
    setifno("player.sunflower.attackEx.mp_reward",50);
    setifno("player.buttermshroom.attack_cd",1500);
    setifno("player.buttermshroom.damage",15);
    setifno("player.buttermshroom.Exdamage",80);
    setifno("player.buttermshroom.attack.mp_reward",35);
    setifno("player.buttermshroom.attackEx.mp_reward",35);
    setifno("player.timer.invulnerable",750);
    setifno("player.timer.cursor_visibility",2500);
    setifno("player.timer.buttered",3000);
    setifno("player.timer.silenced",1000);
    setifno("player.timer.recovering",5000);
    setifno("player.timer.hurrying",5000);
    setifno("player.timer.invisible",5000);
    setifno("player.multiplier.recovery",0.02f);
    setifno("player.multiplier.hurrying",1.5f);
    setifno("player.multiplier.effort",200);
    setifno("player.key.p1.left",'a');
    setifno("player.key.p1.right",'d');
    setifno("player.key.p1.jump",'w');
    setifno("player.key.p1.attack",'f');
    setifno("player.key.p1.attackEx",'g');
    setifno("player.key.p2.left",SDLK_LEFT);
    setifno("player.key.p2.right",SDLK_RIGHT);
    setifno("player.key.p2.jump",SDLK_UP);
    setifno("player.key.p2.attack",SDLK_PERIOD);
    setifno("player.key.p2.attackEx",SDLK_SLASH);
    std::vector<Platform> default_platforms;
    default_platforms.resize(6);
    Platform& ground = default_platforms[0];
    ground.image=&img_platform_large;
    ground.render_pos.x=122;
    ground.render_pos.y=455;
    ground.shape = {(float)ground.render_pos.x+30, 
        (float)ground.render_pos.x+img_platform_large.getWidth()-30,
        (float)ground.render_pos.y+60
    };
    Platform& platform1 = default_platforms[1];
    platform1.image = &img_platform_small;
    platform1.render_pos = {200, 300};
    platform1.shape = {
        (float)platform1.render_pos.x+40,
        (float)platform1.render_pos.x + img_platform_small.getWidth()-40,
        (float)platform1.render_pos.y + img_platform_small.getHeight()/2
    };
    Platform& platform2 = default_platforms[2];
    platform2.image = &img_platform_small;
    platform2.render_pos = {400, 200};
    platform2.shape = {
        (float)platform2.render_pos.x+40,
        (float)platform2.render_pos.x + img_platform_small.getWidth()-40,
        (float)platform2.render_pos.y + img_platform_small.getHeight()/2
    };
    Platform& platform3 = default_platforms[3];
    platform3.image = &img_platform_small;
    platform3.render_pos = {600, 100};
    platform3.shape = {
        (float)platform3.render_pos.x+40,
        (float)platform3.render_pos.x + img_platform_small.getWidth()-40,
        (float)platform3.render_pos.y + img_platform_small.getHeight()/2
    };
    Platform& platform4 = default_platforms[4];
    platform4.image = &img_platform_small;
    platform4.render_pos = {800, 200};
    platform4.shape = {
        (float)platform4.render_pos.x+40,
        (float)platform4.render_pos.x + img_platform_small.getWidth()-40,
        (float)platform4.render_pos.y + img_platform_small.getHeight()/2
    };
    Platform& platform5 = default_platforms[5];
    platform5.image = &img_platform_small;
    platform5.render_pos = {1000, 300};
    platform5.shape = {
        (float)platform5.render_pos.x+40,
        (float)platform5.render_pos.x + img_platform_small.getWidth()-40,
        (float)platform5.render_pos.y + img_platform_small.getHeight()/2
    };
    addDefaultPlatforms(default_platforms);
    saveToFile();
}

bool Config::tryRepair(std::string& content){
    auto noExtraCommas=[](std::string& str)->bool{
        bool fixed=false;
        std::regex opj(R"(,\s*})");
        std::string fix_str = std::regex_replace(str, opj, "}");

        std::regex array(R"(,\s*\])");
        fix_str = std::regex_replace(str, array, "]");
        if(fix_str!=str){
            str=fix_str;
            return true;
        }
        return false;
    };
    auto fixQuotes=[](std::string& str)->bool{
        std::regex missingQuoteKey(R"(([{,]\s*)([a-zA-Z0-9_]+)(\s*:))");
        std::string fix_str=std::regex_replace(str,missingQuoteKey,"$1\"$2\"$3");
        if(fix_str!=str){
            str=fix_str;
            return true;
        }
        return false;
    };
    auto completeJson=[](std::string& str)->bool{
        bool fixed=false;
        int openBraces=0,closeBraces=0;
        int openBrackets=0,closeBrackets=0;

        for(auto c:str){
            if(c=='{')openBraces++;
            else if(c=='}')closeBraces++;
            else if(c=='[')openBrackets++;
            else if(c==']')closeBrackets++;
        }
        while(openBraces>closeBraces){
            str.push_back('}');
            closeBraces++;
            fixed=true;
        }
        return fixed;
    };
    bool fixed=false;
    fixed=noExtraCommas(content)||fixed;
    fixed=fixQuotes(content)||fixed;
    fixed=completeJson(content)||fixed;
    return fixed;
}