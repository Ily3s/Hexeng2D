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
		uint64_t unit_size = 0;
		std::function<uint64_t(void** val_ptr)> get_size = nullptr;
		std::function<void(uint64_t, void** val_ptr)> reserve = nullptr;

		SaveVar() = default;

		SaveVar(uint64_t id_p, void* val_p, uint64_t unit_size_p,
			std::function<uint64_t(void** val_ptr)> get_size_p,
			std::function<void(uint64_t, void** val_ptr)> reserve_p);

		SaveVar(uint64_t id_p, void* val_p, uint64_t size_p, uint64_t unit_size_p);
	};

	/// <summary>
	/// A save instance. It isn't closely related to a filepath but rather to a bunch of variables references.
	/// </summary>
	class HXG_DECLSPEC SaveFile
	{
	public :

		SaveFile() = default;

		/// <summary>
		/// Add a variable to the instance
		/// </summary>
		void add_var(const SaveVar& var);

		/// <summary>
		/// Create or overwrite a save file at filepath that stores all the variables of the instance.
		/// </summary>
		/// <param name="filepath">Has to be a valid filepath to a file in an existing directory</param>
		void save(const std::string& filepath);

		/// <summary>
		/// Set the variables of the instance to what's in the file at filepath.
		/// </summary>
		/// <param name="filepath">Has to be the path to a save that exists and has been previously created with save()</param>
		void load(const std::string& filepath);

	private :

		std::fstream m_file;

		std::vector<SaveVar> m_vars;
		std::unordered_map<uint64_t, SaveVar> m_var_map;
	};

}

#endif