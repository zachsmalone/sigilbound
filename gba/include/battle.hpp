#pragma once
namespace sigilbound {
enum class Result { active, victory, defeat };
struct Battle {
    int hp = 24, enemy_hp = 18, tackle = 35, brace = 20, xp = 0;
    Result result = Result::active;
    void reset() { *this = Battle{}; }
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
};
}
