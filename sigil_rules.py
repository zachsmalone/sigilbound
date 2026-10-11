"""Reference rules validator; not yet integrated into the Game Boy ROM."""
TEAM_LIMIT = 5
MOVE_LIMIT = 4
FIELD_MOVES = ("Water Sphere", "Soar", "Flex", "Crush", "Slice", "Lumin")

def validate_roster(team, storage):
    if not 1 <= len(team) <= TEAM_LIMIT:
        raise ValueError("Active team must contain one to five beasts")
    ids = []
    for beast in (*team, *storage):
        uid = beast["id"]
        if not isinstance(uid, str) or not uid:
            raise ValueError("Beast identity must be a nonempty string")
        ids.append(uid)
        hp, maximum = beast["hp"], beast["max_hp"]
        if type(hp) is not int or type(maximum) is not int or maximum < 1 or not 0 <= hp <= maximum:
            raise ValueError("Invalid HP")
        equipped, learned, uses = beast["equipped"], beast["learned"], beast["uses"]
        if len(equipped) > MOVE_LIMIT or len(set(equipped)) != len(equipped):
            raise ValueError("At most four distinct moves may be equipped")
        if len(set(learned)) != len(learned) or not set(equipped) <= set(learned):
            raise ValueError("Equipped moves must be learned")
        if set(uses) != set(learned) or any(type(v) is not int or v < 0 for v in uses.values()):
            raise ValueError("Every learned move must retain nonnegative uses")
    if len(ids) != len(set(ids)):
        raise ValueError("A beast cannot exist twice in team or storage")

def equip_move(beast, move, slot=None):
    if move not in beast["learned"] or move in beast["equipped"]:
        raise ValueError("Move must be learned and not already equipped")
    equipped = list(beast["equipped"])
    if slot is None:
        if len(equipped) >= MOVE_LIMIT:
            raise ValueError("Choose a move slot to replace")
        equipped.append(move)
    else:
        if type(slot) is not int or not 0 <= slot < len(equipped):
            raise ValueError("Invalid move slot")
        equipped[slot] = move
    result = {**beast, "equipped": equipped, "learned": list(beast["learned"]), "uses": dict(beast["uses"])}
    validate_roster([result], [])
    return result

def field_provider(team, move, owned_hsms, authorizations):
    if move not in FIELD_MOVES or move not in owned_hsms or move not in authorizations:
        return None
    if not any(beast["hp"] > 0 for beast in team):
        return None  # Whole-team defeat recovery takes precedence.
    return next((beast["id"] for beast in team if move in beast["equipped"]), None)
