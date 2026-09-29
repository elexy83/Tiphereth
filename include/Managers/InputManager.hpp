#pragma once

#include <SFML/Window.hpp>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

/**
 * @brief A single input binding: nothing, a keyboard key or a mouse button.
 *
 * Defined outside InputManager so it can safely be used in default arguments.
 */
struct InputBinding {
    enum class Type { None, Key, Mouse };

    Type type = Type::None;
    sf::Keyboard::Key key = sf::Keyboard::Unknown;
    sf::Mouse::Button mouse = sf::Mouse::Left;

    static InputBinding fromKey(sf::Keyboard::Key k) {
        InputBinding b;
        b.type = Type::Key;
        b.key = k;
        return b;
    }

    static InputBinding fromMouse(sf::Mouse::Button m) {
        InputBinding b;
        b.type = Type::Mouse;
        b.mouse = m;
        return b;
    }

    bool isBound() const { return type != Type::None; }

    bool operator==(const InputBinding& o) const {
        if (type != o.type) return false;
        if (type == Type::Key) return key == o.key;
        if (type == Type::Mouse) return mouse == o.mouse;
        return true;
    }
    bool operator!=(const InputBinding& o) const { return !(*this == o); }

    /// Real-time polling (held down).
    bool isDown() const {
        switch (type) {
        case Type::Key:   return sf::Keyboard::isKeyPressed(key);
        case Type::Mouse: return sf::Mouse::isButtonPressed(mouse);
        default:          return false;
        }
    }

    /// Event based.
    bool matches(const sf::Event& e) const {
        switch (type) {
        case Type::Key:   return e.type == sf::Event::KeyPressed && e.key.code == key;
        case Type::Mouse: return e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == mouse;
        default:          return false;
        }
    }
};

/**
 * @brief Manages rebindable inputs, grouped by gameplay mode.
 *
 * Each action has SlotCount bindings (ex: ZQSD as primary, WASD as secondary).
 * Bindings are persisted in a flat JSON file, same spirit as LocalizationManager:
 *
 *   { "gameplay1.move_up.1": "Z", "gameplay1.move_up.2": "W", "gameplay1.attack.1": "Mouse:Left" }
 *
 * Group ids and action ids must not contain '.'.
 */
class InputManager {
public:
    using Binding = InputBinding;
    static constexpr int SlotCount = 2;

    struct Action {
        std::string id;
        std::array<Binding, SlotCount> defaults;
        std::array<Binding, SlotCount> current;
    };

    struct Group {
        std::string id;
        std::vector<Action> actions;
    };

private:
    std::vector<Group> m_groups;

    Group* findGroupMutable(const std::string& id) {
        for (auto& g : m_groups) if (g.id == id) return &g;
        return nullptr;
    }

    Action* findActionMutable(const std::string& groupId, const std::string& actionId) {
        Group* g = findGroupMutable(groupId);
        if (!g) return nullptr;
        for (auto& a : g->actions) if (a.id == actionId) return &a;
        return nullptr;
    }

    static const std::vector<std::pair<sf::Keyboard::Key, std::string>>& keyNames() {
        static const auto table = [] {
            std::vector<std::pair<sf::Keyboard::Key, std::string>> t;
            for (int i = 0; i < 26; ++i)
                t.emplace_back(static_cast<sf::Keyboard::Key>(sf::Keyboard::A + i), std::string(1, static_cast<char>('A' + i)));
            for (int i = 0; i < 10; ++i) {
                t.emplace_back(static_cast<sf::Keyboard::Key>(sf::Keyboard::Num0 + i), "Num" + std::to_string(i));
                t.emplace_back(static_cast<sf::Keyboard::Key>(sf::Keyboard::Numpad0 + i), "Numpad" + std::to_string(i));
            }
            for (int i = 0; i < 12; ++i)
                t.emplace_back(static_cast<sf::Keyboard::Key>(sf::Keyboard::F1 + i), "F" + std::to_string(i + 1));
            t.insert(t.end(), {
                { sf::Keyboard::Space, "Space" },       { sf::Keyboard::Enter, "Enter" },
                { sf::Keyboard::Tab, "Tab" },           { sf::Keyboard::LShift, "LShift" },
                { sf::Keyboard::RShift, "RShift" },     { sf::Keyboard::LControl, "LControl" },
                { sf::Keyboard::RControl, "RControl" }, { sf::Keyboard::LAlt, "LAlt" },
                { sf::Keyboard::RAlt, "RAlt" },         { sf::Keyboard::Up, "Up" },
                { sf::Keyboard::Down, "Down" },         { sf::Keyboard::Left, "Left" },
                { sf::Keyboard::Right, "Right" },       { sf::Keyboard::PageUp, "PageUp" },
                { sf::Keyboard::PageDown, "PageDown" }, { sf::Keyboard::Home, "Home" },
                { sf::Keyboard::End, "End" },           { sf::Keyboard::Insert, "Insert" },
                { sf::Keyboard::Comma, "Comma" },       { sf::Keyboard::Period, "Period" },
                { sf::Keyboard::Semicolon, "Semicolon" }
                });
            return t;
            }();
        return table;
    }

