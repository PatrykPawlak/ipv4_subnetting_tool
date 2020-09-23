//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "ipv4_subnetting_tool.hpp"

namespace ipv4 = ipv4_subnetting_tool;

int main(int argc, const char** argv)
{
	ipv4::ipv4_subnetting_tool *program = new ipv4::ipv4_subnetting_tool();

	program->run(argc, argv);

	delete program;

	//_CrtDumpMemoryLeaks();
	return 0;
}
