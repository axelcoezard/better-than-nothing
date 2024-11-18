#pragma once

#include <cstdint>
#include <string>

namespace BetterThanNothing
{
	/**
	 * @brief The window class that is a wrapper around GLFWwindow and handles some events
	 */
	class Window
	{
		/**
		 * @brief The GLFWwindow pointer
		 */
		GLFWwindow* m_Window{};

		/**
		 * @brief The title of the window
		 */
		std::string m_Title;

		/**
		 * @brief The width of the window
		 */
		int32_t m_Width{};

		/**
		 * @brief The height of the window
		 */
		int32_t m_Height{};

		/**
		 * @brief If the window is fullscreen
		 */
		bool m_fullscreen = false;

		/**
		 * @brief If the window is resizable
		 */
		bool m_resizable = true;

		/**
		 * @brief If the window is resized
		 */
		bool m_Resized = false;

	public:
		Window() = default;

		/**
		* @brief The window constructor
		 * @param title The title of the window
		 * @param width The width of the window
		 * @param height The height of the window
		 * @param fullscreen If the window is fullscreen
		 * @param resizable If the window is resizable
		 */
		Window(std::string  title, int32_t width, int32_t height,  bool fullscreen = false, bool resizable = false);

		/**
		 * @brief The window destructor
		 */
		~Window();

		/**
		 * @brief A wrapper around glfwPollEvents that polls for events
		 */
		static void PollEvents()
		{
			glfwPollEvents();
		}

		/**
		 * @brief A wrapper around glfwWindowShouldClose
		 * @return If the window should close
		 */
		bool ShouldClose() const
		{
			if (m_Window == nullptr)
				return true;
			return glfwWindowShouldClose(m_Window) == GLFW_TRUE;
		}

		/**
		 * @brief A wrapper around glfwSetWindowShouldClose that sets the window to close
		 */
		void Close() const
		{
			if (m_Window != nullptr)
				glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
		}

		/**
		 * @brief Called when the window is resized
		 * @param window The GLFW window that is resized
		 * @param width The new width of the window
		 * @param height The new height of the window
		 */
		static void ResizeCallback(GLFWwindow* window, int width, int height);

		/**
		 * @brief Called when a key is pressed or released
		 * @param window The GLFW window that is resized
		 * @param key The key that is pressed
		 * @param scancode The scancode of the key
		 * @param action The action of the key (PRESS, RELEASE, REPEAT)
		 * @param mods The mods of the key
		 */
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		/**
		 * @brief Called when the mouse cursor is moved
		 * @param window The GLFW window that is resized
		 * @param xpos The x position of the mouse cursor
		 * @param ypos The y position of the mouse cursor
		 */
		static void MouseCursorCallback(GLFWwindow* window, double xpos, double ypos);

		/**
		 * @brief Called when the mouse is scrolled
		 * @param window The GLFW window that is resized
		 * @param xoffset The x offset of the mouse scroll
		 * @param yoffset The y offset of the mouse scroll
		 */
		static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		/**
		 * @brief Called when a mouse button is pressed or released
		 * @param window The GLFW window that is resized
		 * @param button The button that is pressed
		 * @param action The action of the button (PRESS, RELEASE, REPEAT)
		 * @param mods The mods of the button
		 */
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		/**
		 * @brief Gets the GLFWwindow pointer
		 * @return The GLFWwindow pointer
		 */
		 [[nodiscard]]
		GLFWwindow* Handle() const
		{
			return m_Window;
		}

		[[nodiscard]]
		std::pair<uint32_t, uint32_t> GetFramebufferSize() const
		{
			int width, height;
			glfwGetFramebufferSize(m_Window, &width, &height);
			return {width, height};
		}

		/**
		 * @brief Gets the title of the window
		 * @return The title of the window
		 */
		std::string& GetTitle()
		{
			return m_Title;
		}

		/**
		 * @brief Gets the width of the window
		 * @return The width of the window
		 */
		int32_t GetWidth() const
		{
			return m_Width;
		}

		void SetWidth(const int32_t width)
		{
			m_Width = width;
		}

		/**
		 * @brief Gets the height of the window
		 * @return The height of the window
		 */
		int32_t GetHeight() const
		{
			return m_Height;
		}

		void SetHeight(const int32_t height)
		{
			m_Height = height;
		}

		/**
		 * @brief Gets if the window is resized
		 * @return Whether the window is resized or not
		 */
		bool IsResized() const
		{
			return m_Resized;
		}

		/**
		 * @brief Sets if the window is resized
		 */
		void SetResized(const bool resized)
		{
			m_Resized = resized;
		}
	};
};
