#pragma once



namespace BetterThanNothing
{
	struct Input {
	protected:
		static constexpr int32 KEYBOARD_KEY_COUNT = 1024;
		static constexpr int32 MOUSE_BUTTON_COUNT = 32;

		static int m_keys[KEYBOARD_KEY_COUNT];

		static float64 m_mouseX;
		static float64 m_mouseY;
		static float64 m_lastMouseX;
		static float64 m_lastMouseY;
		static float64 m_mouseScroll;

		static int m_buttons[MOUSE_BUTTON_COUNT];

	public:
		explicit Input();
		~Input() = default;

		static void UpdateKey(int key, int action);
		static void PressKey(int key);
		static void ReleaseKey(int key);

		static void UpdateMouseButton(int button, int action);
		static void PressMouseButton(int button);
		static void ReleaseMouseButton(int button);
		static void UpdateMousePosition(float64 x, float64 y);

		static glm::vec2 GetMousePosition();
		static glm::vec2 GetLastMousePosition();
		static glm::vec2 GetMouseDelta();

		static void UpdateMouseScroll(float64 y);
		static float64 GetMouseScroll();

		static bool IsKeyPressed(int key);
		static bool IsMouseButtonPressed(int button);
	};
};
