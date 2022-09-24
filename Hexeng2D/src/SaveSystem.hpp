#ifndef SAVE_SYSTEM_HPP
#define SAVE_SYSTEM_HPP

#include <fstream>
#include <unordered_map>

#include "Hexeng.hpp"

namespace Hexeng
{

	class HXG_DECLSPEC SaveVar
	{
	public :

		uint64_t id = 0;
		void* value = nullptr;
		uint64_t unit_size = 0;
		std::function<uint64_t(void** val_ptr)> get_size = nullptr;
		std::function<void(uint64_t, void** val_ptr)> reserve = nullptr;

		SaveVar() = default;

		SaveVar(uint64_t id_p, void* val_p, uint64_t unit_size_p,
			std::function<uint64_t(void** val_ptr)> get_size_p,
			std::function<void(uint64_t, void** val_ptr)> reserve_p);

		SaveVar(uint64_t id_p, void* val_p, uint64_t size_p, uint64_t unit_size_p);
	};

	class HXG_DECLSPEC SaveFile
	{
	public :

		SaveFile() = default;

		void add_var(const SaveVar& var);

		void save(const std::string& filepath);
		void load(const std::string& filepath);

	private :

		std::fstream m_file;

		std::vector<SaveVar> m_vars;
		std::unordered_map<uint64_t, SaveVar> m_var_map;
	};

}

#endif