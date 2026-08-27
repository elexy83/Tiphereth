#pragma once

/**
 * @brief States namespace containing game state identifier keys.
 */
namespace States
{
    /**
     * @brief Unique enumeration identifiers for all application states.
     *
     * The ID enum is used by the Game engine's factory method to instantiate
     * and transition between different application states (menus, options, gameplay).
     */
    enum class ID
    {
        /// No active or unassigned state.
        None,

        /// The main title screen menu state.
        TitleScreen,

        /// The settings and configuration options menu state.
        Option,

        /// The fighter or character selection screen state.
        ChooseCharacter,

        /// The active gameplay session state.
        Game
    };
}