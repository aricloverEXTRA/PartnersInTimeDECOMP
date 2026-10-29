# PiT Native Runtime Plan

## Current state (`port/`)

`port/` is a **ROM patcher/launcher** (SDL2 + Dear ImGui), not a game port.
It verifies a EUR cartridge, applies mod profiles (Hard Mode, custom), and
exports generated data (banner, NitroFS tree, SDAT probe, manifest). No Nintendo
content is shipped. See `README.md`.

A prepared ROM is not expected to boot from source: the decomp is ~57% C/C++,
so ~43% of the code is still opaque binary blobs.

## Goal

A native (PC/Android) playable build of PiT, in the spirit of Harbour Masters
ports (Zelda64Recomp), while the decomp is still incomplete.

## Strategy: recomp + interpreter fallback

Because the decomp is partial, a pure from-source port is not possible yet.
Instead, use a **recomp-with-fallback** approach:

1. **Recompiled native code** for every function that is 100% byte-exact in C/C++
   (currently 893K/1.56M bytes, 57%). These run at native speed.
2. **Interpreter fallback** for the remaining binary blobs, using an ARM9/ARM7
   interpreter (e.g. based on Dynarmic or a lightweight custom core). Slower
   but correct.
3. **NitroSDK runtime** reimplemented on SDL2: graphics (2D/3D, OAM, VRAM),
   audio (SDAT via libsndfile/SDL_mixer), input (touch/keys), filesystem
   (NitroFS from the ROM), timers, interrupts (IRQ dispatch via OS_IntrMain).

## Architecture sketch

```
port/
  src/
    core/          existing patcher (keep)
    runtime/       NEW: NitroSDK shim layer
      gfx.c        2D BG/OBJ, VRAM, palettes, OAM
      audio.c      SDAT player, channels, SFX
      fs.c         NitroFS reader (from ROM)
      input.c      touch + keypad -> DS registers
      timer.c      60Hz VBlank, timers
      irq.c        OS_IntrMain dispatch (reuses decomp's .s)
    recomp/        NEW: recompiled decomp units (linked as native .o)
    main.c         runtime entry: load ROM, init subsystems, run main loop
  CMakeLists.txt   add runtime + recomp targets
```

## Phases

1. **Skeleton** (1-2 weeks): SDL2 window, VBlank loop, ROM loader, NitroFS
   reader. No graphics yet — just prove the loop runs.
2. **Graphics 2D** (2-4 weeks): BG layers, sprites (OAM), palettes, VRAM
   transfers. Enough to see the title screen.
3. **Audio** (1-2 weeks): SDAT playback, SFX, music.
4. **Recomp integration** (ongoing): link reconstructed C/C++ as native code;
   un-reconstructed functions stay in the interpreter.
5. **Input + gameplay** (2-4 weeks): touch, buttons, game logic via recomp.
6. **Polish** (ongoing): performance, save states, mod support.

## What exists today

- `port/` launcher (patcher) — fully working, CI builds Windows + Android.
- `build/pc/pit_pc.exe` — M1/M2 native PC port (links reconstructed C/C++
  plus binary blobs). Runs but is not a clean port.
- DeSmuME (in `tools/` or system) — for testing/verification only.

## What does NOT exist yet

- No recompiler, no interpreter, no runtime layer, no graphics emulation.
- No path to "native" playability without the above.

## Recommendation

Do not attempt a full port now. Instead:
1. Continue decomp (target: 80%+ C/C++ before starting runtime).
2. When ready, start with Phase 1 (skeleton) as a separate subproject under
   `port/runtime/`, keeping the launcher untouched.
3. Use the decomp's byte-exact ROM as the test oracle: the native build must
   produce identical frames/audio to the emulator.

## Legal

Same as the launcher: no Nintendo content shipped. The runtime reads the user's
own ROM. See `port/README.md` "Legal" section.