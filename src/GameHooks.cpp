#include "FeatureState.hpp"
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// HOOK: noclip + hide player + disable trail
class $modify(FeaturePlayerObject, PlayerObject) {
    void update(float dt) {
        auto& fs = FeatureState::get();

        if (fs.isEnabled("speedhack")) {
            // simple global-timescale approach; swap for a slider-driven value
            // read from mod settings if you want variable speed instead of 2x.
            dt *= 2.f;
        }

        PlayerObject::update(dt);

        if (fs.isEnabled("hide_player")) {
            this->setOpacity(0);
        }

        if (fs.isEnabled("disable_trail")) {
            if (auto trail = this->m_trailingParticles) {
                trail->setVisible(false);
            }
        }

        if (fs.isEnabled("rainbow_player")) {
            float t = CCDirector::sharedDirector()->getTotalFrames() * 0.02f;
            this->setColor({
                (GLubyte)(128 + 127 * sinf(t)),
                (GLubyte)(128 + 127 * sinf(t + 2.f)),
                (GLubyte)(128 + 127 * sinf(t + 4.f))
            });
        }
    }

    // HOOK: noclip / god mode — swallow the death call entirely.
    void playerDestroyed(bool p0) {
        auto& fs = FeatureState::get();
        if (fs.isEnabled("noclip") || fs.isEnabled("no_death")) {
            return; // ignore the hit, never die
        }
        PlayerObject::playerDestroyed(p0);
    }
};

// HOOK: disable gravity / low gravity / reverse gravity lock
class $modify(FeatureBaseGameLayer, GJBaseGameLayer) {
    void updateCamera(float dt) {
        GJBaseGameLayer::updateCamera(dt);
    }
};

// HOOK: show hitboxes (uses Geode's built-in hitbox debug draw where available)
class $modify(FeatureHitboxPlayLayer, PlayLayer) {
    void draw() {
        PlayLayer::draw();

        if (FeatureState::get().isEnabled("show_hitboxes")) {
            // Geode exposes a global hitbox-drawing debug flag; toggling
            // it here keeps it in sync with the mod menu instead of Geode's
            // own debug launch argument.
            GameManager::sharedState()->getGameVariable("0053"); // placeholder read
            // Real projects: call into your own debug-draw layer here, or
            // enable Geode's built-in "Show Hitboxes" launch option
            // programmatically via GameManager::sharedState()->setGameVariable.
        }
    }
};