    static const std::array<const char*, 5>& mouseNames() {
        static const std::array<const char*, 5> names = { "Left", "Right", "Middle", "X1", "X2" };
        return names;
    }

public:
    static std::string defaultPath() { return "assets/config/keybinds.json"; }

    // ------------------------------------------------------------------
    // Serialization helpers
    // ------------------------------------------------------------------

    /// True if this key can be written to / read from the JSON file.
    static bool hasKeyName(sf::Keyboard::Key key) {
        for (const auto& [k, name] : keyNames()) if (k == key) return true;
        return false;
    }

    static std::string toString(const Binding& b) {
        switch (b.type) {
        case Binding::Type::Key:
            for (const auto& [k, name] : keyNames()) if (k == b.key) return name;
            return "None";
        case Binding::Type::Mouse: {
            const auto idx = static_cast<std::size_t>(b.mouse);
            if (idx < mouseNames().size()) return std::string("Mouse:") + mouseNames()[idx];
            return "None";
        }
        default:
            return "None";
        }
    }

    static bool tryParse(const std::string& text, Binding& out) {
        if (text.empty() || text == "None") { out = Binding(); return true; }

        const std::string mousePrefix = "Mouse:";
        if (text.rfind(mousePrefix, 0) == 0) {
            const std::string name = text.substr(mousePrefix.size());
            const auto& names = mouseNames();
            for (std::size_t i = 0; i < names.size(); ++i) {
                if (name == names[i]) {
                    out = Binding::fromMouse(static_cast<sf::Mouse::Button>(i));
                    return true;
                }
            }
            return false;
        }

        for (const auto& [k, name] : keyNames()) {
            if (name == text) { out = Binding::fromKey(k); return true; }
        }
        return false;
    }

    // ------------------------------------------------------------------
    // Registration (call once at startup, BEFORE load())
    // ------------------------------------------------------------------

    void registerAction(const std::string& groupId, const std::string& actionId,
        Binding primary = Binding(), Binding secondary = Binding()) {
        Group* group = findGroupMutable(groupId);
        if (!group) {
            Group g;
            g.id = groupId;
            m_groups.push_back(std::move(g));
            group = &m_groups.back();
        }
        Action a;
        a.id = actionId;
        a.defaults = { primary, secondary };
        a.current = a.defaults;
        group->actions.push_back(std::move(a));
    }

    // ------------------------------------------------------------------
    // Gameplay queries
    // ------------------------------------------------------------------

    const std::vector<Group>& groups() const { return m_groups; }

    const Action* findAction(const std::string& groupId, const std::string& actionId) const {
        for (const auto& g : m_groups) {
            if (g.id != groupId) continue;
            for (const auto& a : g.actions) if (a.id == actionId) return &a;
        }
        return nullptr;
    }

    /// Real-time polling: is the action currently held (any slot)?
    bool isActionDown(const std::string& groupId, const std::string& actionId) const {
        const Action* a = findAction(groupId, actionId);
        if (!a) return false;
        for (const auto& b : a->current) if (b.isDown()) return true;
        return false;
    }

    /// Event based: does this event trigger the action (any slot)?
    bool isActionPressed(const sf::Event& event, const std::string& groupId, const std::string& actionId) const {
        const Action* a = findAction(groupId, actionId);
        if (!a) return false;
        for (const auto& b : a->current) if (b.matches(event)) return true;
        return false;
    }

    // ------------------------------------------------------------------
    // Rebinding (used by the option menu)
    // ------------------------------------------------------------------

    /**
     * @brief Assigns a binding. If another action of the same group already uses it,
     *        the two swap (the other action receives the slot's previous binding).
     */
    void setBinding(std::size_t groupIdx, std::size_t actionIdx, int slot, Binding b) {
        if (groupIdx >= m_groups.size() || slot < 0 || slot >= SlotCount) return;
        auto& actions = m_groups[groupIdx].actions;
        if (actionIdx >= actions.size()) return;

        const Binding old = actions[actionIdx].current[slot];
        if (b.isBound()) {
            for (std::size_t i = 0; i < actions.size(); ++i) {
                for (int s = 0; s < SlotCount; ++s) {
                    if (i == actionIdx && s == slot) continue;
                    if (actions[i].current[s] == b) actions[i].current[s] = old;
                }
            }
        }
        actions[actionIdx].current[slot] = b;
    }

