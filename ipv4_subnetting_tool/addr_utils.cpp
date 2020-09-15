#include "addr_utils.hpp"

namespace ipv4_subnetting_tool
{
	namespace network
	{

		uint32_t addr_utils::to_uint(const std::string& p_addr)
		{
			int octets[4];
			uint32_t address = 0;

			if (sscanf_s(p_addr.c_str(), "%d.%d.%d.%d", &octets[3], &octets[2], &octets[1], &octets[0]) != 4) return 0;

			for (int i = 3; i >= 0; --i)
			{
				address |= octets[i] << (i * 8);
			}

			return address;
		}

		std::string addr_utils::to_string(const uint32_t& p_addr)
		{
			int octets[4];
			std::string address = "";

			for (int i = 3; i >= 0; --i)
			{
				octets[i] = (p_addr >> (i * 8)) & 0xff;
				address.append(std::to_string(octets[i]));

				if (i > 0)
					address.append(".");
			}

			return address;
		}

		std::string addr_utils::to_string(const uint32_t& p_addr, const notation_t p_notation)
		{
			std::string address = "";

			switch (p_notation)
			{
			case notation_t::dotted:
				address = addr_utils::to_string(p_addr);
				break;

			case notation_t::cidr:
			default:
				int zero_bits = 0;
				uint32_t addr = p_addr;

				while (addr)
				{
					zero_bits += (addr & 0x01);
					addr >>= 1;
				}

				address = std::to_string(zero_bits);

				break;
			}
			
			return address;
		}
	}
}
