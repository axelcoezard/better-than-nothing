#pragma once

namespace BetterThanNothing
{
	class JobManager
	{
	private:
		std::queue<std::function<void()>> m_jobs;
		std::vector<std::thread> m_workers;

		std::atomic<bool> m_running = true;

		std::mutex m_mutex;
		std::condition_variable m_condition;

	public:
		JobManager();
		~JobManager();

		void InitWorkers();
		void QueueJob(const std::function<void()>& job);

	private:
		void AddWorker();
	};
}