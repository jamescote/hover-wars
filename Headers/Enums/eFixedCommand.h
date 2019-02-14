#pragma once

/*
Represents a fixed command. Their actions are binary (they either
are executed or not), and do not require any extra parameters.
*/
enum eFixedCommand
{
    COMMAND_ABILITY_ROCKET,
    COMMAND_ABILITY_SPIKES,
    COMMAND_ABILITY_TRAIL,
    COMMAND_CAMERA_CHANGE,
    COMMAND_DASH_BACK,
    COMMAND_DASH_FORWARD,
    COMMAND_DASH_LEFT,
    COMMAND_DASH_RIGHT,
    COMMAND_DEBUG_TOGGLE_WIREFRAME,
    COMMAND_MENU_BACK,
    COMMAND_MENU_PAUSE,
    COMMAND_MENU_START,
    COMMAND_INVALID_FIXED,
};