#include "tree.hpp"

namespace ipv4_subnetting_tool
{
	namespace tree
	{
		tree::~tree()
		{
			delete_node(m_root_node);
		}

		void tree::delete_node(tree_node* p_node)
		{
			if (p_node)
			{
				delete_node(p_node->m_first_child);
				delete_node(p_node->m_next_sibling);
				if (p_node->m_parent != nullptr) p_node->m_parent->m_first_child = nullptr;
				delete p_node;
			}
		}

		void tree::insert_node(tree_node** p_new_node)
		{
			insert_with_sorting(&(this->m_root_node), p_new_node);
		}

		void tree::print()
		{
			print_tree(*m_root_node);
		}

		void tree::write_to_file(std::ofstream** p_ofs, tree_node& p_root_node)
		{
			tree_node* current_iterator = &p_root_node;
			static int indent_counter = 0;
			while (current_iterator != nullptr)
			{
				for (int it = 0; it < indent_counter; ++it)
				{
					(**p_ofs) << "    ";
				}
				++indent_counter;

				std::string ip = current_iterator->m_data.get_string_ip_addr();
				std::string cidr_mask = current_iterator->m_data.get_string_mask_addr(ipv4_subnetting_tool::network::notation_t::cidr);
				std::string dotted_mask = current_iterator->m_data.get_string_mask_addr(ipv4_subnetting_tool::network::notation_t::dotted);
				std::string comment = current_iterator->m_data.get_comment();

				(**p_ofs) << std::left << ip << "/" << cidr_mask << " (" << dotted_mask << ") " << comment << std::endl;

				if (current_iterator != nullptr)
				{
					write_to_file(p_ofs, *(current_iterator->m_first_child));
				}

				current_iterator = current_iterator->m_next_sibling;
				--indent_counter;
			}
		}

		void tree::save_to_file(const std::string& p_filename)
		{
			std::ofstream *ofs = new std::ofstream();

			ofs->open(p_filename, std::ofstream::out);

			if (!ofs->is_open())
			{
				std::cerr << "ERROR: Attempt to open file: [" + p_filename + "] failed.\n\n";
				delete_node(this->m_root_node);
				exit(1);
			}
			else
			{
				std::clog << "Attempt to open the file: [" + p_filename + "] succeeded.\n\n";

				std::clog << "Attempt to write sorted subnets from structure to file. Please wait...\n\n";

				write_to_file(&ofs, *m_root_node);

				std::clog << "Attempt to write sorted subnets to file: [" + p_filename + "] succeeded.\n\n";

				ofs->close();
			}
			delete ofs;
		}

		void tree::read_from_file(const std::string& p_filename)
		{
			std::ifstream ifs;

			ifs.open(p_filename, std::ifstream::in);

			if (!ifs.is_open())
			{
				std::cerr << "ERROR: Attempt to open file: [" + p_filename + "] failed.\n\n";
				exit(1);
			}
			else
			{
				std::clog << "Attempt to open the file: [" + p_filename + "] succeeded.\n\n";

				std::regex ip_pattern("^(((?:[0-9]{1,3}\\.){3}[0-9]{1,3}))");

				std::string valid_octets = "(255|254|252|248|240|224|192|128)";

				std::regex dotted_mask_pattern("\\/((255\\.){3}(255|254|252|248|240|224|192|128|0)|((255\\.){2}(255|254|252|248|240|224|192|128|0)\\.0)|(255\\.)(255|254|252|248|240|224|192|128|0)(\\.0){2})");

				std::regex cidr_mask_pattern("\\/([0-9]|[1-2][0-9]|3[0-2])\\s");

				std::regex comment_pattern("\\s[a-zA-z](.+)$");

				std::smatch ip_result;
				std::smatch cidr_mask_result;
				std::smatch dotted_mask_result;
				std::smatch comment_result;
				

				std::clog << "Attempt to read subnets from file to structure. Please wait...\n\n";

				uint32_t counter = 0;

				for (std::string line; std::getline(ifs, line);)
				{

					std::string ip = "";
					std::string mask = "";
					std::string comment = "";

					bool ip_found_flag = std::regex_search(line, ip_result, ip_pattern);
					bool cidr_mask_found_flag = std::regex_search(line, cidr_mask_result, cidr_mask_pattern);
					bool dotted_mask_found_flag = std::regex_search(line, dotted_mask_result, dotted_mask_pattern);
					bool comment_found_flag = std::regex_search(line, comment_result, comment_pattern);

					if (ip_found_flag)
					{
						ip = ip_result[0];
						//ip = std::regex_replace(ip, std::regex("\/"), "");
					}

					if (cidr_mask_found_flag)
					{
						mask = cidr_mask_result[0];
						mask = std::regex_replace(mask, std::regex("\/"), "");
					}

					if (dotted_mask_found_flag)
					{
						mask = dotted_mask_result[0];
						mask = std::regex_replace(mask, std::regex("\/"), "");
					}

					if (comment_found_flag)
					{
						comment = comment_result[0];
						comment = std::regex_replace(comment, std::regex("^ "), "");
					}

					if (ip_found_flag && (cidr_mask_found_flag || dotted_mask_found_flag) && comment_found_flag)
					{
						namespace ipv4_n = ipv4_subnetting_tool::network;

						ipv4_n::subnet_allocation subnet_alloc = ipv4_n::subnet_allocation(ip, mask, comment);
						tree_node* new_node = new tree_node(subnet_alloc);

						insert_node(&new_node);

						new_node = nullptr;

						delete new_node;
					}
					else
					{
						std::cerr << "ERROR: Cannot add subnet allocation from line [" << counter << "] {";
						if (!ip_found_flag) std::cerr << "{incorrect ip addr} ";
						else std::cerr << "{" << ip << "} ";
						if (!dotted_mask_found_flag || !cidr_mask_found_flag) std::cerr << "{incorrect mask addr} ";
						else std::cerr << "{" << mask << " ";
						if (!comment_found_flag) std::cerr << "{empty comment}";
						else std::cerr << "{" << comment << "}";
						std::cerr << "}\n\n";
					}
					++counter;
				}
			
				std::clog << "Attempt to read subnets from file: [" + p_filename + "] succeeded.\n\n";

				ifs.close();
			}
		}

