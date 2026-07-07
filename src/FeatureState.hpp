#pragma once
#include "Features.hpp"
#include <Geode/loader/Mod.hpp>
#include <unordered_map>
#include <string>

// Central runtime state for every toggle, backed by Geode's save container
// so it persists across game restarts. Also tracks whether *any* cheat-flagged
// feature has ever been switched on this session, which SafeMode.hpp reads to
// decide whether to allow normal-mode progress/score submission.

class FeatureState {
public:
    static FeatureState& get() {
        static FeatureState instance;
        return instance;
    }

    void init() {
        for (auto& f : Features::FEATURE_LIST) {
            bool saved = Mod::get()->getSavedValue<bool>(f.id, f.defaultValue);
            m_state[f.id] = saved;
            if (saved && f.cheat) m_dirty = true;
        }
    }

    bool isEnabled(const std::string& id) const {
        auto it = m_state.find(id);
        return it != m_state.end() ? it->second : false;
    }

    void setEnabled(const std::string& id, bool value) {
        m_state[id] = value;
        Mod::get()->setSavedValue(id, value);

        for (auto& f : Features::FEATURE_LIST) {
            if (f.id == id && f.cheat && value) {
                m_dirty = true; // one-way switch for the session; see SafeMode.hpp
            }
        }
    }

    // True once any cheat feature has been enabled this session.
    bool isDirty() const { return m_dirty; }

private:
    std::unordered_map<std::string, bool> m_state;
    bool m_dirty = false;
};
