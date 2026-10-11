from pathlib import Path
import hashlib, json, os
from importlib.metadata import version

ROM = Path("output/Sigilbound_Battle_Baseline.gb")
OUT = Path("output/emulator")
ADDRESSES = {"hp":0xC118, "enemy":0xC300, "tackle":0xC12D,
             "brace":0xC12E, "xp":0xC114, "result":0xC001}
INITIAL = {"hp":24, "enemy":18, "tackle":35, "brace":20, "xp":0, "result":0}

def state(emulator):
    return {key:emulator.memory[address] for key,address in ADDRESSES.items()}

def expect(emulator, expected, label, report):
    actual = state(emulator)
    if actual != expected:
        raise AssertionError(f"{label}: expected {expected}, got {actual}")
    report["checks"].append({"name":label, "passed":True, "state":actual})

def start(report):
    from pyboy import PyBoy
    emulator = PyBoy(str(ROM), window="null", sound_emulated=False, cgb=False)
    emulator.set_emulation_speed(0)
    try:
        for _ in range(600):
            if not emulator.tick(1, True, False):
                raise RuntimeError("Emulator stopped during boot")
            header = bytes(emulator.memory[0xC100:0xC106])
            title = bytes(emulator.memory[0x9800:0x9810])
            if header == b"SGBP\x01\x01" and title == b"SIGILBOUND BATTL" and state(emulator) == INITIAL:
                emulator.tick(30, True, False)
                expect(emulator, INITIAL, "boot and native party initialization", report)
                return emulator
        raise AssertionError("ROM did not reach initialized battle within 600 frames")
    except Exception:
        try: emulator.screen.image.save(OUT / "boot_failure.png")
        finally: emulator.stop(False)
        raise

def press(emulator, button):
    emulator.button_release("a")
    emulator.button_release("b")
    emulator.tick(12, True, False)
    emulator.button_press(button)
    emulator.tick(30, True, False)
    emulator.button_release(button)
    emulator.tick(12, True, False)

def run():
    OUT.mkdir(parents=True, exist_ok=True)
    report = {"commit":os.environ.get("GITHUB_SHA"), "checks":[], "passed":False,
              "emulator":"PyBoy", "delta_tested":False,
              "note":"Headless emulator test; screenshots require visual review. Not campaign acceptance."}
    emulator = None
    try:
        report["rom_sha256"] = hashlib.sha256(ROM.read_bytes()).hexdigest()
        report["emulator_version"] = version("pyboy")
        emulator = start(report)
        emulator.screen.image.save(OUT / "startup.png")
        press(emulator, "a")
        attacked = {"hp":22, "enemy":15, "tackle":34, "brace":20, "xp":0, "result":0}
        expect(emulator, attacked, "single attack", report)
        emulator.button_press("a")
        emulator.tick(30, True, False)
        once = state(emulator)
        emulator.tick(120, True, False)
        if state(emulator) != once:
            raise AssertionError("Held A repeated a turn")
        report["checks"].append({"name":"held-button debounce", "passed":True})
        emulator.button_release("a")
        emulator.tick(12, True, False)
        emulator.stop(False)
        emulator = start(report)
        press(emulator, "b")
        expect(emulator, {"hp":23,"enemy":18,"tackle":35,"brace":19,"xp":0,"result":0}, "brace", report)
        emulator.stop(False)
        emulator = start(report)
        for _ in range(6): press(emulator, "a")
        expect(emulator, {"hp":14,"enemy":0,"tackle":29,"brace":20,"xp":20,"result":1}, "victory and reward", report)
        if bytes(emulator.memory[0x99C0:0x99C7]) != b"VICTORY":
            raise AssertionError("Victory text missing")
        emulator.screen.image.save(OUT / "victory.png")
        press(emulator, "a")
        expect(emulator, INITIAL, "fixture restart", report)
        emulator.stop(False)
        emulator = start(report)
        for _ in range(20): press(emulator, "b")
        exhausted = {"hp":4,"enemy":18,"tackle":35,"brace":0,"xp":0,"result":0}
        expect(emulator, exhausted, "brace exhaustion", report)
        press(emulator, "b")
        expect(emulator, exhausted, "unavailable move does not advance turn", report)
        press(emulator, "a")
        press(emulator, "a")
        expect(emulator, {"hp":0,"enemy":12,"tackle":33,"brace":0,"xp":0,"result":2}, "defeat without reward", report)
        if bytes(emulator.memory[0x99C0:0x99C6]) != b"DEFEAT":
            raise AssertionError("Defeat text missing")
        emulator.screen.image.save(OUT / "defeat.png")
        report["passed"] = True
    except Exception as error:
        report["error"] = f"{type(error).__name__}: {error}"
        if emulator is not None:
            try: emulator.screen.image.save(OUT / "failure.png")
            except Exception as screenshot_error:
                report["screenshot_error"] = str(screenshot_error)
        raise
    finally:
        try:
            if emulator is not None: emulator.stop(False)
        finally:
            (OUT / "report.json").write_text(json.dumps(report, indent=2))

if __name__ == "__main__": run()
