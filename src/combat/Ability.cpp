#include "Ability.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

static TargetType ParseTarget(const std::string& s) {
    if (s == "self") return TargetType::Self;
    if (s == "all_enemies") return TargetType::AllEnemies;
    return TargetType::Enemy;
}

static EffectType ParseEffect(const std::string& s) {
    if (s == "heal") return EffectType::Heal;
    return EffectType::Damage;
}

void AbilityDatabase::LoadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "ERROR: Could not open ability file: " << path << std::endl;
        return;
    }

    json data = json::parse(file);

    for (const auto& entry : data["abilities"]) {
        Ability a;
        a.id          = entry.value("id", "");
        a.name        = entry.value("name", a.id);
        a.description = entry.value("description", "");
        a.target      = ParseTarget(entry.value("target", "enemy"));
        a.cost        = entry.value("cost", 0);
        a.cooldown    = entry.value("cooldown", 0);

        if (entry.contains("effects")) {
            for (const auto& eff : entry["effects"]) {
                a.effects.push_back({ ParseEffect(eff.value("type", "damage")),
                                      eff.value("amount", 0) });
            }
        }

        if (a.id.empty()) {
            std::cout << "WARNING: skipping ability with no id in " << path << std::endl;
            continue;
        }
        abilities[a.id] = std::move(a);
    }

    std::cout << "Loaded " << abilities.size() << " abilities from " << path << std::endl;
}

const Ability* AbilityDatabase::Get(const std::string& id) const {
    auto it = abilities.find(id);
    return it == abilities.end() ? nullptr : &it->second;
}

Loadout BuildLoadout(const AbilityDatabase& db, const std::vector<std::string>& ids) {
    Loadout loadout;
    for (const std::string& id : ids) {
        const Ability* a = db.Get(id);
        if (!a) {
            std::cout << "WARNING: loadout references unknown ability '" << id << "'" << std::endl;
            continue;
        }
        loadout.slots.push_back({ a, 0 });
    }
    return loadout;
}