		void tree::insert_with_sorting(tree_node** p_root_node, tree_node** p_new_node)
		{
			// root node is null
			if ((*p_root_node) == nullptr)
			{
				(*p_root_node) = (*p_new_node);
			}
			// end of root node is null

			// root node isn't null
			else
			{
				tree_node* current_iterator = nullptr;
				tree_node* parent = nullptr;
				tree_node* last_child = nullptr;
				tree_node* first_sibling = nullptr;

				std::pair<uint32_t, uint32_t> root_node_range;
				std::pair<uint32_t, uint32_t> new_node_range;

				bool is_new_in_root = false;
				bool is_root_in_new = false;
				bool is_root_and_new_equal = false;

				root_node_range = (**p_root_node).m_data.get_uint_addr_range();
				new_node_range = (**p_new_node).m_data.get_uint_addr_range();

				is_new_in_root = root_node_range.first <= new_node_range.first && root_node_range.second >= new_node_range.second;
				is_root_in_new = new_node_range.first <= root_node_range.first && new_node_range.second >= root_node_range.second;
				is_root_and_new_equal = new_node_range == root_node_range;

				// insert new node as child of root node
				if (is_new_in_root && !is_root_and_new_equal)
				{
					// search where to insert new node in root->children

					current_iterator = (*p_root_node)->m_first_child;

					// p_root_node has children
					if (current_iterator != nullptr)
					{
						// search where insert
						while (current_iterator->m_next_sibling != nullptr && current_iterator->m_next_sibling->m_data.get_uint_ip_addr() < (*p_new_node)->m_data.get_uint_ip_addr())
						{
							current_iterator = current_iterator->m_next_sibling;
						}

						insert_with_sorting(&current_iterator, &(*p_new_node));
					}
					// p_root_node hasn't children
					else
					{
						(*p_new_node)->m_parent = (*p_root_node);
						(*p_root_node)->m_first_child = (*p_new_node);
					}
				}
				// end of insert new node as child of root node

				// insert root node as child of new node
				else if (is_root_in_new && !is_root_and_new_equal)
				{
					current_iterator = (*p_root_node);

					// search where is future last child and first sibling of new node
					while (current_iterator->m_next_sibling != nullptr
						&& !(new_node_range.first <= current_iterator->m_next_sibling->m_data.get_uint_ip_addr())
						&& new_node_range.second >= current_iterator->m_next_sibling->m_data.get_uint_ip_addr())
					{
						current_iterator = current_iterator->m_next_sibling;
					}

					last_child = current_iterator->m_next_sibling;
					if (last_child != nullptr) first_sibling = last_child->m_next_sibling;

					// root node has parent
					if ((*p_root_node)->m_parent != nullptr)
					{
						parent = (*p_root_node)->m_parent;
						parent->m_first_child = (*p_new_node);
						(*p_new_node)->m_parent = parent;
					}

					// root node has prev sibling
					if ((*p_root_node)->m_prev_sibling != nullptr)
					{
						(*p_root_node)->m_prev_sibling->m_next_sibling = (*p_new_node);
						(*p_new_node)->m_prev_sibling = (*p_root_node)->m_prev_sibling;
						(*p_root_node)->m_prev_sibling = nullptr;
					}

					// first sibling is not null
					if (first_sibling != nullptr)
					{
						first_sibling->m_prev_sibling = (*p_new_node);
					}

					(*p_root_node)->m_parent = (*p_new_node);
					(*p_new_node)->m_first_child = (*p_root_node);
					(*p_new_node)->m_next_sibling = first_sibling;
					(*p_root_node) = (*p_new_node);
				}
				// end of insert root node as child of new node

				// new node is already allocated
				else if (is_root_and_new_equal)
				{
					std::string ip_addr = (*p_root_node)->m_data.get_string_ip_addr();
					std::string mask_addr = (*p_root_node)->m_data.get_string_mask_addr(network::notation_t::cidr);
					std::string msg = "ERROR: Attempt to add [" + ip_addr + "/" + mask_addr + " " + (*p_new_node)->m_data.get_comment() + "]. Subnet is already allocated with comment: [" + (*p_root_node)->m_data.get_comment() + "] so new wasn't added.";
					
					std::cerr << msg << std::endl;
				}
				// end of new node is already allocated

				// insert new node after root node
				else if (root_node_range.second < new_node_range.first && !is_root_and_new_equal)
				{

					current_iterator = (*p_root_node)->m_next_sibling;

					// root node has next sibling
					if (current_iterator != nullptr)
					{
						// search where add new node after root node
						insert_with_sorting(&current_iterator, &(*p_new_node));
					}
					// root node hasn't next sibling
					else
					{
						(*p_root_node)->m_next_sibling = (*p_new_node);
						(*p_new_node)->m_prev_sibling = (*p_root_node);
					}
				}
				// end of insert new node after root node

				// insert new node before root node
				else if (root_node_range.first > new_node_range.second && !is_root_in_new)
				{
					// root node has parent
					if ((*p_root_node)->m_parent != nullptr)
					{
						parent = (*p_root_node)->m_parent;

						(*p_new_node)->m_parent = parent;
						parent->m_first_child = (*p_new_node);
						(*p_root_node)->m_parent = nullptr;
					}

					(*p_new_node)->m_next_sibling = (*p_root_node);
					(*p_root_node)->m_prev_sibling = (*p_new_node);
				}
				// end of insert new node before root node

				// handle exceptions
				else
				{
					std::string root_ip_addr = (*p_root_node)->m_data.get_string_ip_addr();
					std::string root_mask_addr = (*p_root_node)->m_data.get_string_mask_addr(network::notation_t::cidr);
					std::string new_ip_addr = (*p_new_node)->m_data.get_string_ip_addr();
					std::string new_mask_addr = (*p_new_node)->m_data.get_string_mask_addr(network::notation_t::cidr);
					std::string msg = "INSERT NEW NODE ERROR: ";
					msg += "Root(" + root_ip_addr + "/" + root_mask_addr + ", " + (*p_root_node)->m_data.get_comment() + "), ";
					msg += "New(" + new_ip_addr + "/" + new_mask_addr + ", " + (*p_new_node)->m_data.get_comment() + ")";
					std::cerr << msg << "\n\n";
				}
				// end of handle exceptions
			}
			// end of root node isn't null
		}

		void tree::print_tree(tree_node& p_root_node)
		{
			tree_node* current_iterator = &p_root_node;
			static int indent_counter = 0;
			while (current_iterator != nullptr)
			{
				for (int it = 0; it < indent_counter; ++it)
				{
					std::cout << "    ";
				}
				++indent_counter;
				
				std::string ip = current_iterator->m_data.get_string_ip_addr();
				std::string cidr_mask = current_iterator->m_data.get_string_mask_addr(ipv4_subnetting_tool::network::notation_t::cidr);
				std::string dotted_mask = current_iterator->m_data.get_string_mask_addr(ipv4_subnetting_tool::network::notation_t::dotted);
				std::string comment = current_iterator->m_data.get_comment();

				std::printf("%-s/%-s (%-s) %-s\n", ip.c_str(), cidr_mask.c_str(), dotted_mask.c_str(), comment.c_str());

				if (current_iterator != nullptr)
				{
					tree::print_tree(*(current_iterator->m_first_child));
				}

				current_iterator = current_iterator->m_next_sibling;
				--indent_counter;
			}
		}
	}
}
