#include "Menus/PromptMenu.h"
#include "CommandHandler.h"

/*
    If the user holds down a certain movement direction, there is a buffer time
    until the prompt movement repeats.

    Unit : seconds
*/
#define PROMPT_START_REPEAT_DELAY 0.5f
/*
    As the prompt repeats, there is a delay betweeen each repeat to ensure the
    cursor does not move too quickly.

    Unit : seconds
*/
#define PROMPT_REPEAT_DELAY 0.15f

/*
    To prevent very slight joystick movement from registering as cursor movement,
    the joystick direction vector must reach a certain magnitude before it
    registers.
    The possible magnitudes are between 0.0f - 1.0f
*/
#define PROMPT_JOYSTICK_MIN_MAGNITUDE 0.75f

PromptMenu::PromptMenu(vector<vector<pair<const char*, eFixedCommand>>> vPrompts) : Menu(
    // pressedKey
    unordered_map<int, eFixedCommand>
    {
        // Movement
        {GLFW_KEY_W,     COMMAND_PROMPT_UP},
        {GLFW_KEY_UP,    COMMAND_PROMPT_UP},
        {GLFW_KEY_A,     COMMAND_PROMPT_LEFT},
        {GLFW_KEY_LEFT,  COMMAND_PROMPT_LEFT},
        {GLFW_KEY_S,     COMMAND_PROMPT_DOWN},
        {GLFW_KEY_DOWN,  COMMAND_PROMPT_DOWN},
        {GLFW_KEY_D,     COMMAND_PROMPT_RIGHT},
        {GLFW_KEY_RIGHT, COMMAND_PROMPT_RIGHT},
    },
    // justPressedKey
    unordered_map<int, eFixedCommand>
    {
        // Select
        {GLFW_KEY_SPACE,     COMMAND_PROMPT_SELECT},
        {GLFW_KEY_ENTER,     COMMAND_PROMPT_SELECT},
        {GLFW_KEY_BACKSPACE, COMMAND_PROMPT_BACK},
        {GLFW_KEY_ESCAPE,    COMMAND_PROMPT_BACK},
    },
    // justReleasedKey
    unordered_map<int, eFixedCommand>
    {
        {GLFW_KEY_W,     COMMAND_PROMPT_CURSOR_RELEASE},
        {GLFW_KEY_UP,    COMMAND_PROMPT_CURSOR_RELEASE},
        {GLFW_KEY_A,     COMMAND_PROMPT_CURSOR_RELEASE},
        {GLFW_KEY_LEFT,  COMMAND_PROMPT_CURSOR_RELEASE},
        {GLFW_KEY_S,     COMMAND_PROMPT_CURSOR_RELEASE},
        {GLFW_KEY_DOWN,  COMMAND_PROMPT_CURSOR_RELEASE},
        {GLFW_KEY_D,     COMMAND_PROMPT_CURSOR_RELEASE},
        {GLFW_KEY_RIGHT, COMMAND_PROMPT_CURSOR_RELEASE},
    },
    // repeatButton
    unordered_map<int, eFixedCommand>
    {
        {BUTTON_UP,     COMMAND_PROMPT_UP},
        {BUTTON_LEFT,   COMMAND_PROMPT_LEFT},
        {BUTTON_DOWN,   COMMAND_PROMPT_DOWN},
        {BUTTON_RIGHT,  COMMAND_PROMPT_RIGHT},
    },
    // justPressedButton
    unordered_map<int, eFixedCommand>
    {
        {BUTTON_A,      COMMAND_PROMPT_SELECT},
        {TRIGGER_RIGHT, COMMAND_PROMPT_SELECT},
        {BUTTON_START,  COMMAND_PROMPT_SELECT},
        {BUTTON_B,      COMMAND_PROMPT_BACK},
        {TRIGGER_LEFT,  COMMAND_PROMPT_BACK},
        {BUTTON_BACK,   COMMAND_PROMPT_BACK},
    },
    // justReleasedButton
    unordered_map<int, eFixedCommand>
    {
        {BUTTON_UP,     COMMAND_PROMPT_CURSOR_RELEASE},
        {BUTTON_LEFT,   COMMAND_PROMPT_CURSOR_RELEASE},
        {BUTTON_DOWN,   COMMAND_PROMPT_CURSOR_RELEASE},
        {BUTTON_RIGHT,  COMMAND_PROMPT_CURSOR_RELEASE},
    }
)
{
    m_vPrompts = vPrompts;
    m_eCursorDirection = COMMAND_INVALID_FIXED;
    m_iCurrentPromptX = 0;
    m_iCurrentPromptY = 0;

    m_fSecondsToNextRepeat = 0;
    m_fSecondsToStartRepeat = PROMPT_START_REPEAT_DELAY;
}


void PromptMenu::setupKeyCommands()
{
}

