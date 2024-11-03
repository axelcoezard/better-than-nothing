//
// Created by acoezard on 03/11/24.
//

#pragma once

namespace BetterThanNothing
{
	class JobGraph
	{
	private:
		std::vector<Job*> m_jobs;

	public:
		JobGraph() = default;

		Job* AddJob(const std::string& name, const std::function<void()>& function)
		{
			Job* job = new Job(name, function);
			m_jobs.push_back(job);
			return job;
		}

		const Job* NextJob()
		{
			for (auto job : m_jobs)
			{

			}

			return nullptr;
		}
	};
} // BetterThanNothing
