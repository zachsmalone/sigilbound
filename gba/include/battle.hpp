#pragma once
#include <array>
#include <string>
namespace sigilbound {
enum class Result { active, victory, defeat };
struct Move {
    const char* name = "";
    int uses = 0;
    int max_uses = 0;
    int damage = 0;
    bool defensive = false;
};
struct Beast {
    std::string id;
    int hp = 0;
    int max_hp = 0;
    std::array<Move, 4> moves{};
    int equipped = 0; // number of valid moves in the array
};
struct Team {
    std::array<Beast, 5> members{};
    int active = 0;
    int count = 0;
};
struct Battle {
    int hp = 24, enemy_hp = 18, tackle = 35, brace = 20, xp = 0;
    Result result = Result::active;
    Team player{};
    void reset() { *this = Battle{}; }
    // Existing diagnostic act() preserved for regression.
    bool act(bool defend) {
        if (result != Result::active || (defend ? brace : tackle) == 0) return false;
        if (defend) --brace;
        else {
            --tackle;
            enemy_hp = enemy_hp > 3 ? enemy_hp - 3 : 0;
            if (enemy_hp == 0) { result = Result::victory; xp = 20; return true; }
        }
        const int damage = defend ? 1 : 2;
        hp = hp > damage ? hp - damage : 0;
        if (hp == 0) result = Result::defeat;
        return true;
    }
    // Stub for next step: select among up to 4 equipped moves on the active beast.
    // Not wired into the ROM yet. Gated on emulator verification of the current battle slice.
    bool act_move(int slot) {
        if (result != Result::active || player.count == 0) return false;
        if (slot < 0 || slot >= player.members[player.active].equipped) return false;
        // Placeholder: still uses the diagnostic numbers until full rules are implemented.
        return act(false);
    }
};
}
