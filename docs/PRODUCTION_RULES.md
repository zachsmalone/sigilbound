# Sigilbound production rules v1

Status: design baseline and Python reference-model foundation. NOT a claim of ROM implementation or campaign completion. User direction: one-on-one beast battles; five-beast team; Lady Erin storage/summoning; top-down walking with hold-B running; learned LSM/HSM moves.

## Team and battle
Active team: one to five uniquely identified beasts. Four equipped moves per beast. Extra beasts live with Lady Erin. Zero HP incapacitates; no permanent death. Team-wide defeat returns to the last sanctuary and heals the team.
One active beast per side. Valid move, voluntary switch, battle item, or bonding attempt consumes a turn. Invalid menu input does not. Switches/items resolve before moves; moves resolve by priority then Speed; exact ties use seeded randomness. A fainted actor loses its queued action. Forced replacement grants no bonus enemy attack and begins participation next turn. Ordinary wild retreat always succeeds; story and bonded-opponent retreat is blocked. Victory XP is awarded once.
Bonding is only for eligible, conscious wild beasts. Lower HP improves chances; exact formula is unbalanced and pending. Success ends the encounter. Full teams send the new beast to Lady Erin; storage capacity is validated before spending resources. No beast release feature initially.

## Lady Erin
Sanctuary services: team/storage exchange, reorder, inspect, heal, teach/re-equip sigil moves. Exchange is atomic and preserves identity, stats, HP, moves and uses. Keep at least one usable team member when exchanging at a sanctuary. Storage capacity must be explicit in the eventual save format, never silently overwrite a beast.

## Sigil teaching
LSM = Low Sigil Move, battle teaching sigil. HSM = High Sigil Move, battle and major field teaching sigil. Both reusable, permanently owned, unsellable and undiscardable. Species-specific compatibility; teaching outside battle only. Four equipped slots; no duplicates. Full slots require replacement. Learned moves remain in history. Re-equipping never refills battle uses; only healing does. Exact compatibility lists and move-use maxima remain to be authored.

## HSM catalogue
- Water Sphere: water/seas traversal; water-damage battle role.
- Soar: travel to visited, activated landing points; air-damage role, no two-turn disappearance initially.
- Flex: push marked heavy blocks; physical-attack boost.
- Crush: destroy marked brittle barriers; physical damage.
- Slice: clear marked vegetation; physical damage.
- Lumin: illuminate dark maps; lower enemy accuracy.
Battle numbers require testing. Field actions require owned HSM, permanent story authorization, and an active-team beast with the move equipped. Field use costs no HP or battle uses. An incapacitated provider can help unless the whole team requires defeat recovery. Stored beasts cannot provide remotely.

## Exploration and safety
D-pad moves; A interacts/confirms; B held runs and cancels in menus; Start opens menus. Battle B cancels rather than selects Brace. Running has no stamina and uses normal collision/trigger checks without skipping tiles. Water Sphere has fixed travel speed.
Water Sphere starts at valid shorelines and persists across connected water maps. Landfall uses valid shore tiles. While on water, lock provider storage and removal of its equipped Water Sphere. Save provider identity and travel mode. Invalid traversal saves recover to the last sanctuary.
Soar requires an allowed outdoor departure and safe activated landing point; cannot reach unvisited story regions. Flex blocks cannot enter invalid tiles; every legal puzzle state has a reachable reset. Permanent Crush and progression Slice barriers remain removed. Lumin changes visibility, not collision; dark maps keep safe exits.
After first sanctuary activation, Sanctuary Recall is always available outside battle and returns to the last sanctuary without rewards. No story flag disables it. Every mandatory field gate has guaranteed prior access to the sigil, authorization, compatible beast, teaching opportunity and recovery route; no rare-only encounter requirement.
Save invariants: unique beast ownership; team/storage, stats, learned/equipped moves and uses; sigils, authorizations, story flags, permanent obstacle edits, Soar destinations, player position/travel mode and last sanctuary. Save/reload and atomic exchanges must be tested before campaign acceptance.

## Delivery order and acceptance
1. Sprite battle diagnostic: two visible characters, existing combat regression checks, rendered screenshot review. Previous sprite proposal remains uncommitted until its exact write is approved/executed.
2. Five-member team and switching integrated in ROM. Validate voluntary/forced switches, unavailable beasts, one-time rewards.
3. Lady Erin and persistence. Validate identity preservation, full team/storage, save/reload and team usability.
4. Top-down first map, collisions, running and controls. Test wall/trigger boundaries at both speeds.
5. Water Sphere and Recall. Test embark, map transition, landfall, provider locks, exhausted moves, defeat and reload at sea.
6. Remaining HSMs and authored progression. Test gates, safe destinations, puzzle resets and permanent edits.

Current reference model checks team limit, identity uniqueness, HP ranges, move slots/history/uses, non-refilling re-equipping and field-provider eligibility. It does NOT implement ROM team storage, switching, teaching, traversal, saving or any UI. Existing ROM builder is unchanged by this foundation commit.
