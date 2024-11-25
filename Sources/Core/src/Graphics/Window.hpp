#pragma once

namespace BetterThanNothing
{
	class Window
	{
		GLFWwindow* m_Window = nullptr;

		std::string m_Title{};

		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

		bool m_fullscreen = false;
		bool m_resizable = true;
		bool m_Resized = false;

	public:
		Window() = default;
		explicit Window(std::string  title, uint32_t width, uint32_t height, bool fullscreen = false, bool resizable = false);
		~Window();

		[[nodiscard]]
		GLFWwindow* Handle() const;

		static void PollEvents();

		void Show() const;

		[[nodiscard]]
		bool ShouldClose() const;

		[[nodiscard]]
		std::pair<uint32_t, uint32_t> GetFramebufferSize() const;

		[[nodiscard]]
		bool IsResized() const;
		void SetResized(bool resized);

	protected:
		void SetWidth(uint32_t width);
		void SetHeight(uint32_t height);

		static void ResizeCallback(GLFWwindow* window, int width, int height);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseCursorCallback(GLFWwindow* window, double xpos, double ypos);
		static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	};
};
