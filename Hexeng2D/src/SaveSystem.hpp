#ifndef SAVE_SYSTEM_HPP
#define SAVE_SYSTEM_HPP

#include <fstream>
#include <unordered_map>

#include "Hexeng.hpp"

namespace Hexeng
{

	struct HXG_DECLSPEC SaveVar
	{
		uint64_t id = 0;
		void* value = nullptr;
		std::function<uint64_t(void** val_ptr)> get_size = nullptr;
		std::function<void(uint64_t, void** val_ptr)> reserve = nullptr;

		SaveVar() = default;

		SaveVar(uint64_t id_p, void* val_p,
			std::function<uint64_t(void** val_ptr)> get_size_p,
			std::function<void(uint64_t, void** val_ptr)> reserve_p)
			: id(id_p), value(val_p), get_size(get_size_p), reserve(reserve_p) {}

		SaveVar(uint64_t id_p, void* val_p, uint64_t size_p)
			: id(id_p), value(val_p), get_size([size_p](void**) {return size_p; }), reserve([](uint64_t, void**) {}) {}
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