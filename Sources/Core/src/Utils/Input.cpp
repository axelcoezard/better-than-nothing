#include "../BetterThanNothing.hpp"

namespace BetterThanNothing
{

	int Input::m_Keys[KEYBOARD_KEY_COUNT] = {};

	float64 Input::m_MouseX = 0;
	float64 Input::m_MouseY = 0;
	float64 Input::m_LastMouseX = 0;
	float64 Input::m_LastMouseY = 0;

	float64 Input::m_MouseScroll = 0;
	int Input::m_Buttons[MOUSE_BUTTON_COUNT] = {};

	Input::Input()
	{
		for (int & m_Key : m_Keys) {
			m_Key = GLFW_RELEASE;
		}
	}

	Input::~Input() = default;

	void Input::UpdateKey(const int key, const int action)
	{
		m_Keys[key] = action;
	}

	void Input::PressKey(const int key)
	{
		m_Keys[key] = GLFW_PRESS;
	}

	void Input::ReleaseKey(const int key)
	{
		m_Keys[key] = GLFW_RELEASE;
	}

	void Input::UpdateMouseButton(const int button, const int action)
	{
		m_Buttons[button] = action;
	}

	void Input::PressMouseButton(const int button)
	{
		m_Buttons[button] = GLFW_PRESS;
	}

	void Input::ReleaseMouseButton(const int button)
	{
		m_Buttons[button] = GLFW_RELEASE;
	}

	void Input::UpdateMousePosition(const float64 x, const float64 y)
	{
		m_LastMouseX = m_MouseX;
		m_LastMouseY = m_MouseY;
		m_MouseX = x;
		m_MouseY = y;
	}

	glm::vec2 Input::GetMousePosition()
	{
		return {m_MouseX, m_MouseY};
	}

	glm::vec2 Input::GetLastMousePosition()
	{
		return {m_LastMouseX, m_LastMouseY};
	}

	glm::vec2 Input::GetMouseDelta()
	{
		return {m_MouseX - m_LastMouseX, m_LastMouseY - m_MouseY};
	}

	void Input::UpdateMouseScroll(float64 y)
	{
		m_MouseScroll = y;
	}

	float64 Input::GetMouseScroll()
	{
		return m_MouseScroll;
	}

	bool Input::IsKeyPressed(const int key)
	{
		return m_Keys[key] == GLFW_PRESS || m_Keys[key] == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(const int button)
	{
		return m_Buttons[button] == GLFW_PRESS || m_Buttons[button] == GLFW_REPEAT;
	}
};
