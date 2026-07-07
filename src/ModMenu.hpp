#pragma once
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/utils/cocos.hpp>
#include "Features.hpp"
#include "FeatureState.hpp"

using namespace geode::prelude;

// A CCLayer-based popup, added/removed from the running scene when Tab is
// pressed (see TabListener in main.cpp). Scrolls through every entry in
// Features::FEATURE_LIST, grouped by category, each with a live toggle.

class ModMenu : public CCLayerColor {
public:
    static ModMenu* create() {
        auto ret = new ModMenu();
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool init() override {
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (!CCLayerColor::initWithColor({10, 10, 15, 235})) return false;
        this->setContentSize({420.f, winSize.height});
        this->setAnchorPoint({0.f, 0.f});
        this->setPosition({-420.f, 0.f}); // start off-screen left
        this->setID("tab-mod-menu");

        auto title = CCLabelBMFont::create("Mod Menu", "goldFont.fnt");
        title->setScale(0.7f);
        title->setPosition({210.f, winSize.height - 30.f});
        this->addChild(title);

        auto scroll = CCLayer::create();
        scroll->setID("mod-menu-scroll");
        scroll->setPosition({20.f, winSize.height - 70.f});
        this->addChild(scroll);

        float y = 0.f;
        Features::Category lastCat = static_cast<Features::Category>(-1);

        for (auto& feature : Features::FEATURE_LIST) {
            if (feature.category != lastCat) {
                lastCat = feature.category;
                auto header = CCLabelBMFont::create(Features::categoryName(feature.category), "chatFont.fnt");
                header->setAnchorPoint({0.f, 1.f});
                header->setScale(0.55f);
                header->setColor({255, 220, 100});
                header->setPosition({0.f, y});
                scroll->addChild(header);
                y -= 22.f;
            }

            auto row = buildToggleRow(feature, y);
            scroll->addChild(row);
            y -= 26.f;
        }

        scroll->setContentSize({380.f, fabsf(y)});
        // Wrap in a ScrollLayer so ~100 rows don't overflow the screen.
        auto scrollLayer = ScrollLayer::create({380.f, winSize.height - 90.f});
        scrollLayer->setPosition({20.f, 20.f});
        scrollLayer->m_contentLayer->addChild(scroll);
        scrollLayer->m_contentLayer->setContentSize(scroll->getContentSize());
        scroll->setPosition({0.f, 0.f});
        this->addChild(scrollLayer);

        return true;
    }

    CCMenu* buildToggleRow(Features::FeatureInfo const& feature, float y) {
        auto menu = CCMenu::create();
        menu->setPosition({0.f, 0.f});
        menu->setAnchorPoint({0.f, 0.f});

        auto label = CCLabelBMFont::create(feature.label.c_str(), "chatFont.fnt");
        label->setScale(0.42f);
        label->setAnchorPoint({0.f, 0.5f});
        label->setPosition({26.f, y - 8.f});
        if (feature.cheat) label->setColor({255, 140, 140});
        menu->addChild(label);

        auto onSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
        auto offSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
        onSprite->setScale(0.6f);
        offSprite->setScale(0.6f);

        std::string fid = feature.id;
        auto toggler = CCMenuItemToggler::create(
            offSprite, onSprite,
            this, menu_selector(ModMenu::onToggle)
        );
        toggler->setPosition({8.f, y - 8.f});
        toggler->toggle(FeatureState::get().isEnabled(fid));
        toggler->setUserObject(CCString::create(fid));
        menu->addChild(toggler);

        return menu;
    }

    void onToggle(CCObject* sender) {
        auto toggler = static_cast<CCMenuItemToggler*>(sender);
        auto idStr = static_cast<CCString*>(toggler->getUserObject());
        std::string id = idStr->getCString();
        bool newValue = !toggler->isToggled(); // isToggled() reflects pre-click state
        FeatureState::get().setEnabled(id, newValue);
    }

    // ---- Open / close animation ----
    void playOpenAnimation() {
        this->setVisible(true);
        this->stopAllActions();
        this->runAction(CCEaseBackOut::create(
            CCMoveTo::create(0.28f, {0.f, 0.f})
        ));
    }

    void playCloseAnimation(std::function<void()> onDone) {
        this->stopAllActions();
        auto move = CCMoveTo::create(0.22f, {-420.f, 0.f});
        auto call = CCCallFuncN::create(this, callfuncN_selector(ModMenu::onCloseFinished));
        this->runAction(CCSequence::create(CCEaseIn::create(move, 1.5f), call, nullptr));
        m_onClosed = onDone;
    }

    void onCloseFinished(CCNode*) {
        this->setVisible(false);
        if (m_onClosed) m_onClosed();
    }

private:
    std::function<void()> m_onClosed;
};
