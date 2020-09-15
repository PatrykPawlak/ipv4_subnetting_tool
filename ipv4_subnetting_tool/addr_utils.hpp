#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <stdexcept>

namespace ipv4_subnetting_tool
{
	namespace network
	{

		enum class notation_t
		{
			dotted,
			cidr
		};

		class addr_utils
		{
		public:
			static uint32_t to_uint(const std::string&);
			static std::string to_string(const uint32_t&);
			static std::string to_string(const uint32_t&, const notation_t);

		private:
			addr_utils(); // to prevent create instance of addr_utils 
		};
	}
}
