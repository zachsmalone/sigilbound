#include "battle.hpp"
#include <cassert>
using namespace sigilbound;
int main() {
    Battle b;
    assert(b.act(false) && b.hp == 22 && b.enemy_hp == 15 && b.tackle == 34);
    b.reset(); assert(b.act(true) && b.hp == 23 && b.enemy_hp == 18 && b.brace == 19);
    b.reset(); for (int i=0; i<6; ++i) assert(b.act(false));
    assert(b.result == Result::victory && b.hp == 14 && b.enemy_hp == 0 && b.xp == 20);
    assert(!b.act(false) && b.xp == 20 && b.tackle == 29);
    b.reset(); for (int i=0; i<20; ++i) assert(b.act(true));
    assert(b.hp == 4 && b.brace == 0 && !b.act(true) && b.hp == 4);
    assert(b.act(false) && b.act(false));
    assert(b.result == Result::defeat && b.hp == 0 && b.enemy_hp == 12 && b.xp == 0);
    assert(!b.act(true));
    b.reset(); assert(b.hp == 24 && b.enemy_hp == 18 && b.tackle == 35 && b.brace == 20 && b.xp == 0 && b.result == Result::active);
}
