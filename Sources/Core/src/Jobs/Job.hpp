#include <utility>

//
// Created by acoezard on 03/11/24.
//

#pragma once

namespace BetterThanNothing
{
	struct Job
	{
		std::string m_name;
		std::function<void()> function;
		std::vector<Job> dependencies;
		bool executed = false;

		void AddDependency(const Job& job)
		{
			dependencies.push_back(job);
		}

		bool HasDependencies() const
		{
			return !dependencies.empty();
		}

		void Execute()
		{
			if (!executed && CanExecute())
			{
				function();
				executed = true;
			}
		}

		bool CanExecute() const
		{
			return std::ranges::all_of(dependencies, [](const Job& dependency) {
				return dependency.executed;
			});
		}
	};
} // BetterThanNothing