    void resetGroup(std::size_t groupIdx) {
        if (groupIdx >= m_groups.size()) return;
        for (auto& a : m_groups[groupIdx].actions) a.current = a.defaults;
    }

    void resetAll() {
        for (std::size_t i = 0; i < m_groups.size(); ++i) resetGroup(i);
    }

    // ------------------------------------------------------------------
    // Persistence
    // ------------------------------------------------------------------

    bool load(const std::string& path = defaultPath()) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cout << "InputManager::load - Pas de fichier, touches par defaut : " << path << std::endl;
            return false;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        const std::string content = buffer.str();
        file.close();

        const std::regex pattern(R"(\"([^"]+)\"\s*:\s*\"([^"]*)\")");
        int loaded = 0;

        for (auto it = std::sregex_iterator(content.begin(), content.end(), pattern);
            it != std::sregex_iterator(); ++it) {
            const std::string key = (*it)[1].str();
            const std::string value = (*it)[2].str();

            // "group.action.slot"
            const std::size_t firstDot = key.find('.');
            const std::size_t lastDot = key.rfind('.');
            if (firstDot == std::string::npos || firstDot == lastDot) continue;

            const std::string groupId = key.substr(0, firstDot);
            const std::string actionId = key.substr(firstDot + 1, lastDot - firstDot - 1);
            const int slot = std::atoi(key.c_str() + lastDot + 1) - 1;
            if (slot < 0 || slot >= SlotCount) continue;

            Action* action = findActionMutable(groupId, actionId);
            Binding parsed;
            if (!action || !tryParse(value, parsed)) continue;

            action->current[slot] = parsed;
            ++loaded;
        }

        std::cout << "InputManager::load - " << loaded << " touches chargees" << std::endl;
        return true;
    }

    bool save(const std::string& path = defaultPath()) const {
        std::error_code ec;
        const auto parent = std::filesystem::path(path).parent_path();
        if (!parent.empty()) std::filesystem::create_directories(parent, ec);

        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "InputManager::save - Impossible d'ecrire : " << path << std::endl;
            return false;
        }

        file << "{\n";
        bool first = true;
        for (const auto& g : m_groups) {
            for (const auto& a : g.actions) {
                for (int s = 0; s < SlotCount; ++s) {
                    if (!first) file << ",\n";
                    first = false;
                    file << "  \"" << g.id << '.' << a.id << '.' << (s + 1) << "\": \""
                        << toString(a.current[s]) << '"';
                }
            }
        }
        file << "\n}\n";
        return true;
    }
};

/**
 * @brief Declares every action of every gameplay with its default keys.
 *
 * Rename the groups / actions to match your 4 gameplays. Each id needs matching
 * translation keys: "input.group.<group>" and "input.action.<action>".
 * Primary = AZERTY (ZQSD), secondary = QWERTY (WASD): both work out of the box.
 */
inline void registerDefaultBindings(InputManager& input)
{
    using B = InputBinding;
    using K = sf::Keyboard;

    const auto movement = [&input](const std::string& group) {
        input.registerAction(group, "move_up", B::fromKey(K::Z), B::fromKey(K::W));
        input.registerAction(group, "move_left", B::fromKey(K::Q), B::fromKey(K::A));
        input.registerAction(group, "move_down", B::fromKey(K::S));
        input.registerAction(group, "move_right", B::fromKey(K::D));
        };
    const auto mouseActions = [&input](const std::string& group) {
        input.registerAction(group, "primary_action", B::fromMouse(sf::Mouse::Left));
        input.registerAction(group, "secondary_action", B::fromMouse(sf::Mouse::Right));
        };

    // Gameplay 1: full set (more than 7 actions -> the list scrolls in the menu)
    movement("gameplay1");
    mouseActions("gameplay1");
    input.registerAction("gameplay1", "interact", B::fromKey(K::E));
    input.registerAction("gameplay1", "sprint", B::fromKey(K::LShift));
    input.registerAction("gameplay1", "inventory", B::fromKey(K::I), B::fromKey(K::Tab));

    // Gameplays 2 to 4
    movement("gameplay2");
    mouseActions("gameplay2");
    input.registerAction("gameplay2", "interact", B::fromKey(K::E));

    movement("gameplay3");
    mouseActions("gameplay3");

    movement("gameplay4");
    input.registerAction("gameplay4", "interact", B::fromKey(K::E));
}
