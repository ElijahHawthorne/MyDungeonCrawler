#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// How an ability chooses who it affects.
enum class TargetType { Self, Enemy, AllEnemies };

// A single thing an ability does. New kinds of effect get one new enum value
// here plus one new case in ApplyAbility (Combat.cpp).
enum class EffectType { Damage, Heal };

struct Effect {
    EffectType type;
    int amount;
};

// A data-driven ability definition. Loaded from assets/data/abilities.json;
// nothing here is hardcoded in C++, so balance tweaks and new abilities that
// reuse existing effect types need no recompile.
struct Ability {
    std::string id;
    std::string name;
    std::string description;
    TargetType  target = TargetType::Enemy;
    int         cost = 0;      // energy spent to use it
    int         cooldown = 0;  // turns before it can be used again
    std::string animation;     // player animation clip to play when used ("" = none)
    std::string impact;        // enemy hurt reaction: "slash", "fire" ("" = none)
    std::vector<Effect> effects;
};

// In-memory catalog of every ability, keyed by id. Populated once at startup.
class AbilityDatabase {
public:
    void LoadFromFile(const std::string& path);
    const Ability* Get(const std::string& id) const;
    size_t Count() const { return abilities.size(); }
private:
    std::unordered_map<std::string, Ability> abilities;
};

// One entry in a combatant's loadout: a resolved pointer into the database
// plus the per-combat cooldown state.
struct AbilitySlot {
    const Ability* ability = nullptr;
    int cooldownRemaining = 0;
};

// The set of abilities a combatant brings into a fight.
struct Loadout {
    std::vector<AbilitySlot> slots;
};

// Resolve a list of ability ids against the database into a ready-to-use
// loadout. Unknown ids are skipped with a warning.
Loadout BuildLoadout(const AbilityDatabase& db, const std::vector<std::string>& ids);