void PromptMenu::executeFixedCommand(eHovercraft hovercraft, eFixedCommand command)
{
    switch (command)
    {
    case COMMAND_PROMPT_UP:
    case COMMAND_PROMPT_LEFT:
    case COMMAND_PROMPT_DOWN:
    case COMMAND_PROMPT_RIGHT:
        moveCursor(command);
        break;
    case COMMAND_PROMPT_CURSOR_RELEASE:
        releaseCursor();
    case COMMAND_PROMPT_SELECT:
        SOUND_MANAGER->play(SoundManager::eSoundEvent::SOUND_UI_CURSOR_SELECT);
        select(getCurrentPromptCommand());
        break;
    case COMMAND_PROMPT_BACK:
        back();
        break;
    case COMMAND_CLOSE_WINDOW:
        glfwSetWindowShouldClose(COMMAND_HANDLER->m_pWindow, GL_TRUE);
        break;
    }
}

void PromptMenu::moveCursor(eFixedCommand direction)
{
    if (m_fSecondsToNextRepeat > 0.0f) {
        return;
    }
    m_fSecondsToNextRepeat = PROMPT_REPEAT_DELAY;
    int columns;
    int rows;
    int beforeX = m_iCurrentPromptX;
    int beforeY = m_iCurrentPromptY;
    switch (direction)
    {
    case COMMAND_PROMPT_UP:
        columns = m_vPrompts.at(m_iCurrentPromptX).size();
        // prevent negative modulo
        m_iCurrentPromptY = (((m_iCurrentPromptY - 1) % columns) + columns) % columns; 
        break;
    case COMMAND_PROMPT_LEFT:
        rows = m_vPrompts.size();
        // prevent negative modulo
        m_iCurrentPromptX = (((m_iCurrentPromptX - 1) % rows) + rows) % rows;
        break;
    case COMMAND_PROMPT_DOWN:
        columns = m_vPrompts.at(m_iCurrentPromptX).size();
        m_iCurrentPromptY = (m_iCurrentPromptY + 1) % columns;
        break;
    case COMMAND_PROMPT_RIGHT:
        rows = m_vPrompts.size();
        m_iCurrentPromptX = (m_iCurrentPromptX + 1) % rows;
        break;
    default:
        return; // end early as it was not a cursor movement command
    }
    if ((beforeX != m_iCurrentPromptX) || (beforeY != m_iCurrentPromptY))
    {
        // We have modulo checks to keep the cursor looping within valid options
        // However, if it loops through a dimension with only 1 option, the cursor
        // hasn't actually changed in practice. As a result, we do not want to play
        // a cursor move sound to signify the cursor has not moved.
        SOUND_MANAGER->play(SoundManager::eSoundEvent::SOUND_UI_CURSOR_MOVE);
    }
    // debug
    cout << "Cursor moved to (" << m_iCurrentPromptX << ", " << m_iCurrentPromptY << ") " << endl;
    cout << "\t" << getCurrentPrompt() << " : " << getCurrentPromptCommand() << endl;
}

/*
The the user has just released cursor movement, which resets the start repeat
delay.
*/
void PromptMenu::releaseCursor()
{
    m_fSecondsToStartRepeat = PROMPT_START_REPEAT_DELAY;
    m_fSecondsToNextRepeat = 0.0f;
    m_eCursorDirection = COMMAND_INVALID_FIXED;
}

void PromptMenu::updateTimeValues(float fTimeInSeconds)
{
    m_fSecondsToStartRepeat -= fTimeInSeconds;
    if (m_fSecondsToStartRepeat <= 0.0f)
    {
        m_fSecondsToNextRepeat -= fTimeInSeconds;
    }
}

void PromptMenu::handleAccumulatedKeyCommands(eHovercraft hovercraft, eFixedCommand command)
{
}

void PromptMenu::executeAccumulatedKeyCommands(eHovercraft hovercraft, eFixedCommand command)
{
}

void PromptMenu::updateLeftStick(eHovercraft hovercraft, float x, float y)
{
    executeFixedCommand(hovercraft, joystickStateToPromptDirection(x, y));
}

void PromptMenu::updateRightStick(eHovercraft hovercraft, float x, float y)
{
    executeFixedCommand(hovercraft, joystickStateToPromptDirection(x, y));
}

/*
Joysticks can be used to nagivate the menu. Since joystick values are not
discrete in their direction like keys or buttons are, we need to be able to
find out what direction the joysticks are headed towards.

@param x    x-coordinate of joystick
@param y    y-coordinate of joystick
@return the prompt direction the joystick is moving in.
        If th magnitude of the joystick is at least
        PROMPT_JOYSTICK_MIN_MAGNITUDE, the direction can be:
            COMMAND_PROMPT_RIGHT
            COMMAND_PROMPT_LEFT
            COMMAND_PROMPT_UP
            COMMAND_PROMPT_DOWN
        Otherwise, the direction will be:
            COMMAND_INVALID_FIXED
        and will not register as a direction.

*/
eFixedCommand PromptMenu::joystickStateToPromptDirection(float x, float y)
{
    if (FuncUtils::getMagnitude(x, y) < PROMPT_JOYSTICK_MIN_MAGNITUDE)
    {
        releaseCursor();
        return COMMAND_INVALID_FIXED;
    }
    if (abs(x) > abs(y)) // x takes priority
    {
        return x > 0 ? COMMAND_PROMPT_RIGHT : COMMAND_PROMPT_LEFT;
    }
    else // y takes priority
    {
        return y > 0 ? COMMAND_PROMPT_UP : COMMAND_PROMPT_DOWN;
    }

}
