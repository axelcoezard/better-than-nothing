//
// Created by acoezard on 03/11/24.
//

#pragma once

#include <thread>
#include <vector>

#include "Job.hpp"
#include "JobGraph.hpp"

namespace BetterThanNothing
{
	class JobManager
	{
	private:
		const size_t MAX_THREADS = std::thread::hardware_concurrency();
		const size_t RESERVED_THREADS = 1;
		const size_t AVAILABLE_THREADS = MAX_THREADS - RESERVED_THREADS;

		std::vector<std::thread> m_threads;

	public:
		JobManager()
		{
			m_threads.reserve(AVAILABLE_THREADS);
		}

		void Execute(const JobGraph& jobGraph)
		{
			(void) jobGraph;
			// TODO: Implement job execution
		}
	};
};