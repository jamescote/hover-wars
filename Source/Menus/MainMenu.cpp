#include "Menus/MainMenu.h"
#include "Menus/MainMenu.h"
#include "CommandHandler.h"

// Singleton instance
MainMenu* MainMenu::m_pInstance = nullptr;

MainMenu::MainMenu() : PromptMenu(
    vector < vector<pair<const char*, eFixedCommand>> >
    {
        { {"New Game", eFixedCommand::COMMAND_PROMPT_NEXT_MENU},  },
    }
)
{

}

Menu* MainMenu::getInstance()
{
    if (nullptr == m_pInstance) {
        m_pInstance = new MainMenu();
    }
    return m_pInstance;
}

void MainMenu::select(eFixedCommand command)
{
    switch (command)
    {
    case COMMAND_PROMPT_NEXT_MENU:
        nextMenu(MainMenu::getInstance());
        break;
    }
}
