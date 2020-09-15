#include "subnet_allocation.hpp"

namespace ipv4_subnetting_tool
{
	namespace network
	{
		subnet_allocation::subnet_allocation() : m_ip_addr(0), m_mask_addr(0), m_comment("") {};

		subnet_allocation::subnet_allocation(uint32_t p_ip_addr, uint32_t p_mask_addr, std::string p_comment)
		{
			if ((p_ip_addr & p_mask_addr) == p_ip_addr) m_ip_addr = p_ip_addr;
			else m_ip_addr = p_ip_addr & p_mask_addr;

			m_mask_addr = p_mask_addr;
			m_comment = p_comment;
		}

		subnet_allocation::subnet_allocation(std::string p_ip_addr, std::string p_mask_addr, std::string p_comment)
		{
			std::regex cidr_mask_pattern("^([0-9]|[1-2][0-9]|3[0-2])\\s");

			std::smatch mask_result;

			if (std::regex_search(p_mask_addr, mask_result, cidr_mask_pattern))
			{
				uint8_t mask = std::stoi(p_mask_addr);
				uint32_t temp_mask = 0;

				for (int i = 0; i < mask; ++i)
				{
					temp_mask += 1 * (uint32_t)std::pow(2, i);
				}

				m_mask_addr = addr_utils::to_uint(addr_utils::to_string(temp_mask << (32 - mask)));
			}
			else
			{
				m_mask_addr = addr_utils::to_uint(p_mask_addr);
			}

			m_ip_addr = addr_utils::to_uint(p_ip_addr);
			m_comment = p_comment;

			if (!((m_ip_addr & m_mask_addr) == m_ip_addr)) m_ip_addr = m_ip_addr & m_mask_addr;
		}

		std::string subnet_allocation::get_string_ip_addr(void) const
		{
			return addr_utils::to_string(m_ip_addr);
		}

		std::string subnet_allocation::get_string_mask_addr(notation_t p_notation) const
		{
			return addr_utils::to_string(m_mask_addr, p_notation);
		}

		std::pair<std::string, std::string> subnet_allocation::get_string_addr_range(notation_t) const
		{
			return std::pair<std::string, std::string>();
		}

		std::pair<uint32_t, uint32_t> subnet_allocation::get_uint_addr_range(void) const
		{
			uint32_t start_ip_addr = (m_ip_addr & m_mask_addr);
			uint32_t end_ip_addr = (m_ip_addr | ~m_mask_addr);

			return std::make_pair(start_ip_addr, end_ip_addr);
		}

		std::ostream& subnet_allocation::print(std::ostream& p_os, const notation_t p_notation) const
		{
			p_os << std::setw(16) << this->get_string_ip_addr() << "/";

			if (p_notation == notation_t::dotted) p_os << std::setw(16);
			else p_os << std::setw(2);

			p_os << this->get_string_mask_addr(p_notation);
			p_os << "\t" << m_comment;

			return p_os;
		}
	}
}
