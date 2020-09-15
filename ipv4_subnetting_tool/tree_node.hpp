#pragma once

#include "subnet_allocation.hpp"

namespace ipv4_subnetting_tool
{
	namespace tree
	{
		class tree_node
		{
		public:
			tree_node() : m_parent(nullptr), m_first_child(nullptr), m_prev_sibling(nullptr), m_next_sibling(nullptr) {};
			tree_node(network::subnet_allocation& p_subnet_alloc) : m_data(p_subnet_alloc) {};

			friend class tree;

		private:
			network::subnet_allocation m_data;

			tree_node* m_parent = nullptr;
			tree_node* m_first_child = nullptr;
			tree_node* m_prev_sibling = nullptr;
			tree_node* m_next_sibling = nullptr;
		};
	}
}
