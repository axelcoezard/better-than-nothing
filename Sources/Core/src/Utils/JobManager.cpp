#include "JobManager.hpp"

#include <thread>
#include <vector>
#include <queue>

namespace BetterThanNothing
{
	constexpr const uint32_t ALREADY_USED_THREADS_COUNT = 2; // Gameplay and Render threads

	JobManager::JobManager()
	{
		this->InitWorkers();
	}

	JobManager::~JobManager()
	{
		m_running.store(false);

		for (std::thread& worker : m_workers)
			worker.join();
	}

	void JobManager::InitWorkers()
	{
		uint32_t hardwareThreadsCount = std::thread::hardware_concurrency();
		uint32_t availableThreadsCount = hardwareThreadsCount - ALREADY_USED_THREADS_COUNT;

		m_workers.reserve(availableThreadsCount);
		for (size_t i = 0; i < availableThreadsCount; i++)
			this->AddWorker();
	}

	void JobManager::QueueJob(const std::function<void()>& job)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_jobs.push(job);
		}

		m_condition.notify_one();
	}

	void JobManager::AddWorker()
	{
		m_workers.emplace_back([this]() {
			while (m_running)
			{
				std::function<void()> job;
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					m_condition.wait(lock, [this]() { return !m_jobs.empty(); });

					job = m_jobs.front();
					m_jobs.pop();
				}
				m_currentJobsCount.fetch_add(1);
				job();
				m_currentJobsCount.fetch_sub(1);
				m_finishedJobsCount.fetch_add(1);
			}
		});

		LOG_SUCCESS("Worker added")
	}

	uint32_t JobManager::GetFinishedJobsCount() const
	{
		return m_finishedJobsCount.load();
	}

	uint32_t JobManager::GetCurrentJobsCount() const
	{
		return m_currentJobsCount.load();
	}

	uint32_t JobManager::GetWaitingJobsCount() const
	{
		return m_jobs.size();
	}
}