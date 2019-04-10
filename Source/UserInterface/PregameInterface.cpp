#include "UserInterface/PregameInterface.h"
#include "GameManager.h"
#include "Menus/PregameMenu.h"

// Singleton instance
PregameInterface* PregameInterface::m_pInstance = nullptr;

PregameInterface::PregameInterface() : MenuInterface(
    // Scaling
    vector<pair<float, float>>
    {
        // 0 Background
        {0.0f, 0.0f},
        // 1 Player count
        {0.2f, 0.7f},
        // 2 Player count number
        {0.6f, 0.7f},
        // 3 Bot count
        {0.2f, 0.55f},
        // 4 Bot count number
        {0.6f, 0.55f},
        // 5 Game time
        {0.2f, 0.40f},
        // 6 Game time number
        {0.6f, 0.40f},
        // 3 Main menu
        {0.2f, 0.25f},
        // 4 Start game
        {0.5f, 0.25f},
    },
    // Translating
    vector<pair<float, float>>
    {
        // 0 Background
        {0.0f, 0.0f},
        // 1 Player count
        {0.0f, 0.0f},
        // 2 Bot count
        {0.0f, 0.0f},
        // 3 Main menu
        {0.0f, 0.0f},
        // 4 Start game
        {0.0f, 0.0f},
        // 5 Player count number
        {0.0f, 0.0f},
        // 6 Bot count number
        {0.0f, 0.0f},
        // 7 Game time
        {0.0f, 0.0f},
        // 8 Game time number
        {0.0f, 0.0f},
    }
)
{
    GAME_MANAGER->addInterface(this);
}

PregameInterface* PregameInterface::getInstance(int iWidth, int iHeight)
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new PregameInterface();
    }
    m_pInstance->updateWidthAndHeight(iWidth, iHeight);
    return m_pInstance;
}


void PregameInterface::updateOverride(float fSecondsSinceLastUpdate)
{
}

void PregameInterface::reinitialize(float gameTime)
{
}

void PregameInterface::renderOverride()
{
    renderImage(IMAGE_BACKGROUND_PRE_MENU, m_vComponentCoordinates[BACKGROUND].first, m_vComponentCoordinates[BACKGROUND].second, 1.0f);
    renderOption();
}

void PregameInterface::renderNumberOfPlayer(int i_NumOfPlayer) {
    // get the player count and render number
    switch(i_NumOfPlayer) {
        case 1:
            renderImage(IMAGE_1, m_vComponentCoordinates[PLAYER_COUNT_NUMBER].first, m_vComponentCoordinates[PLAYER_COUNT_NUMBER].second, 1.0f);
            break;
        case 2:
            renderImage(IMAGE_2, m_vComponentCoordinates[PLAYER_COUNT_NUMBER].first, m_vComponentCoordinates[PLAYER_COUNT_NUMBER].second, 1.0f);
            break;
        case 3:
            renderImage(IMAGE_3, m_vComponentCoordinates[PLAYER_COUNT_NUMBER].first, m_vComponentCoordinates[PLAYER_COUNT_NUMBER].second, 1.0f);
            break;
        case 4:
            renderImage(IMAGE_4, m_vComponentCoordinates[PLAYER_COUNT_NUMBER].first, m_vComponentCoordinates[PLAYER_COUNT_NUMBER].second, 1.0f);
            break;
    }
}

void PregameInterface::renderNumberOfBot(int i_NumOfBot) {
    // get the bot count and render number
    switch(i_NumOfBot) {
        case 0:
            renderImage(IMAGE_0, m_vComponentCoordinates[BOT_COUNT_NUMBER].first, m_vComponentCoordinates[BOT_COUNT_NUMBER].second, 1.0f);
            break;
        case 1:
            renderImage(IMAGE_1, m_vComponentCoordinates[BOT_COUNT_NUMBER].first, m_vComponentCoordinates[BOT_COUNT_NUMBER].second, 1.0f);
            break;
        case 2:
            renderImage(IMAGE_2, m_vComponentCoordinates[BOT_COUNT_NUMBER].first, m_vComponentCoordinates[BOT_COUNT_NUMBER].second, 1.0f);
            break;
        case 3:
            renderImage(IMAGE_3, m_vComponentCoordinates[BOT_COUNT_NUMBER].first, m_vComponentCoordinates[BOT_COUNT_NUMBER].second, 1.0f);
            break;
        case 4:
            renderImage(IMAGE_4, m_vComponentCoordinates[BOT_COUNT_NUMBER].first, m_vComponentCoordinates[BOT_COUNT_NUMBER].second, 1.0f);
            break;
    }
}

