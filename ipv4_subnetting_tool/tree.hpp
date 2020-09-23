#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include "tree_node.hpp"

namespace ipv4_subnetting_tool
{
	namespace tree
	{
		class tree
		{
		public:
			tree() : m_root_node(nullptr), m_counter(0) {};
			~tree();

			void insert_node(tree_node**);
			void print();
			bool isEmpty();
			void save_to_file(const std::string&);
			void read_from_file(const std::string&);

		private:
			tree_node* m_root_node;
			uint32_t m_counter;

			void insert_with_sorting(tree_node**, tree_node**);
			void print_tree(tree_node&);
			void write_to_file(std::ofstream**, tree_node&);
			void delete_node(tree_node*);
		};
	}
}
