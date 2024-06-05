#pragma once

namespace BetterThanNothing
{
	class ConfigManager;
	class ResourceManager;

	class Window;
	class Device;
	class Renderer;

	class Scene;
	class Event;

	/**
	 * @brief The Application class is the main class of the engine's applications.
	 */
	class Application
	{
	protected:
		/**
		 * @brief A pointer to the ConfigManager
		*/
		std::shared_ptr<ConfigManager> m_ConfigManager;

		/**
		 * @brief A pointer to the Window
		*/
		std::shared_ptr<Window> m_Window;

		/**
		 * @brief A pointer to the Device
		*/
		std::shared_ptr<Device> m_Device;

		/**
		 * @brief A pointer to the ResourceManager
		*/
		std::shared_ptr<ResourceManager> m_ResourceManager;

		/**
		 * @brief A pointer to the Renderer
		*/
		std::shared_ptr<Renderer> m_Renderer;

		/**
		 * @brief The Scenes of the Application
		*/
		std::vector<std::shared_ptr<Scene>> m_Scenes;

		/**
		 * @brief The current Scene of the Application
		*/
		u32 m_CurrentSceneId;

	public:
		/**
		 * @brief Construct a new Application object
		 * @param title The title of the window
		 * @param width The width of the window
		 * @param height The height of the window
		*/
		Application();

		/**
		 * @brief Destroy the Application object
		*/
		virtual ~Application();

		/**
		 * @brief Run the Application
		*/
		void Run();

		/**
		 * @brief Called when the Application is enabled
		*/
		virtual void OnEnable() = 0;

		/**
		 * @brief Called when the Application is disabled
		*/
		virtual void OnDisable() = 0;

		/**
		 * @brief Called when an Event is received
		 * @param event A pointer to the Event object received
		*/
		void OnEvent(Event* event);

		/**
		 * @brief Create a Scene object and register it to the Application
		 *
		 * @param name The name of the Scene
		 * @return A pointer to the created Scene object
		 */
		std::shared_ptr<Scene> CreateScene(const std::string& name);

		/**
		 * @brief Get the Window object
		 * @return A pointer to the Window object
		 */
		std::shared_ptr<Window>& GetWindow() { return m_Window; }

		/**
		 * @brief Get the Device object
		 * @return A pointer to the Device object
		 */
		std::shared_ptr<Device>& GetDevice() { return m_Device; }

		/**
		 * @brief Get the Renderer object
		 * @return A pointer to the Renderer object
		 */
		std::shared_ptr<Renderer>& GetRenderer() { return m_Renderer; }
	};
};
