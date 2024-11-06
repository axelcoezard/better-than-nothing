#include <BetterThanNothing.hpp>
#include <utility>

namespace BetterThanNothing
{
	Window::Window(const std::string& title, const int width, const int height)
		: m_Title(title), m_Width(width), m_Height(height)
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

		glfwSetWindowUserPointer(m_Window, this);
		glfwSetFramebufferSizeCallback(m_Window, ResizeCallback);
		glfwSetKeyCallback(m_Window, KeyCallback);
		glfwSetCursorPosCallback(m_Window, MouseCursorCallback);
		glfwSetScrollCallback(m_Window, MouseScrollCallback);
		glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
	}

	Window::~Window()
	{
		if (m_Window != nullptr) {
			glfwDestroyWindow(m_Window);
			glfwTerminate();
		}
	}

	void Window::SetEventCallback(std::function<void(Event*)> eventcallback)
	{
		m_EventCallback = std::move(eventcallback);
	}

	void Window::ResizeCallback(GLFWwindow* window, int width, int height)
	{
		const auto windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
		windowPtr->SetWidth(width);
		windowPtr->SetHeight(height);
		windowPtr->SetResized(true);
	}

	void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		Input::UpdateKey(key, action);

		const auto windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

		KeyEvent* event = nullptr;
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			event = new KeyPressEvent(key, scancode, mods);
		} else {
			event = new KeyReleaseEvent(key, scancode, mods);
		}

		windowPtr->m_EventCallback(event);
	}

	void Window::MouseCursorCallback(GLFWwindow* window, f64 xpos, f64 ypos)
	{
		(void) window;
		Input::UpdateMousePosition(xpos, ypos);
	}

	void Window::MouseScrollCallback(GLFWwindow* window, f64 xoffset, f64 yoffset)
	{
		(void) window;
		(void) xoffset;
		Input::UpdateMouseScroll(yoffset);
	}

	void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		(void) window;
		(void) mods;
		Input::UpdateMouseButton(button, action);
	}
};
