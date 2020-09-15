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
			tree() : m_root_node(nullptr) {};
			~tree();

			void insert_node(tree_node**);
			void print();
			void save_to_file(const std::string&);
			void read_from_file(const std::string&);

		private:
			tree_node* m_root_node;

			void insert_with_sorting(tree_node**, tree_node**);
			void print_tree(tree_node&);
			void write_to_file(std::ofstream**, tree_node&);
			void delete_node(tree_node*);
		};
	}
}
