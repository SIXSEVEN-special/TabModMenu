#pragma once
#include "FeatureState.hpp"
#include <Geode/modify/PlayLayer.hpp>

// Automatic Safe Mode: the instant any cheat-flagged feature is toggled on,
// FeatureState::isDirty() becomes true for the rest of the session. Every
// PlayLayer that starts after that point is force-flipped into practice/safe
// behavior so a run can never silently submit as a legitimate normal-mode
// completion. This mirrors how GD mod menus like Mega Hack / Ranger's Cheats
// prevent cheated runs from posting real leaderboard scores.

class $modify(SafeModePlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (FeatureState::get().isDirty()) {
            // Force practice-mode style bookkeeping so death doesn't
            // submit progress/attempts as a legit run.
            m_isPracticeMode = true;

            if (auto label = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("safe-mode-badge"))) {
                label->setVisible(true);
            } else {
                auto badge = CCLabelBMFont::create("SAFE MODE", "bigFont.fnt");
                badge->setID("safe-mode-badge");
                badge->setScale(0.35f);
                badge->setColor({255, 90, 90});
                badge->setPosition({CCDirector::sharedDirector()->getWinSize().width - 60.f, 20.f});
                badge->setZOrder(1000);
                this->addChild(badge);
            }
        }

        return true;
    }

    // Block normal-mode score/progress submission while any cheat is dirty.
    void levelComplete() {
        if (FeatureState::get().isDirty()) {
            // Still let the player see the completion locally, just make sure
            // it doesn't behave like a legitimate first-time completion.
            m_isPracticeMode = true;
        }
        PlayLayer::levelComplete();
    }
};
