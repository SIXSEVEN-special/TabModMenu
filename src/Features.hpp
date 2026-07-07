#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// ---------------------------------------------------------------------------
// Feature registry
// ---------------------------------------------------------------------------
// This is the single source of truth for every toggle in the mod menu.
// Add a line to FEATURE_LIST and it automatically:
//   - shows up in the menu, sorted into its category
//   - gets a persistent on/off state (saved via Geode's mod settings/save-data)
//   - is checked by SafeMode::isDirty() so normal-mode score submission gets
//     blocked the moment any "cheat" feature is switched on (see ModMenu.cpp)
//
// `cheat = true`  -> flips SafeMode on the instant it's enabled.
// `cheat = false` -> purely cosmetic/QoL, never forces safe mode.
//
// Only a subset of these (noclip, speedhack, hitboxes, no-fail, hide player,
// etc.) have real gameplay hooks wired up in PlayLayer.cpp/GameHooks.cpp as
// working examples. The rest are fully functional *toggles* (state persists,
// show in the UI, can be read anywhere via Features::get(id)) but don't yet
// have gameplay code attached -- that's intentionally left for you to fill
// in, since "noclip" and "change background music" need wildly different
// implementations and there's no way to guess all 100 for you. Search for
// "// HOOK:" comments in GameHooks.cpp to see where to plug new ones in.

namespace Features {

    enum class Category {
        Movement,
        Visual,
        Practice,
        LevelInfo,
        Audio,
        Automation,
        Cosmetic,
        Misc
    };

    inline const char* categoryName(Category c) {
        switch (c) {
            case Category::Movement:   return "Movement & Physics";
            case Category::Visual:     return "Visual / ESP";
            case Category::Practice:   return "Practice & QoL";
            case Category::LevelInfo:  return "Level Info";
            case Category::Audio:      return "Audio & FX";
            case Category::Automation: return "Automation / Bot";
            case Category::Cosmetic:   return "Cosmetic / Fun";
            case Category::Misc:       return "Misc";
        }
        return "Misc";
    }

    struct FeatureInfo {
        std::string id;       // stable save-key, never rename once shipped
        std::string label;    // shown in the UI
        Category category;
        bool cheat;           // true => enabling it forces Safe Mode
        bool defaultValue = false;
    };

