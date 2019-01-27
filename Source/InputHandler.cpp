/*

Receives user input (mouse, keyboard and controller) and initiates appropriate
actions that correspond to input to CommandHandler.

*/
#include <iostream> // debug
#include "stdafx.h"
#include "InputHandler.h"
#include "Mouse_Handler.h"

// Single Singleton instance
InputHandler* InputHandler::m_pInstance = nullptr;

InputHandler::InputHandler(GLFWwindow *rWindow)
{
	// Initializing Base Class
	initializeKeysPressed();
	m_pCommandHandler = CommandHandler::getInstance(rWindow);
	// Controller
	// Each controller counts as 1 joystick

	controllersPresent[0] = glfwJoystickPresent(GLFW_JOYSTICK_1);
	controllersPresent[1] = glfwJoystickPresent(GLFW_JOYSTICK_2);
	controllersPresent[2] = glfwJoystickPresent(GLFW_JOYSTICK_3);
	controllersPresent[3] = glfwJoystickPresent(GLFW_JOYSTICK_4);

	for (int i = 0; i < MAX_PLAYER_COUNT; i++)
	{
		if (controllersPresent[i])
		{
			std::cout << "Controller " << (i + 1) << " is connected" << std::endl;
		}
	}

	// Keyboard
	glfwSetKeyCallback(rWindow, InputHandler::keyCallback);
	// Mouse
	m_pMouseHandler = Mouse_Handler::getInstance(rWindow);
	glfwSetMouseButtonCallback(rWindow, InputHandler::mouseButtonCallback);
	glfwSetCursorPosCallback(rWindow, InputHandler::mouseMoveCallback);
	glfwSetScrollCallback(rWindow, InputHandler::mouseScrollCallback);
}

InputHandler* InputHandler::getInstance(GLFWwindow *rWindow)
{
	if (nullptr == m_pInstance)
	{
		m_pInstance = new InputHandler(rWindow);
	}

	return m_pInstance;
}

InputHandler::~InputHandler()
{
	m_pCommandHandler = nullptr;
	m_pMouseHandler = nullptr;
}


// handles keyboard input events
void InputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (GLFW_KEY_UNKNOWN == key)
	{
		return;
	}

	// TODO is it expensive to assign these every call back?
	InputHandler* pInputHandler = InputHandler::getInstance(window);
	pInputHandler->pressed[key] = action != GLFW_RELEASE;
	// GameManager* pGPXMngr = GameManager::getInstance(window);
	// ShaderManager* pShdrMngr = SHADER_MANAGER;
	// EntityManager* pEnvMngr = ENTITY_MANAGER;

	// Special keys handled differently than just pressed/not pressed
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		// TODO Later should escape no longer close window, as there may need
		// to be some tear down steps before the game exits?
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_F:
		if (GLFW_PRESS == action)
		{
			pInputHandler->m_pCommandHandler->executeCommand(KEYBOARD_PLAYER, CommandHandler::DEBUG_TOGGLE_WIREFRAME);
		}
		break;

	}
}

/*
Looks at all the keys that are currently pressed and sends each pressed keys'
commands to CommandHandler. This should be called every frame update.
*/
void InputHandler::handleInput()
{
	CommandHandler::Command command;
	for (int key = 0; key < KEYS; key++)
	{
		if (pressed[key])
		{
			switch (key)
			{
			case GLFW_KEY_W:
				command = CommandHandler::MOVE_FORWARD;
				break;
			case GLFW_KEY_S:
				command = CommandHandler::MOVE_BACK;
				break;
			case GLFW_KEY_A:
				command = CommandHandler::MOVE_LEFT;
				break;
			case GLFW_KEY_D:
				command = CommandHandler::MOVE_RIGHT;
				break;
			case GLFW_KEY_J:
				command = CommandHandler::TURN_LEFT;
				break;
			case GLFW_KEY_L:
				command = CommandHandler::TURN_RIGHT;
				break;
			case GLFW_KEY_K:
				command = CommandHandler::DASH_BACK;
				break;
			case GLFW_KEY_I:
				command = CommandHandler::DASH_FORWARD;
				break;
			case GLFW_KEY_H:
				command = CommandHandler::DASH_LEFT;
				break;
			case GLFW_KEY_SEMICOLON:
				command = CommandHandler::DASH_RIGHT;
				break;
			case GLFW_KEY_SPACE:
				command = CommandHandler::ABILITY_ROCKET;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				command = CommandHandler::ABILITY_TRAIL;
				break;
			case GLFW_KEY_APOSTROPHE:
				command = CommandHandler::ABILITY_SPIKES;
				break;
			case GLFW_KEY_ENTER:
				command = CommandHandler::MENU_SELECT;
				break;
			case GLFW_KEY_P:
				command = CommandHandler::MENU_PAUSE;
				// if ( iAction == GLFW_RELEASE )
					// m_pEntMngr->pause();
				break;
			default:
				command = CommandHandler::NOTHING;
			}
			if (CommandHandler::NOTHING != command) {
				m_pCommandHandler->executeCommand(KEYBOARD_PLAYER, command);
			}
		}
	}
}


// Mouse Button Callback
// Handle mouse movement controls.
void InputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Mouse_Handler* mMouseHndlr = Mouse_Handler::getInstance( window );
	double fX, fY;

	if (GLFW_MOUSE_BUTTON_1 == button)
	{
		glfwGetCursorPos(window, &fX, &fY);
		if (GLFW_PRESS == action)
		{
			mMouseHndlr->mouseTStart();
		}
		else if (GLFW_RELEASE == action)
		{
			mMouseHndlr->mouseTEnd();
		}
	}
	if (GLFW_MOUSE_BUTTON_2 == button)
	{
		glfwGetCursorPos(window, &fX, &fY);
		if (GLFW_PRESS == action)
		{
			mMouseHndlr->mouseRStart();
		}
		else if (GLFW_RELEASE == action)
		{
			mMouseHndlr->mouseREnd();
		}
	}
}

// Handles input from Mouse Moves.
void InputHandler::mouseMoveCallback(GLFWwindow* window, double x, double y)
{
	Mouse_Handler* mMouseHndlr = Mouse_Handler::getInstance(window);

	mMouseHndlr->updateMouse((float)x, (float)y);
}

// Handle scroll wheel callbacks
void InputHandler::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Mouse_Handler* pMsHndlr = Mouse_Handler::getInstance(window);

	pMsHndlr->mouseZoom((float)yoffset * 0.05f);
}

void InputHandler::initializeKeysPressed()
{
	for (int key = 0; key < KEYS; key++)
	{
		pressed[key] = false;
	}
}
