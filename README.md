# Tab Mod Menu (Geode mod)

A Geometry Dash Geode mod that opens a mod-menu overlay with **Tab**, slides in
with an easing animation, and lists **101 toggleable features** grouped into
categories (Movement, Visual, Practice/QoL, Level Info, Audio, Automation,
Cosmetic, Misc). Any feature flagged as a "cheat" automatically flips on
**Safe Mode** for the rest of the session so a modified run can't silently
submit as a legit normal-mode completion.

## What's real vs. what's a stub

I can't compile or run this here (no GD/Geode SDK or GPU in this sandbox, and
the build needs domains this environment can't reach), so I can't hand you a
tested .dll/.so — only source you build yourself (same as your Warmup mod CI).
To be upfront about scope:

- **Fully wired with real gameplay hooks:** noclip, god mode/no-fail, hide
  player, disable trail, rainbow player, speedhack (via `dt` scaling),
  Safe Mode enforcement + badge. See `src/GameHooks.cpp` and `src/SafeMode.hpp`.
- **Fully wired UI + persistent state, no gameplay effect yet:** the other
  ~90 entries in `src/Features.hpp`. Each one already: shows up in the menu,
  toggles visually, saves to disk, and is readable anywhere via
  `FeatureState::get().isEnabled("feature_id")`. They just don't *do*
  anything until you add a few lines wherever that behavior belongs
  (e.g. `custom_bg_color` belongs in a `PlayLayer::init` hook, `mute_music`
  belongs wherever `FMOD`/`AudioEngine` volume is set). I didn't fake this —
  100 genuinely distinct gameplay/UI behaviors is a multi-week feature build,
  not something to fabricate as if it already works.

Search for `FeatureState::get().isEnabled(` in `GameHooks.cpp` to see the
pattern for wiring up any of the remaining toggles.

## Build

Same flow as your other Geode project:

```bash
git clone https://github.com/geode-sdk/geode --recursive
export GEODE_SDK=$(pwd)/geode
cd TabModMenu
cmake -B build
cmake --build build
```

Or via `geode` CLI:

```bash
geode build
```

Or GitHub Actions with `geode-sdk/build-geode-mod@main` (same action you were
using for Warmup) — just point it at this repo.

## Files

```
TabModMenu/
├── mod.json              # Geode manifest
├── CMakeLists.txt
├── README.md
└── src/
    ├── Features.hpp       # the 101-entry feature registry (data-driven)
    ├── FeatureState.hpp   # persistent toggle storage + safe-mode dirty flag
    ├── SafeMode.hpp        # forces practice-mode behavior once any cheat is on
    ├── GameHooks.cpp       # real hooks for the flagship movement/visual cheats
    ├── ModMenu.hpp         # the Tab-activated scrollable menu UI + animation
    └── main.cpp            # Tab keybind + menu lifecycle
```

## Customizing

- **Add a feature:** add one line to `FEATURE_LIST` in `Features.hpp`. It
  shows up in the menu automatically — no UI code needed.
- **Change the open animation:** edit `ModMenu::playOpenAnimation()` — currently
  a `CCEaseBackOut` slide-in from off-screen left.
- **Change the keybind:** swap `KEY_Tab` in `main.cpp` for any `enumKeyCodes`.
- **Speedhack slider instead of fixed 2x:** replace the flat `dt *= 2.f` in
  `GameHooks.cpp` with a value read from a Geode mod setting (a slider setting
  type is built into `mod.json`'s `settings` block).
