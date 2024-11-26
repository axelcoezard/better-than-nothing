#pragma once

namespace BetterThanNothing
{
	class Window
	{
		GLFWwindow* m_Window = nullptr;

		std::string m_Title{};

		uint32 m_Width = 0;
		uint32 m_Height = 0;

		bool m_fullscreen = false;
		bool m_resizable = true;
		bool m_Resized = false;

	public:
		Window() = default;
		explicit Window(std::string  title, uint32 width, uint32 height, bool fullscreen = false, bool resizable = false);
		~Window();

		[[nodiscard]]
		GLFWwindow* Handle() const;

		static void PollEvents();

		void Show() const;

		[[nodiscard]]
		bool ShouldClose() const;

		[[nodiscard]]
		std::pair<uint32, uint32> GetFramebufferSize() const;

		[[nodiscard]]
		bool IsResized() const;
		void SetResized(bool resized);

	protected:
		void SetWidth(uint32 width);
		void SetHeight(uint32 height);

		static void ResizeCallback(GLFWwindow* window, int width, int height);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseCursorCallback(GLFWwindow* window, float64 xpos, float64 ypos);
		static void MouseScrollCallback(GLFWwindow* window, float64 xoffset, float64 yoffset);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	};
};
