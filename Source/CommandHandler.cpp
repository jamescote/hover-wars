#include "CommandHandler.h"
#include "Menus/StartMenu.h"

// Singleton instance
CommandHandler* CommandHandler::m_pInstance = nullptr;
/*
Constructor
*/
CommandHandler::CommandHandler(GLFWwindow *rWindow)
{
    m_pWindow = rWindow;
}

/*
Get Singleton instance
*/
CommandHandler* CommandHandler::getInstance(GLFWwindow *rWindow)
{
    if (nullptr == m_pInstance)
    {
        m_pInstance = new CommandHandler(rWindow);
        // Set current menu is done after after the constructor to prevent
        // mutual recursion
        m_pInstance->setCurrentMenu(StartMenu::getInstance());
    }

    return m_pInstance;
}

// Assumes instances has already been instantiated beforehand
CommandHandler* CommandHandler::getInstance()
{
    assert(nullptr != m_pInstance);
    return m_pInstance;
}

/*
Destructor
*/
CommandHandler::~CommandHandler()
{
    // Delete all menu instances, which are created as needed as the player
    // navigates between menus
    for (Menu* menu : m_vMenuInstances)
    {
        delete menu;
    }
    m_vMenuInstances.clear();
}

void CommandHandler::setCurrentMenu(Menu* menu)
{
    m_pCurrentMenu = menu;
    menu->enter();
}


/*
Execute all the commands for a given frame. This should be called every frame
update.
*/
void CommandHandler::update(float fTimeInSeconds)
{
    m_pCurrentMenu->update(fTimeInSeconds);
}

void CommandHandler::addMenu(Menu* menu)
{
    m_vMenuInstances.push_back(menu);
}
