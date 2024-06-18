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

		std::atomic<uint32_t> m_finishedJobsCount = 0;
		std::atomic<uint32_t> m_currentJobsCount = 0;

	public:
		JobManager();
		~JobManager();

		void InitWorkers();
		void QueueJob(const std::function<void()>& job);

		uint32_t GetFinishedJobsCount() const;
		uint32_t GetCurrentJobsCount() const;
		uint32_t GetWaitingJobsCount() const;

	private:
		void AddWorker();
	};
}