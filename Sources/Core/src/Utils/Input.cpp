#include "../BetterThanNothing.hpp"

namespace BetterThanNothing
{
	int Input::m_keys[KEYBOARD_KEY_COUNT] = {};

	float64 Input::m_mouseX = 0;
	float64 Input::m_mouseY = 0;
	float64 Input::m_lastMouseX = 0;
	float64 Input::m_lastMouseY = 0;
	float64 Input::m_mouseScroll = 0;

	int Input::m_buttons[MOUSE_BUTTON_COUNT] = {};

	Input::Input()
	{
		for (int & m_Key : m_keys) {
			m_Key = GLFW_RELEASE;
		}
	}

	void Input::UpdateKey(const int key, const int action)
	{
		m_keys[key] = action;
	}

	void Input::PressKey(const int key)
	{
		m_keys[key] = GLFW_PRESS;
	}

	void Input::ReleaseKey(const int key)
	{
		m_keys[key] = GLFW_RELEASE;
	}

	void Input::UpdateMouseButton(const int button, const int action)
	{
		m_buttons[button] = action;
	}

	void Input::PressMouseButton(const int button)
	{
		m_buttons[button] = GLFW_PRESS;
	}

	void Input::ReleaseMouseButton(const int button)
	{
		m_buttons[button] = GLFW_RELEASE;
	}

	void Input::UpdateMousePosition(const float64 x, const float64 y)
	{
		m_lastMouseX = m_mouseX;
		m_lastMouseY = m_mouseY;
		m_mouseX = x;
		m_mouseY = y;
	}

	glm::vec2 Input::GetMousePosition()
	{
		return {m_mouseX, m_mouseY};
	}

	glm::vec2 Input::GetLastMousePosition()
	{
		return {m_lastMouseX, m_lastMouseY};
	}

	glm::vec2 Input::GetMouseDelta()
	{
		return {m_mouseX - m_lastMouseX, m_lastMouseY - m_mouseY};
	}

	void Input::UpdateMouseScroll(float64 y)
	{
		m_mouseScroll = y;
	}

	float64 Input::GetMouseScroll()
	{
		return m_mouseScroll;
	}

	bool Input::IsKeyPressed(const int key)
	{
		return m_keys[key] == GLFW_PRESS || m_keys[key] == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(const int button)
	{
		return m_buttons[button] == GLFW_PRESS || m_buttons[button] == GLFW_REPEAT;
	}
};
