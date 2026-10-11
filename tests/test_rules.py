import unittest
from sigil_rules import validate_roster, equip_move, field_provider

def beast(uid="one"):
    return {"id":uid, "hp":24, "max_hp":24,
            "equipped":["Tackle"], "learned":["Tackle", "Water Sphere"],
            "uses":{"Tackle":35, "Water Sphere":0}}

class RulesTests(unittest.TestCase):
    def test_five_beasts_valid(self):
        validate_roster([beast(str(i)) for i in range(5)], [])

    def test_six_beasts_rejected(self):
        with self.assertRaises(ValueError):
            validate_roster([beast(str(i)) for i in range(6)], [])

    def test_empty_team_rejected(self):
        with self.assertRaises(ValueError):
            validate_roster([], [beast()])

    def test_duplicate_identity_rejected(self):
        with self.assertRaises(ValueError):
            validate_roster([beast()], [beast()])

    def test_invalid_hp_rejected(self):
        b = beast(); b["hp"] = 25
        with self.assertRaises(ValueError):
            validate_roster([b], [])

    def test_re_equipping_preserves_uses_and_original(self):
        original = beast()
        result = equip_move(original, "Water Sphere", 0)
        self.assertEqual(result["uses"]["Water Sphere"], 0)
        self.assertEqual(original["equipped"], ["Tackle"])
        self.assertEqual(result["equipped"], ["Water Sphere"])

    def test_unlearned_move_rejected(self):
        with self.assertRaises(ValueError):
            equip_move(beast(), "Soar")

    def test_duplicate_move_rejected(self):
        with self.assertRaises(ValueError):
            equip_move(beast(), "Tackle")

    def test_field_permission_and_equipped_move_required(self):
        b = equip_move(beast(), "Water Sphere", 0)
        self.assertIsNone(field_provider([b], "Water Sphere", set(), {"Water Sphere"}))
        self.assertIsNone(field_provider([b], "Water Sphere", {"Water Sphere"}, set()))
        self.assertIsNone(field_provider([beast()], "Water Sphere", {"Water Sphere"}, {"Water Sphere"}))
        self.assertEqual(field_provider([b], "Water Sphere", {"Water Sphere"}, {"Water Sphere"}), "one")

    def test_incapacitated_provider_allowed_but_not_whole_team_defeat(self):
        b = equip_move(beast(), "Water Sphere", 0); b["hp"] = 0
        permission = {"Water Sphere"}
        self.assertEqual(field_provider([b, beast("two")], "Water Sphere", permission, permission), "one")
        self.assertIsNone(field_provider([b], "Water Sphere", permission, permission))

    def test_missing_move_use_history_rejected(self):
        b = beast(); del b["uses"]["Water Sphere"]
        with self.assertRaises(ValueError):
            validate_roster([b], [])
