#pragma once
#include "UserInterface/MenuInterface.h"

class LoadingInterface final : public MenuInterface
{
public:

    static LoadingInterface* getInstance(int iWidth, int iHeight);

    // Default Constructor
    LoadingInterface();
    LoadingInterface(const LoadingInterface* pCopy);                              // Default Copy Constructor
    LoadingInterface& operator=(const LoadingInterface* pCopy);                   // Assignment Operator.

    void update(float fSecondsSinceLastUpdate);

    void reinitialize(float gameTime);

    void render();

private:

    static LoadingInterface* m_pInstance;
};