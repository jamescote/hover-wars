#include "UserInterface/EndgameInterface.h"
#include "GameManager.h"
#include "Menus/EndgameMenu.h"

// Singleton instance
EndgameInterface* EndgameInterface::m_pInstance = nullptr;

EndgameInterface::EndgameInterface() : PromptInterface(
    // Scaling
    vector<pair<float, float>>
    {
        // 0 Background
        {0.0f, 0.0f},
        // 1 Version
        {0.27f, 0.6f},
    },
    // Translating
    vector<pair<float, float>>
    {
        // 0 Background
        {0.0f, 0.0f},
        // 1 Version
        {0.0f, 0.0f},
    }
)
{
    GAME_MANAGER->addInterface(this);
}

EndgameInterface* EndgameInterface::getInstance(int iWidth, int iHeight)
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new EndgameInterface();
    }
    m_pInstance->updateWidthAndHeight(iWidth, iHeight);
    return m_pInstance;
}


void EndgameInterface::updateOverride(float fSecondsSinceLastUpdate)
{
}

void EndgameInterface::reinitialize(float gameTime)
{
}

void EndgameInterface::renderOverride()
{
    renderBackgroundImage(IMAGE_BACKGROUND_MAIN_MENU);
    renderImage(IMAGE_GAME_OVER, VERSION);
}

