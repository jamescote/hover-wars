#pragma once

/*
There are 8 possible hovercrafts.
0-3 correspond to players
4-7 correspond to bots
*/
enum eHovercraft
{
    HOVERCRAFT_PLAYER_1 = 0,
    HOVERCRAFT_PLAYER_2 = 1,
    HOVERCRAFT_PLAYER_3 = 2,
    HOVERCRAFT_PLAYER_4 = 3,
    HOVERCRAFT_BOT_1,
    HOVERCRAFT_BOT_2,
    HOVERCRAFT_BOT_3,
    HOVERCRAFT_BOT_4,
    MAX_HOVERCRAFT_INDEX = HOVERCRAFT_BOT_4,
    HOVERCRAFT_INVALID,
};
