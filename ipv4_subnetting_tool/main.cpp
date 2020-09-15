#include "ipv4_subnetting_tool.hpp"

namespace ipv4 = ipv4_subnetting_tool;

int main(int argc, const char** argv)
{
	ipv4::ipv4_subnetting_tool program;

	program.run(argc, argv);

	return 0;
}
