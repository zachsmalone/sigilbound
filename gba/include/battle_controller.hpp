#pragma once
#include "team.hpp"
namespace sigilbound {
enum class TeamBattleResult { active, victory, defeat };
enum class BattleMenu { root, moves, team, replacement, result };
class BattleController {
    Party initial_, party_;
    unsigned held_=0;
    int enemy_hp_=18, xp_=0;
    std::size_t cursor_=0;
    TeamBattleResult result_=TeamBattleResult::active;
    BattleMenu menu_=BattleMenu::root;
    void retaliate(int damage) {
        party_.damage_active(static_cast<std::uint16_t>(damage));
        if (party_.defeated()) { result_=TeamBattleResult::defeat; menu_=BattleMenu::result; }
        else if (!party_.beast(party_.active_index())->hp) menu_=BattleMenu::replacement;
        else menu_=BattleMenu::root;
        cursor_=0;
    }
public:
    explicit BattleController(const Party& party):initial_(party),party_(party) {
        if (party_.defeated()) { result_=TeamBattleResult::defeat; menu_=BattleMenu::result; }
        else if (!party_.beast(party_.active_index())->hp) menu_=BattleMenu::replacement;
    }
    const Party& party() const { return party_; }
    int enemy_hp() const { return enemy_hp_; }
    int xp() const { return xp_; }
    TeamBattleResult result() const { return result_; }
    BattleMenu menu() const { return menu_; }
    std::size_t cursor() const { return cursor_; }
    void reset() {
        const unsigned held=held_;
        *this=BattleController(initial_);
        held_=held;
    }
    bool choose_move(std::size_t slot) {
        const auto* b=party_.beast(party_.active_index());
        if (result_!=TeamBattleResult::active || !b || !b->hp || slot>=4) return false;
        const auto id=b->moves[slot].id;
        if ((id!=1 && id!=2) || !party_.consume_move(slot)) return false;
        if (id==1) {
            enemy_hp_=enemy_hp_>3 ? enemy_hp_-3 : 0;
            if (!enemy_hp_) {
                result_=TeamBattleResult::victory; xp_=20;
                menu_=BattleMenu::result; cursor_=0; return true;
            }
        }
        retaliate(id==2 ? 1 : 2); return true;
    }
    bool switch_beast(std::size_t index) {
        if (result_!=TeamBattleResult::active) return false;
        const auto outcome=party_.switch_to(index);
        if (outcome==SwitchOutcome::rejected) return false;
        if (outcome==SwitchOutcome::voluntary_turn) retaliate(2);
        else { menu_=BattleMenu::root; cursor_=0; }
        return true;
    }
    void input(unsigned held) {
        held &= 0x03FF;
        const unsigned pressed=held & ~held_;
        held_=held;
        if (pressed & 8) { reset(); return; } // Diagnostic Start reset.
        if (menu_==BattleMenu::result) return;
        if (pressed & 2) {
            if (menu_==BattleMenu::moves || menu_==BattleMenu::team) { menu_=BattleMenu::root; cursor_=0; }
            return;
        }
        const std::size_t count=menu_==BattleMenu::root ? 2 : menu_==BattleMenu::moves ? 4 : party_.size();
        if (pressed & 64) { cursor_=(cursor_+count-1)%count; return; }
        if (pressed & 128) { cursor_=(cursor_+1)%count; return; }
        if (!(pressed & 1)) return;
        if (menu_==BattleMenu::root) { menu_=cursor_==0 ? BattleMenu::moves : BattleMenu::team; cursor_=0; }
        else if (menu_==BattleMenu::moves) choose_move(cursor_);
        else switch_beast(cursor_);
    }
};
}
