#ifndef INPUTEVENT_HPP
#define INPUTEVENT_HPP

#include <string>
#include <map>

class InputEvent
{
public:
	enum InputEventKey {
		IEK_KEY_UNKNOWN,
		IEK_KEY_SPACE,
		IEK_KEY_APOSTROPHE,
		IEK_KEY_COMMA,
		IEK_KEY_MINUS,
		IEK_KEY_PERIOD,
		IEK_KEY_SLASH,
		IEK_KEY_0,
		IEK_KEY_1,
		IEK_KEY_2,
		IEK_KEY_3,
		IEK_KEY_4,
		IEK_KEY_5,
		IEK_KEY_6,
		IEK_KEY_7,
		IEK_KEY_8,
		IEK_KEY_9,
		IEK_KEY_SEMICOLON,
		IEK_KEY_EQUAL,
		IEK_KEY_A,
		IEK_KEY_B,
		IEK_KEY_C,
		IEK_KEY_D,
		IEK_KEY_E,
		IEK_KEY_F,
		IEK_KEY_G,
		IEK_KEY_H,
		IEK_KEY_I,
		IEK_KEY_J,
		IEK_KEY_K,
		IEK_KEY_L,
		IEK_KEY_M,
		IEK_KEY_N,
		IEK_KEY_O,
		IEK_KEY_P,
		IEK_KEY_Q,
		IEK_KEY_R,
		IEK_KEY_S,
		IEK_KEY_T,
		IEK_KEY_U,
		IEK_KEY_V,
		IEK_KEY_W,
		IEK_KEY_X,
		IEK_KEY_Y,
		IEK_KEY_Z,
		IEK_KEY_LEFT_BRACKET,
		IEK_KEY_BACKSLASH,
		IEK_KEY_RIGHT_BRACKET,
		IEK_KEY_GRAVE_ACCENT,
		IEK_KEY_WORLD_1,
		IEK_KEY_WORLD_2,
		IEK_KEY_ESCAPE,
		IEK_KEY_ENTER,
		IEK_KEY_TAB,
		IEK_KEY_BACKSPACE,
		IEK_KEY_INSERT,
		IEK_KEY_DELETE,
		IEK_KEY_RIGHT,
		IEK_KEY_LEFT,
		IEK_KEY_DOWN,
		IEK_KEY_UP,
		IEK_KEY_PAGE_UP,
		IEK_KEY_PAGE_DOWN,
		IEK_KEY_HOME,
		IEK_KEY_END,
		IEK_KEY_CAPS_LOCK,
		IEK_KEY_SCROLL_LOCK,
		IEK_KEY_NUM_LOCK,
		IEK_KEY_PRINT_SCREEN,
		IEK_KEY_PAUSE,
		IEK_KEY_F1,
		IEK_KEY_F2,
		IEK_KEY_F3,
		IEK_KEY_F4,
		IEK_KEY_F5,
		IEK_KEY_F6,
		IEK_KEY_F7,
		IEK_KEY_F8,
		IEK_KEY_F9,
		IEK_KEY_F10,
		IEK_KEY_F11,
		IEK_KEY_F12,
		IEK_KEY_F13,
		IEK_KEY_F14,
		IEK_KEY_F15,
		IEK_KEY_F16,
		IEK_KEY_F17,
		IEK_KEY_F18,
		IEK_KEY_F19,
		IEK_KEY_F20,
		IEK_KEY_F21,
		IEK_KEY_F22,
		IEK_KEY_F23,
		IEK_KEY_F24,
		IEK_KEY_F25,
		IEK_KEY_KP_0,
		IEK_KEY_KP_1,
		IEK_KEY_KP_2,
		IEK_KEY_KP_3,
		IEK_KEY_KP_4,
		IEK_KEY_KP_5,
		IEK_KEY_KP_6,
		IEK_KEY_KP_7,
		IEK_KEY_KP_8,
		IEK_KEY_KP_9,
		IEK_KEY_KP_DECIMAL,
		IEK_KEY_KP_DIVIDE,
		IEK_KEY_KP_MULTIPLY,
		IEK_KEY_KP_SUBTRACT,
		IEK_KEY_KP_ADD,
		IEK_KEY_KP_ENTER,
		IEK_KEY_KP_EQUAL,
		IEK_KEY_LEFT_SHIFT,
		IEK_KEY_LEFT_CONTROL,
		IEK_KEY_LEFT_ALT,
		IEK_KEY_LEFT_SUPER,
		IEK_KEY_RIGHT_SHIFT,
		IEK_KEY_RIGHT_CONTROL,
		IEK_KEY_RIGHT_ALT,
		IEK_KEY_RIGHT_SUPER,
		IEK_KEY_MENU,
		IEK_MOUSE_BUTTON_1,
		IEK_MOUSE_BUTTON_2,
		IEK_MOUSE_BUTTON_3,
		IEK_MOUSE_BUTTON_4,
		IEK_MOUSE_BUTTON_5,
		IEK_MOUSE_BUTTON_6,
		IEK_MOUSE_BUTTON_7,
		IEK_MOUSE_BUTTON_8,
		IEK_MOUSE_MOVE
	};

	enum InputEventKeyState {
		IEKS_NO_STATE,
		IEKS_PRESS,
		IEKS_RELEASE,
		IEKS_REPEAT
	};

	InputEventKey eventKey;
	InputEventKeyState eventKeyState;
	bool modifierShift;
	bool modifierControl;
	bool modifierAlt;
	bool modifierSuper;
	double xCoordinate;
	double yCoordinate;

private:
	static std::map<InputEventKey, std::string> eventKeyDescriptions;
	static std::map<InputEventKeyState, std::string> eventKeyStateDescriptions;
	static std::map<int, InputEventKey> glfwKeyTranslations;
	static std::map<int, InputEventKeyState> glfwKeyStateTranslations;

public:
	void print();
	static std::string decodeInputEventKey(InputEventKey eventKey);
	static std::string decodeInputEventKeyState(InputEventKeyState eventKeyState);
	static InputEventKey translateGlfwInputEventKey(int glfwEventKey);
	static InputEventKeyState translateGlfwInputEventKeyState(int glfwEventKeyState);

};

#endif
