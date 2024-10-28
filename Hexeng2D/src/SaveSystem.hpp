#ifndef SAVE_SYSTEM_HPP
#define SAVE_SYSTEM_HPP

#include <fstream>
#include <unordered_map>
#include <cstdint>

#include "Hexeng.hpp"

namespace Hexeng
{

	/// <summary>
	/// Get be pushed in a SaveFile
	/// </summary>
	struct HXG_DECLSPEC SaveVar
	{
		uint64_t id = 0;
		void* value = nullptr;
		uint64_t unit_size = 0;
		std::function<uint64_t(void** val_ptr)> get_size = nullptr;
		std::function<void(uint64_t, void** val_ptr)> reserve = nullptr;

		SaveVar() = default;

		/// @brief This constructor is designed for heap allocated (dynamic size and location, std::vector for example) variables.
		/// <param name="id_p">A unique id to identify this variable in a save.</param>
		/// <param name="val_p">The pointer to the data itself.</param>
		/// <param name="unit_size_p">The size (in bytes) of one element of the data.
		/// For example, if the data is an array of int32_t or just one single int32_t, unit_size_p has to be 4</param>
		/// <param name="get_size_p">A function that get called during a save.
		/// The function takes in parameter a ptr to the ptr to the data (void**), and returns the size of the data.
		/// What you want to do in this function is modify the location of the data in case it has changed, and, return the size of the data.
		/// For example, with std::vector vec, you want to do {[](void** val_ptr) {*val_ptr = &vec[0]; return vec.size();}}</param>
		/// <param name="reserve_p">A function that get called during the load of a save. 
		/// This function takes in parameter the size (in bytes) of the saved variable (uint64_t), and, a ptr to the ptr to the var (void** ptr).
		/// In this function, you may want to reserve enough space to load what's in the save using the uint64_t.
		/// And you also want to set the ptr to the data to the data's new location (*ptr = new_location).
		/// For example, with std::vector vec, you want to do {[](uint64_t size, void** val_ptr) {vec.reserve(size); *val_ptr = &vec[0];}}</param>
		SaveVar(uint64_t id_p, void* val_p, uint64_t unit_size_p,
			std::function<uint64_t(void** val_ptr)> get_size_p,
			std::function<void(uint64_t, void** val_ptr)> reserve_p);

		/// @brief This constructor is for stack allocated variables (static size and location) only.
		/// <param name="id_p">A unique id to identify this variable in a save.</param>
		/// <param name="val_p">The pointer to the data itself.</param>
		/// <param name="size_p">The total size of the data.</param>
		/// <param name="unit_size_p">The size (in bytes) of one element of the data.
		/// For example, if the data is an array of int32_t or just one single int32_t, unit_size_p has to be 4</param>
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
