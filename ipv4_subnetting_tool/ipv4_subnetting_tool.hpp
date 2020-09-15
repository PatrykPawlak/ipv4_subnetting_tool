#pragma once

#include <iostream>
#include "tree.hpp"

namespace ipv4_subnetting_tool
{
	class ipv4_subnetting_tool
	{
	public:
		ipv4_subnetting_tool();
		~ipv4_subnetting_tool();

		void run(int, const char**);

	private:
		tree::tree *m_tree;
		void print_help(std::string);
	};
}