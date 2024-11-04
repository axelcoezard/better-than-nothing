//
// Created by acoezard on 03/11/24.
//

#pragma once

#include "../BetterThanNothing.hpp"

namespace BetterThanNothing
{
	typedef std::function<void()> Job;
	typedef DependencyIterator<Job> JobIterator;
	typedef DependencyGraph<Job> JobGraph;

	class JobManager
	{
	private:
		const size_t MAX_THREADS = std::thread::hardware_concurrency();
		const size_t RESERVED_THREADS = 1;
		const size_t AVAILABLE_THREADS = MAX_THREADS - RESERVED_THREADS;

		bool m_running = true;

		std::vector<std::thread> m_jobWorkers;
		std::queue<std::function<void()>> m_jobsQueue;

		std::mutex m_mutex;
		std::condition_variable m_condition;

	public:
		JobManager()
		{
			m_jobWorkers.reserve(AVAILABLE_THREADS);
			for (size_t i = 0; i < AVAILABLE_THREADS; i++)
			{
				m_jobWorkers.emplace_back([this]() {
					while (m_running)
					{
						std::unique_lock<std::mutex> lock(m_mutex);

						m_condition.wait(lock, [this]() {
							return !m_jobsQueue.empty() || !m_running;
						});

						if (!m_running)
							return;

						if (!m_jobsQueue.empty())
						{
							std::function<void()> job = m_jobsQueue.front();
							m_jobsQueue.pop();
							lock.unlock();
							job();
						}
					}
				});
			}
		}

	public:
		void QueueJob(const std::function<void()>& job)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_jobsQueue.push(job);
			lock.unlock();

			m_condition.notify_one();
		}

		void ExecuteGraph(JobGraph& jobGraph)
		{
			static std::unordered_map<std::string, JobIterator> jobIterators;

			if (jobIterators.find(jobGraph.GetName()) == jobIterators.end())
				jobIterators[jobGraph.GetName()] = jobGraph.ComputeDependencies();

			auto begin = jobIterators[jobGraph.GetName()].Begin();
			auto end = jobIterators[jobGraph.GetName()].End();

			while (begin != end)
			{
				QueueJob((*begin)->data);
				begin++;
			}
		}

		void Stop()
		{
			m_running = false;
			m_condition.notify_all();
			for (std::thread& worker : m_jobWorkers)
			{
				std::cout << "Joining worker (count: " << m_jobWorkers.size() << ")" << std::endl;
				if (worker.joinable())
				{
					worker.join();
					std::cout << "Worker joined" << std::endl;
				}
			}
		}
	};
};