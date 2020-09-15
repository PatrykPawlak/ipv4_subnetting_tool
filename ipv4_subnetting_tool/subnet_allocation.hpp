#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <regex>

#include "addr_utils.hpp"

namespace ipv4_subnetting_tool
{
	namespace network
	{
		class subnet_allocation
		{
		public:
			subnet_allocation();
			subnet_allocation(uint32_t, uint32_t, std::string);
			subnet_allocation(std::string, std::string, std::string);
			~subnet_allocation() = default;

			uint32_t get_uint_ip_addr(void) const { return m_ip_addr; };
			uint32_t get_uint_mask_addr(void) const { return m_mask_addr; };
			std::string get_comment(void) const { return m_comment; };

			std::string get_string_ip_addr(void) const;
			std::string get_string_mask_addr(notation_t) const;

			std::pair<std::string, std::string> get_string_addr_range(notation_t) const;
			std::pair<uint32_t, uint32_t> get_uint_addr_range(void) const;

			std::ostream& print(std::ostream&, const notation_t) const;

		private:
			uint32_t m_ip_addr;
			uint32_t m_mask_addr;
			std::string m_comment;
		};

	}
}
