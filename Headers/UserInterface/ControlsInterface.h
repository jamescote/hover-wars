#pragma once
#include "UserInterface/MenuInterface.h"

#define INSERT_COIN_OPTION "Insert Coin"

class ControlsInterface final : public MenuInterface
{
public:

    static ControlsInterface* getInstance(int iWidth, int iHeight);

    // Default Constructor
    ControlsInterface(int iWidth, int iHeight);
    ControlsInterface(const ControlsInterface* pCopy);                              // Default Copy Constructor
    ControlsInterface& operator=(const ControlsInterface* pCopy);                   // Assignment Operator.

    void update(float fSecondsSinceLastUpdate);

    void reinitialize(float gameTime);

    void render();

private:
    enum eUIComponent
    {
        BACKGROUND = 0,
        TITLE,
        INSERT_COIN
    };

    static ControlsInterface* m_pInstance;

    void renderOption();
};