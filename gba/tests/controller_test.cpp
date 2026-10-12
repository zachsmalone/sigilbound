#include "battle_controller.hpp"
#include <cassert>
using namespace sigilbound;
Party fixture(int members=5,int hp=24) {
    Party p;
    for(int i=0;i<members;++i) {
        BeastRecord b; b.id=i+1; b.species=1; b.hp=hp; b.maximum_hp=24;
        b.moves[0]={1,35,35}; b.moves[1]={2,35,35}; b.moves[2]={99,1,1};
        assert(p.add(b));
    }
    return p;
}
void tap(BattleController& b,unsigned key) { b.input(0); b.input(key); b.input(0); }
int main() {
    BattleController b(fixture());
    assert(b.menu()==BattleMenu::root);
    b.input(1); assert(b.menu()==BattleMenu::moves);
    b.input(1); assert(b.enemy_hp()==18); // Held confirm must not attack.
    tap(b,2); assert(b.menu()==BattleMenu::root && b.party().beast(0)->hp==24);
    tap(b,1); tap(b,128); assert(b.cursor()==1);
    tap(b,1); assert(b.party().beast(0)->hp==23 && b.enemy_hp()==18);
    b.reset(); tap(b,128); tap(b,1); assert(b.menu()==BattleMenu::team);
    tap(b,128); tap(b,1);
    assert(b.party().active_index()==1 && b.party().beast(1)->hp==22);
    assert(b.party().beast(0)->hp==24 && b.party().beast(0)->moves[0].uses==35);
    assert(!b.switch_beast(1) && !b.switch_beast(5));
    b.reset(); assert(!b.choose_move(2) && !b.choose_move(3) && !b.choose_move(4));
    assert(b.enemy_hp()==18 && b.party().beast(0)->hp==24 && b.party().beast(0)->moves[2].uses==1);
    for(int i=0;i<6;++i) assert(b.choose_move(0));
    assert(b.result()==TeamBattleResult::victory && b.enemy_hp()==0 && b.xp()==20);
    assert(!b.choose_move(0) && !b.switch_beast(1) && b.xp()==20);
    b.input(8); assert(b.result()==TeamBattleResult::active && b.enemy_hp()==18 && b.xp()==0);
    b.input(8); b.input(0); assert(b.choose_move(0));
    b.input(8); assert(b.enemy_hp()==18); // New Start press resets.
    BattleController forced(fixture(2,2));
    assert(forced.choose_move(1) && forced.choose_move(1));
    assert(forced.menu()==BattleMenu::replacement && forced.result()==TeamBattleResult::active);
    tap(forced,2); assert(forced.menu()==BattleMenu::replacement);
    assert(!forced.choose_move(0) && !forced.switch_beast(0));
    assert(forced.switch_beast(1) && forced.party().beast(1)->hp==2);
    assert(forced.choose_move(1) && forced.choose_move(1));
    assert(forced.result()==TeamBattleResult::defeat && forced.xp()==0);
    assert(!forced.switch_beast(0));
    BattleController exhausted(fixture(1));
    for(int i=0;i<6;++i) assert(exhausted.choose_move(0));
    // Separate valid fixture with an exhausted equipped move.
    BeastRecord empty; empty.id=8; empty.species=1; empty.hp=24; empty.maximum_hp=24; empty.moves[0]={1,0,35};
    Party no_uses; assert(no_uses.add(empty));
    BattleController unavailable(no_uses); assert(!unavailable.choose_move(0));
    assert(unavailable.party().beast(0)->hp==24 && unavailable.enemy_hp()==18);
    BattleController no_team(Party{}); assert(no_team.result()==TeamBattleResult::defeat);
    no_team.input(64); no_team.input(128); no_team.input(1);
    assert(no_team.menu()==BattleMenu::result);
}
