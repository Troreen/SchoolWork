#include "ConfigLoader.h"

#include <fstream>
#include <nlohmann/json.hpp>

using nlohmann::json;

namespace
{
    GameConfig MakeDefaultConfig()
    {
        return GameConfig{};
    }

    template<typename T>
    T ReadOr(const json& j, const char* key, const T& defVal)
    {
        if (!j.is_object())
        {
            return defVal;
        }
        if (!j.contains(key))
        {
            return defVal;
        }
        try
        {
            return j.at(key).get<T>();
        }
        catch (...)
        {
            return defVal;
        }
    }
}

GameConfig ConfigLoader::Load(const std::string& aPath)
{
    std::ifstream in(aPath);
    if (!in.is_open())
    {
        return MakeDefaultConfig();
    }

    json j;
    try
    {
        in >> j;
    }
    catch (...)
    {
        return MakeDefaultConfig();
    }

    GameConfig cfg = MakeDefaultConfig();

    if (j.contains("player") && j["player"].is_object())
    {
        const json& p = j["player"];
        cfg.playerMoveSpeed = ReadOr<float>(p, "move_speed", cfg.playerMoveSpeed);
        cfg.playerGravity = ReadOr<float>(p, "gravity", cfg.playerGravity);
        cfg.playerJumpVelocity = ReadOr<float>(p, "jump_velocity", cfg.playerJumpVelocity);

        cfg.playerSprite = ReadOr<std::string>(p, "sprite", cfg.playerSprite);
    }

    if (j.contains("gameplay") && j["gameplay"].is_object())
    {
        const json& g = j["gameplay"];
        cfg.scrollSpeedScale = ReadOr<float>(g, "scroll_speed_scale", cfg.scrollSpeedScale);
    }

    if (j.contains("sprites") && j["sprites"].is_object())
    {
        const json& s = j["sprites"];
        cfg.obstacleSprite = ReadOr<std::string>(s, "obstacle", cfg.obstacleSprite);
        cfg.backgroundSprite = ReadOr<std::string>(s, "background", cfg.backgroundSprite);
    }

    if (j.contains("text_elements") && j["text_elements"].is_array()) {
        for (const auto& elem : j["text_elements"]) {
            TextElement te;
            te.text = elem.value("text", "");
            te.x = elem.value("x", 0.0f);
            te.y = elem.value("y", 0.0f);
            te.state = elem.value("state", "");
            cfg.textElements.push_back(te);
        }
    }

    if (j.contains("title_screen") && j["title_screen"].is_object())
    {
        const json& ts = j["title_screen"];
        if (ts.contains("texts") && ts["texts"].is_array())
        {
            for (const auto& t : ts["texts"]) {
                TitleText tt;
                tt.text = t.value("text", "");
                tt.x = t.value("x", 0.0f);
                tt.y = t.value("y", 0.0f);
                cfg.titleTexts.push_back(tt);
            }
        }
        if (ts.contains("images") && ts["images"].is_array())
        {
            for (const auto& im : ts["images"]) {
                TitleImage ti;
                ti.path = im.value("path", "");
                ti.x = im.value("x", 0.0f);
                ti.y = im.value("y", 0.0f);
                cfg.titleImages.push_back(ti);
            }
        }
    }

    return cfg;
}
