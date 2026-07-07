#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/loader/Mod.hpp>
#include "ModMenu.hpp"
#include "FeatureState.hpp"
#include "SafeMode.hpp"

using namespace geode::prelude;

// Global-ish handle to the currently open menu, since only one can exist
// at a time regardless of which scene is active.
static ModMenu* g_menu = nullptr;
static bool g_menuOpen = false;

static void toggleModMenu() {
    auto scene = CCDirector::sharedDirector()->getRunningScene();
    if (!scene) return;

    if (!g_menu) {
        g_menu = ModMenu::create();
        g_menu->retain();
        g_menu->setVisible(false);
        scene->addChild(g_menu, 10000);
    }

    if (!g_menuOpen) {
        g_menuOpen = true;
        g_menu->playOpenAnimation();
    } else {
        g_menuOpen = false;
        g_menu->playCloseAnimation([]{});
    }
}

// Intercept the Tab key globally via CCKeyboardDispatcher, so the menu opens
// no matter what layer/scene is currently active (main menu, level, editor).
class $modify(TabListener, CCKeyboardDispatcher) {
    bool dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) {
        if (key == KEY_Tab && isKeyDown && !isKeyRepeat) {
            toggleModMenu();
            return true; // swallow the key so it doesn't tab between UI fields
        }
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat);
    }
};

$on_mod(Loaded) {
    FeatureState::get().init();
    log::info("Tab Mod Menu loaded with {} features.", Features::FEATURE_LIST.size());
}
