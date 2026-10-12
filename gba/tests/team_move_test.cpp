#include "battle.hpp"
#include <cassert>
using namespace sigilbound;
int main() {
    Battle b;
    // Existing diagnostic path still works.
    assert(b.act(false) && b.hp == 22);

    // Team capacity and move limits.
    b.player.count = 2;
    b.player.members[0].id = "nugwolf-001";
    b.player.members[0].hp = 24;
    b.player.members[0].max_hp = 24;
    b.player.members[0].equipped = 2;
    b.player.members[0].moves[0] = {"Tackle", 35, 35, 3, false};
    b.player.members[0].moves[1] = {"Brace", 20, 20, 0, true};

    b.player.members[1].id = "beast-002";
    b.player.members[1].hp = 18;
    b.player.members[1].max_hp = 18;
    b.player.members[1].equipped = 1;
    b.player.members[1].moves[0] = {"Scratch", 30, 30, 2, false};

    assert(b.player.count == 2);
    assert(b.player.members[0].equipped == 2);
    assert(b.player.members[1].id == "beast-002");

    // Voluntary switch.
    assert(b.switch_active(1) && b.player.active == 1);
    assert(!b.switch_active(5));
    assert(!b.switch_active(-1));
    b.player.members[1].hp = 0;
    assert(!b.switch_active(1)); // zero HP blocked
    b.player.members[1].hp = 18;
    assert(b.switch_active(0) && b.player.active == 0);

    // act_move rejects invalid slots.
    assert(!b.act_move(5));
    assert(!b.act_move(-1));
    // Valid slot currently falls back to diagnostic act().
    b.reset();
    b.player.count = 1;
    b.player.members[0].equipped = 1;
    assert(b.act_move(0) && b.enemy_hp == 15);
}