void PregameInterface::renderGameTime(int i_GameTime) {
    // get the player count and render number
    switch(i_GameTime) {
        case 60:
            renderImage(IMAGE_1_MIN, m_vComponentCoordinates[GAME_TIME_NUMBER].first, m_vComponentCoordinates[GAME_TIME_NUMBER].second, 1.0f);
            break;
        case 120:
            renderImage(IMAGE_2_MIN, m_vComponentCoordinates[GAME_TIME_NUMBER].first, m_vComponentCoordinates[GAME_TIME_NUMBER].second, 1.0f);
            break;
        case 180:
            renderImage(IMAGE_3_MIN, m_vComponentCoordinates[GAME_TIME_NUMBER].first, m_vComponentCoordinates[GAME_TIME_NUMBER].second, 1.0f);
            break;
        case 240:
            renderImage(IMAGE_4_MIN, m_vComponentCoordinates[GAME_TIME_NUMBER].first, m_vComponentCoordinates[GAME_TIME_NUMBER].second, 1.0f);
            break;
        case 300:
            renderImage(IMAGE_5_MIN, m_vComponentCoordinates[GAME_TIME_NUMBER].first, m_vComponentCoordinates[GAME_TIME_NUMBER].second, 1.0f);
            break;
        case 360:
            renderImage(IMAGE_6_MIN, m_vComponentCoordinates[GAME_TIME_NUMBER].first, m_vComponentCoordinates[GAME_TIME_NUMBER].second, 1.0f);
            break;
        case 420:
            renderImage(IMAGE_7_MIN, m_vComponentCoordinates[GAME_TIME_NUMBER].first, m_vComponentCoordinates[GAME_TIME_NUMBER].second, 1.0f);
            break;
        case 480:
            renderImage(IMAGE_8_MIN, m_vComponentCoordinates[GAME_TIME_NUMBER].first, m_vComponentCoordinates[GAME_TIME_NUMBER].second, 1.0f);
            break;
        case 540:
            renderImage(IMAGE_9_MIN, m_vComponentCoordinates[GAME_TIME_NUMBER].first, m_vComponentCoordinates[GAME_TIME_NUMBER].second, 1.0f);
            break;
        case 600:
            renderImage(IMAGE_10_MIN, m_vComponentCoordinates[GAME_TIME_NUMBER].first, m_vComponentCoordinates[GAME_TIME_NUMBER].second, 1.0f);
            break;
        default:
            renderImage(IMAGE_1_MIN, m_vComponentCoordinates[GAME_TIME_NUMBER].first, m_vComponentCoordinates[GAME_TIME_NUMBER].second, 1.0f);
            break;
    }
}

