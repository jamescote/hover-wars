#pragma once

 #include "GameManager.h"
// class GameManager;
/*
Receives and stores user input (mouse, keyboard and controller).
*/
class InputHandler
{
public:
	static InputHandler* getInstance(GLFWwindow *rWindow);
	~InputHandler();

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void joystickCallback(int joystickID, int event);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseMoveCallback(GLFWwindow* window, double x, double y);
	static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	void updateMouse(float fX, float fY);
	void updateJoysticks();

	// Flag the Mouse for Rotations or Translations (Cannot do both at the same time).
	void mouseTStart() { m_bTranslateFlag = !m_bRotateFlag; }
	void mouseTEnd() { m_bTranslateFlag = false; }
	void mouseRStart() { m_bRotateFlag = !m_bTranslateFlag; };
	void mouseREnd() { m_bRotateFlag = false; }

	// void mouseZoom(float fZoomVal);

	int m_keyboardPlayer;

	float x;
	float y;

	// Keyboard
	// Tracks all keys if they are pressed
	// Allows for multiple key input
	// https://stackoverflow.com/questions/46631814/handling-multiple-keys-input-at-once-with-glfw
	int pressed[KEYS];

	// Joysticks
	// List of joysticks that are present (detected) by the game.
	// Only present controllers are initialized
	// 1 - present
	// 0 - not present
	int m_pJoystickIsPresent[MAX_PLAYER_COUNT];
	int m_pJoystickAxesCount[MAX_PLAYER_COUNT];
	const float* m_pJoystickAxes[MAX_PLAYER_COUNT];
	int m_pJoystickButtonCount[MAX_PLAYER_COUNT];
	const unsigned char* m_pJoystickButtonsPressed[MAX_PLAYER_COUNT];
	const char* m_pJoystickNames[MAX_PLAYER_COUNT];

	// The only reason this needed is for mouse input, which may not actually
	// be needed for the final product. ie. this may be removed later on.
	// If GameManager::getInstance() is called in constructor, we get 3-way
	// cyclical mutual recursion.
	GameManager* m_gameManager;
private:
	// Singleton Variables
	InputHandler(GLFWwindow *rWindow);
	void checkForPresentJoysticks();
	void debugPrintJoystickAxes(int joystickID);
	void debugPrintJoystickButtons(int joystickID);
	void debugPrintJoystickInformation(int joystickID);
	void debugPrintJoystickInformation();
	void disconnectJoystick(int joystickID);
	void initializeJoystick(int joystickID);
	void initializeJoystickVariables();
	void initializeJoysticksAtStart();
	void initializeKeysPressed();

	static InputHandler* m_pInstance;


	// Mouse
	bool m_bTranslateFlag;
	bool m_bRotateFlag;
	glm::vec2 m_pInitialPos;
};

