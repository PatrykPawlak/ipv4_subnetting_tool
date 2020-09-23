#include "ipv4_subnetting_tool.hpp"

namespace ipv4_subnetting_tool
{
	namespace ipv4_t = tree;

	ipv4_subnetting_tool::ipv4_subnetting_tool()
	{
		m_tree = nullptr;
	}

	ipv4_subnetting_tool::~ipv4_subnetting_tool()
	{
		if (m_tree != nullptr) delete m_tree;
	}

	void ipv4_subnetting_tool::run(int p_argc, const char** p_argv)
	{
		if (p_argc < 3)
		{
			print_help(p_argv[0]);
			return;
		}
		else
		{
			std::vector<std::string> args(p_argv + 1, p_argv + p_argc);
			std::string in_f_name, out_f_name;

			for (auto it = args.begin(); it != args.end(); ++it)
			{
				if (*it == "-h" || *it == "--help")
				{
					print_help(p_argv[0]);
				}

				if (*it == "-i" || *it == "--input")
				{
					in_f_name = *(++it);
				}

				if (*it == "-o" || *it == "--output")
				{
					out_f_name = *(++it);
				}
			}

			if (!in_f_name.empty() && !out_f_name.empty())
			{
				m_tree = new ipv4_t::tree();

				m_tree->read_from_file(in_f_name);

				if (m_tree->isEmpty()) return;

				m_tree->save_to_file(out_f_name);
			}
			else
			{
				return;
			}
		}
	}

	void ipv4_subnetting_tool::print_help(std::string p_program_name)
	{
			std::size_t found = p_program_name.find_last_of("/\\");
			std::string tool_name;

			if (found != std::string::npos) {
				tool_name = p_program_name.substr(found + 1);
			}
			else {
				tool_name = p_program_name;
			}

			std::cout << std::endl << tool_name << " -i [inputFile] -o [outputFile]\n\n"
				<< "Description:\n" << "Enables a user to easy check and fix errors in subnetting.\n\n"
				<< "Parameter List:\n" << "-h, --help\t\tDisplays this help message and exit.\n"
				<< "-i, --input\t\tDefine input file.\n"
				<< "-o, --output\t\tDefine output file.\n\n";
	}
}