void PregameInterface::renderOption() {
    PregameMenu* m = (PregameMenu*)PregameMenu::getInstance();
    string option = m->getCurrentPrompt();
    renderNumberOfPlayer(m->getPlayerCount());
    renderNumberOfBot(m->getBotCount());
    renderGameTime(m->getGameTime());

    if (option == PLAY_COUNT_OPTION) {
        renderImage(IMAGE_NUMBER_OF_PLAYER_2, m_vComponentCoordinates[PLAYER_COUNT].first, m_vComponentCoordinates[PLAYER_COUNT].second, 1.0f);
        renderImage(IMAGE_NUMBER_OF_BOT_1, m_vComponentCoordinates[BOT_COUNT].first, m_vComponentCoordinates[BOT_COUNT].second, 1.0f);
        renderImage(IMAGE_GAME_TIME_1, m_vComponentCoordinates[GAME_TIME].first, m_vComponentCoordinates[GAME_TIME].second, 1.0f);
        renderImage(IMAGE_MAIN_MENU_BUTTON_1, m_vComponentCoordinates[MAIN_MENU].first, m_vComponentCoordinates[MAIN_MENU].second, 1.0f);
        renderImage(IMAGE_START_1, m_vComponentCoordinates[START_GAME].first, m_vComponentCoordinates[START_GAME].second, 1.0f);
    }
    else if (option == BOT_COUNT_OPTION) {
        renderImage(IMAGE_NUMBER_OF_PLAYER_1, m_vComponentCoordinates[PLAYER_COUNT].first, m_vComponentCoordinates[PLAYER_COUNT].second, 1.0f);
        renderImage(IMAGE_NUMBER_OF_BOT_2, m_vComponentCoordinates[BOT_COUNT].first, m_vComponentCoordinates[BOT_COUNT].second, 1.0f);
        renderImage(IMAGE_GAME_TIME_1, m_vComponentCoordinates[GAME_TIME].first, m_vComponentCoordinates[GAME_TIME].second, 1.0f);
        renderImage(IMAGE_MAIN_MENU_BUTTON_1, m_vComponentCoordinates[MAIN_MENU].first, m_vComponentCoordinates[MAIN_MENU].second, 1.0f);
        renderImage(IMAGE_START_1, m_vComponentCoordinates[START_GAME].first, m_vComponentCoordinates[START_GAME].second, 1.0f);
    }
    else if (option == GAME_TIME_OPTION) {
        renderImage(IMAGE_NUMBER_OF_PLAYER_1, m_vComponentCoordinates[PLAYER_COUNT].first, m_vComponentCoordinates[PLAYER_COUNT].second, 1.0f);
        renderImage(IMAGE_NUMBER_OF_BOT_1, m_vComponentCoordinates[BOT_COUNT].first, m_vComponentCoordinates[BOT_COUNT].second, 1.0f);
        renderImage(IMAGE_GAME_TIME_2, m_vComponentCoordinates[GAME_TIME].first, m_vComponentCoordinates[GAME_TIME].second, 1.0f);
        renderImage(IMAGE_MAIN_MENU_BUTTON_1, m_vComponentCoordinates[MAIN_MENU].first, m_vComponentCoordinates[MAIN_MENU].second, 1.0f);
        renderImage(IMAGE_START_1, m_vComponentCoordinates[START_GAME].first, m_vComponentCoordinates[START_GAME].second, 1.0f);
    }
    else if (option == START_GAME_OPTION) {
        renderImage(IMAGE_NUMBER_OF_PLAYER_1, m_vComponentCoordinates[PLAYER_COUNT].first, m_vComponentCoordinates[PLAYER_COUNT].second, 1.0f);
        renderImage(IMAGE_NUMBER_OF_BOT_1, m_vComponentCoordinates[BOT_COUNT].first, m_vComponentCoordinates[BOT_COUNT].second, 1.0f);
        renderImage(IMAGE_GAME_TIME_1, m_vComponentCoordinates[GAME_TIME].first, m_vComponentCoordinates[GAME_TIME].second, 1.0f);
        renderImage(IMAGE_MAIN_MENU_BUTTON_1, m_vComponentCoordinates[MAIN_MENU].first, m_vComponentCoordinates[MAIN_MENU].second, 1.0f);
        renderImage(IMAGE_START_2, m_vComponentCoordinates[START_GAME].first, m_vComponentCoordinates[START_GAME].second, 1.0f);
    }
    else if (option == BACK_OPTION) {
        renderImage(IMAGE_NUMBER_OF_PLAYER_1, m_vComponentCoordinates[PLAYER_COUNT].first, m_vComponentCoordinates[PLAYER_COUNT].second, 1.0f);
        renderImage(IMAGE_NUMBER_OF_BOT_1, m_vComponentCoordinates[BOT_COUNT].first, m_vComponentCoordinates[BOT_COUNT].second, 1.0f);
        renderImage(IMAGE_GAME_TIME_1, m_vComponentCoordinates[GAME_TIME].first, m_vComponentCoordinates[GAME_TIME].second, 1.0f);
        renderImage(IMAGE_MAIN_MENU_BUTTON_2, m_vComponentCoordinates[MAIN_MENU].first, m_vComponentCoordinates[MAIN_MENU].second, 1.0f);
        renderImage(IMAGE_START_1, m_vComponentCoordinates[START_GAME].first, m_vComponentCoordinates[START_GAME].second, 1.0f);
    }
}
