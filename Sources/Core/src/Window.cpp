#include <BetterThanNothing.hpp>
#include <utility>

namespace BetterThanNothing
{
	Window::Window(std::string  title, const uint32 width, const uint32 height, const bool fullscreen, const bool resizable)
		: m_Title(std::move(title)), m_Width(width), m_Height(height), m_fullscreen(fullscreen), m_resizable(resizable)
	{
		if (glfwInit() == GLFW_FALSE)
			throw std::runtime_error("Failed to initialize GLFW");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, m_resizable ? GLFW_TRUE : GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		GLFWmonitor* monitor = m_fullscreen
			? glfwGetPrimaryMonitor()
			: nullptr;

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), monitor, nullptr);

		if (m_Window == nullptr)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}

		glfwSetWindowUserPointer(m_Window, this);
		glfwSetFramebufferSizeCallback(m_Window, ResizeCallback);
		glfwSetKeyCallback(m_Window, KeyCallback);
		glfwSetCursorPosCallback(m_Window, MouseCursorCallback);
		glfwSetScrollCallback(m_Window, MouseScrollCallback);
		glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
	}

	Window::~Window()
	{
		if (m_Window != nullptr)
		{
			glfwDestroyWindow(m_Window);
			glfwTerminate();
		}
	}

	GLFWwindow* Window::Handle() const
	{
		return m_Window;
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::Show() const
	{
		glfwShowWindow(m_Window);
	}

	bool Window::ShouldClose() const
	{
		if (m_Window == nullptr)
			return true;
		return glfwWindowShouldClose(m_Window) == GLFW_TRUE;
	}

	std::pair<uint32, uint32> Window::GetFramebufferSize() const
	{
		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);
		return {width, height};
	}

	bool Window::IsResized() const
	{
		return m_Resized;
	}

	void Window::SetResized(const bool resized)
	{
		m_Resized = resized;
	}

	void Window::SetWidth(const uint32 width)
	{
		m_Width = width;
	}

	void Window::SetHeight(const uint32 height)
	{
		m_Height = height;
	}

	void Window::ResizeCallback(GLFWwindow* window, const int32 width, const int32 height)
	{
		const auto windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
		windowPtr->SetWidth(width);
		windowPtr->SetHeight(height);
		windowPtr->SetResized(true);
	}

	void Window::KeyCallback(GLFWwindow* window, const int32 key, const int32 scancode, const int32 action, const int32 mods)
	{
		(void) scancode;
		(void) mods;

		if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		// TODO: Input::UpdateKey(key, action);
	}

	void Window::MouseCursorCallback(GLFWwindow* window, const float64 xpos, const float64 ypos)
	{
		(void) window;
		(void) xpos;
		(void) ypos;
		// TODO:  Input::UpdateMousePosition(xpos, ypos);
	}

	void Window::MouseScrollCallback(GLFWwindow* window, const float64 xoffset, const float64 yoffset)
	{
		(void) window;
		(void) xoffset;
		(void) yoffset;
		// TODO: Input::UpdateMouseScroll(yoffset);
	}

	void Window::MouseButtonCallback(GLFWwindow* window, const int32 button, const int32 action, const int32 mods)
	{
		(void) window;
		(void) mods;
		(void) button;
		(void) action;
		// TODO: Input::UpdateMouseButton(button, action);
	}
};
