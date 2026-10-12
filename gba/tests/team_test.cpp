#include "team.hpp"
#include <cassert>
using namespace sigilbound;
BeastRecord beast(unsigned id) {
    BeastRecord b; b.id=id; b.species=1; b.hp=24; b.maximum_hp=24;
    b.moves[0]={1,2,2}; return b;
}
int main() {
    Party p;
    assert(p.size()==0 && p.defeated());
    assert(!p.damage_active(1) && !p.consume_move(0));
    auto bad=beast(1); bad.hp=0; assert(!p.add(bad));
    assert(p.add(beast(1)) && p.add(beast(2)));
    assert(!p.add(beast(1)) && p.size()==2);
    assert(p.switch_to(5)==SwitchOutcome::rejected);
    assert(p.switch_to(0)==SwitchOutcome::rejected);
    assert(p.consume_move(0) && p.beast(0)->moves[0].uses==1);
    assert(p.damage_active(7) && p.beast(0)->hp==17);
    assert(p.switch_to(1)==SwitchOutcome::voluntary_turn);
    assert(p.switch_to(0)==SwitchOutcome::voluntary_turn);
    assert(p.beast(0)->hp==17 && p.beast(0)->moves[0].uses==1);
    assert(p.consume_move(0) && !p.consume_move(0));
    assert(!p.consume_move(1) && !p.consume_move(4));
    assert(p.damage_active(65535) && p.beast(0)->hp==0);
    assert(!p.consume_move(0));
    assert(p.switch_to(1)==SwitchOutcome::forced_replacement);
    assert(p.switch_to(0)==SwitchOutcome::rejected);
    assert(p.damage_active(24) && p.defeated());
    assert(p.add(beast(3)) && p.add(beast(4)) && p.add(beast(5)));
    assert(p.size()==5 && !p.add(beast(6)) && p.size()==5);
    assert(p.switch_to(2)==SwitchOutcome::forced_replacement);
    bad=beast(7); bad.hp=25; assert(!valid_beast(bad));
    bad=beast(7); bad.moves[1]=bad.moves[0]; assert(!valid_beast(bad));
    bad=beast(7); bad.moves[0].uses=3; assert(!valid_beast(bad));
    bad=beast(7); bad.moves[1]={0,1,1}; assert(!valid_beast(bad));
    bad=beast(0); assert(!valid_beast(bad));
    assert(p.beast(5)==nullptr);
}
