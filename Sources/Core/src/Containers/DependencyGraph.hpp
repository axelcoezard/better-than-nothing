//
// Created by acoezard on 04/11/24.
//

#pragma once

#include <string>
#include <utility>
#include <vector>
#include <queue>
#include <array>
#include <stdexcept>
#include <memory>

namespace BetterThanNothing
{
	template<typename T>
	struct DependencyNode
	{
		const std::string& name;
		T data;
		std::int32_t degree{0};
		std::vector<DependencyNode*> dependents;

		DependencyNode(const std::string& name, const T& data)
				: name(name), data(data) {}

		DependencyNode(const DependencyNode&) = delete;
		DependencyNode& operator=(const DependencyNode&) = delete;

		void AddDependency(DependencyNode<T>* dependency)
		{
			dependency->dependents.push_back(this);
		}
	};

	template<typename T>
	class DependencyIterator
	{
	private:
		std::vector<DependencyNode<T>*> m_nodes;

	public:
		DependencyIterator() = default;
		explicit DependencyIterator(const std::vector<DependencyNode<T>*> nodes)
				: m_nodes(std::move(nodes)) {}

		[[nodiscard]]
		typename std::vector<DependencyNode<T>*>::iterator Begin()
		{
			return m_nodes.begin();
		}

		[[nodiscard]]
		typename std::vector<DependencyNode<T>*>::iterator End()
		{
			return m_nodes.end();
		}
	};

	template<typename T>
	class DependencyGraph
	{
	private:
		const std::string& m_name;
		std::vector<std::unique_ptr<DependencyNode<T>>> m_nodes;
		DependencyIterator<T> m_iterator;

	public:
		DependencyGraph() = default;
		explicit DependencyGraph(const std::string& name): m_name(name) {}

		~DependencyGraph() = default;

		// Prevent copy to ensure unique ownership of nodes
		DependencyGraph(const DependencyGraph&) = delete;
		DependencyGraph& operator=(const DependencyGraph&) = delete;

		DependencyNode<T>* AddNode(const std::string& name, const T& data)
		{
			auto node = std::make_unique<DependencyNode<T>>(name, data);
			auto* nodePtr = node.get();
			m_nodes.push_back(std::move(node));
			return nodePtr;
		}

		DependencyIterator<T> ComputeDependencies()
		{
			_computeDegrees();
			_topologicalSort();
			return m_iterator;
		}

		[[nodiscard]]
		const std::string& GetName() const
		{
			return m_name;
		}

	private:
		void _computeDegrees()
		{
			for (const auto& node : m_nodes)
				node->degree = 0;

			for (const auto& node : m_nodes)
			{
				for (auto* dependency : node->dependents)
					dependency->degree++;
			}
		}

		void _topologicalSort()
		{
			std::queue<DependencyNode<T>*> queue;
			std::vector<DependencyNode<T>*> result;

			for (const auto& node : m_nodes)
			{
				if (node->degree == 0)
					queue.push(node.get());
			}

			while (!queue.empty())
			{
				auto* currentNode = queue.front();
				queue.pop();
				result.push_back(currentNode);

				for (auto* dependency : currentNode->dependents)
				{
					dependency->degree--;
					if (dependency->degree == 0)
						queue.push(dependency);
				}
			}

			if (result.size() > m_nodes.size())
				throw std::runtime_error("Dependency graph has a cycle");

			m_iterator = DependencyIterator<T>(std::move(result));
		}

	};
} // BetterThanNothing