    // clang-format off
    inline const std::vector<FeatureInfo> FEATURE_LIST = {
        // ---- Movement & Physics (cheat) ----
        {"noclip",              "Noclip",                          Category::Movement, true},
        {"speedhack",           "Speedhack (slider 0.1x-4x)",      Category::Movement, true},
        {"no_gravity",          "Disable Gravity",                  Category::Movement, true},
        {"infinite_jump",       "Infinite Jump",                    Category::Movement, true},
        {"click_between_frames","Click Between Frames (CBF)",       Category::Movement, true},
        {"auto_jump",           "Auto Jump",                        Category::Movement, true},
        {"anti_cheat_shield",   "Ignore Anticheat Kicks",           Category::Movement, true},
        {"no_death",            "No Fail / God Mode",              Category::Movement, true},
        {"low_gravity",         "Low Gravity Mode",                 Category::Movement, true},
        {"reverse_gravity",     "Reverse Gravity Lock",             Category::Movement, true},
        {"static_camera",       "Static Camera (disable follow)",   Category::Movement, false},
        {"free_camera",         "Free Camera (WASD pan)",           Category::Movement, false},
        {"disable_rotation",    "Disable Player Rotation",          Category::Movement, true},

        // ---- Visual / ESP (mostly non-cheat) ----
        {"show_hitboxes",       "Show Hitboxes",                    Category::Visual, false},
        {"show_trajectory",     "Show Trajectory Line",             Category::Visual, false},
        {"show_hitbox_grid",    "Show Collision Grid",              Category::Visual, false},
        {"hide_player",         "Hide Player Icon",                 Category::Visual, false},
        {"hide_all_players",    "Hide Other Players (2P)",          Category::Visual, false},
        {"disable_trail",       "Disable Trail/Ship Streak",        Category::Visual, false},
        {"disable_particles",   "Disable Particles",                Category::Visual, false},
        {"disable_shake",       "Disable Camera Shake",             Category::Visual, false},
        {"disable_flash",       "Disable Screen Flash",             Category::Visual, false},
        {"show_speedchanges",   "Highlight Speed Portals",          Category::Visual, false},
        {"show_orbs",           "Highlight Orbs & Pads",            Category::Visual, false},
        {"show_portals",        "Highlight Gamemode Portals",       Category::Visual, false},
        {"labeled_triggers",    "Label Triggers On-Screen",         Category::Visual, false},
        {"show_object_ids",     "Show Object IDs on Click",         Category::Visual, false},
        {"transparent_ui",      "Transparent Pause/HUD Panels",     Category::Visual, false},
        {"custom_death_effect", "Force Custom Death Effect",        Category::Visual, false},
        {"rainbow_player",      "Rainbow Player Color Cycle",       Category::Visual, false},
        {"disable_bg_effects",  "Disable Background Pulse Effects", Category::Visual, false},
        {"high_contrast",       "High Contrast Mode",               Category::Visual, false},
        {"colorblind_mode",     "Colorblind-Friendly Palette",      Category::Visual, false},
        {"fps_uncap",           "Uncap FPS Display Overlay",        Category::Visual, false},

        // ---- Practice & QoL (non-cheat, safe utility) ----
        {"auto_checkpoint",     "Auto Checkpoint (interval)",       Category::Practice, false},
        {"instant_respawn",     "Instant Respawn (skip anim)",      Category::Practice, false},
        {"checkpoint_markers",  "Visible Checkpoint Markers",       Category::Practice, false},
        {"practice_music_fix",  "Keep Music Synced After Death",    Category::Practice, false},
        {"attempt_counter_ui",  "Custom Attempt Counter Overlay",   Category::Practice, false},
        {"restart_on_key",      "Quick Restart Hotkey",             Category::Practice, false},
        {"auto_retry",          "Auto Retry On Death",              Category::Practice, false},
        {"slow_mo_on_death",    "Slow-Motion On Death",             Category::Practice, false},
        {"skip_death_anim",     "Skip Death Animation",             Category::Practice, false},
        {"remember_last_cp",    "Remember Last Checkpoint Per Level", Category::Practice, false},
        {"practice_percent_ui", "Show Percent At Every Checkpoint", Category::Practice, false},

        // ---- Level Info ----
        {"show_fps",            "FPS Counter",                      Category::LevelInfo, false},
        {"show_ping",           "Ping/Latency Display (online)",    Category::LevelInfo, false},
        {"show_object_count",   "Object Count Overlay",             Category::LevelInfo, false},
        {"show_level_id",       "Show Level ID Overlay",            Category::LevelInfo, false},
        {"show_attempt_time",   "Attempt Timer",                    Category::LevelInfo, false},
        {"show_best_percent",   "Best % Overlay",                   Category::LevelInfo, false},
        {"show_song_info",      "Song Name/Artist Overlay",         Category::LevelInfo, false},
        {"show_creator_info",   "Level Creator Info Overlay",       Category::LevelInfo, false},
        {"show_coins_collected","Coin Collection Tracker",          Category::LevelInfo, false},
        {"show_orb_count",      "Orb Press Counter",                Category::LevelInfo, false},
        {"noclip_accuracy_meter","Input Accuracy Meter",            Category::LevelInfo, true},

        // ---- Audio & FX ----
        {"mute_sfx",            "Mute SFX",                         Category::Audio, false},
        {"mute_music",          "Mute Music",                       Category::Audio, false},
        {"custom_click_sound",  "Custom Click Sound",               Category::Audio, false},
        {"death_sound_toggle",  "Custom Death Sound",               Category::Audio, false},
        {"volume_boost",        "Boost SFX Volume",                 Category::Audio, false},
        {"low_pass_filter",     "Low-Pass Music Filter",            Category::Audio, false},
        {"metronome",           "Metronome Click Track",            Category::Audio, false},
        {"disable_menu_loop",   "Disable Menu Loop Music",          Category::Audio, false},

        // ---- Automation / Bot (cheat) ----
        {"replay_bot",           "Replay Bot (record & playback)",  Category::Automation, true},
        {"macro_recorder",       "Input Macro Recorder",           Category::Automation, true},
        {"auto_complete",        "Auto-Complete Level",            Category::Automation, true},
        {"tps_bypass",           "TPS Bypass",                     Category::Automation, true},
        {"frame_stepper",        "Frame-by-Frame Stepper",         Category::Automation, true},
        {"auto_orb_click",       "Auto Orb Clicking",              Category::Automation, true},
        {"perfect_gravity_bot",  "Perfect Gravity Portal Assist",  Category::Automation, true},

        // ---- Cosmetic / Fun (non-cheat) ----
        {"custom_bg_color",     "Custom Background Color",          Category::Cosmetic, false},
        {"custom_ground_color", "Custom Ground Color",              Category::Cosmetic, false},
        {"disable_ground_line", "Hide Ground Line",                 Category::Cosmetic, false},
        {"cursor_trail",        "Cursor Trail Effect",              Category::Cosmetic, false},
        {"menu_particles",      "Menu Background Particles",        Category::Cosmetic, false},
        {"discord_rpc",         "Discord Rich Presence",            Category::Cosmetic, false},
        {"custom_loading_text", "Custom Loading Screen Text",       Category::Cosmetic, false},
        {"april_fools_icons",   "Force Silly Icon Set",             Category::Cosmetic, false},
        {"screen_shake_boost",  "Extra Screen Shake (fun mode)",    Category::Cosmetic, false},
        {"party_mode",          "Party Mode (color flash bg)",      Category::Cosmetic, false},
        {"tiny_player",         "Tiny Player Icon",                 Category::Cosmetic, false},
        {"giant_player",        "Giant Player Icon",                Category::Cosmetic, false},
        {"custom_font",         "Custom UI Font",                  Category::Cosmetic, false},
        {"retro_ui",            "Retro 1.0 Style UI",               Category::Cosmetic, false},
        {"snow_overlay",        "Falling Snow Overlay",             Category::Cosmetic, false},

        // ---- Misc ----
        {"fps_bypass",          "Remove FPS Cap Limit",             Category::Misc, false},
        {"disable_tooltips",    "Disable UI Tooltips",              Category::Misc, false},
        {"quick_save_layout",   "Quick-Save Editor Layout",         Category::Misc, false},
        {"editor_grid_snap",    "Editor Grid Snap Toggle",          Category::Misc, false},
        {"editor_object_search","Editor Object Search Bar",         Category::Misc, false},
        {"level_backup",        "Auto Level Backup On Save",        Category::Misc, false},
        {"crash_reporter",      "Verbose Crash Reporter",           Category::Misc, false},
        {"debug_console",       "In-Game Debug Console",            Category::Misc, false},
        {"mod_update_checker",  "Auto-Check Mod Updates",           Category::Misc, false},
        {"disable_analytics",   "Disable RobTop Analytics Ping",    Category::Misc, false},
        {"skip_rate_popup",     "Skip 'Rate This Level' Popup",     Category::Misc, false},
        {"fast_level_load",     "Faster Level Load (cache)",        Category::Misc, false},
        {"clipboard_level_id",  "Copy Level ID To Clipboard",       Category::Misc, false},
        {"batch_export",        "Batch Export Level Data",          Category::Misc, false},
        {"custom_keybinds",     "Custom Menu Keybind Remap",        Category::Misc, false},
    };
    // clang-format on

} // namespace Features
