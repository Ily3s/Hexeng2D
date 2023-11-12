#include <string.h>
#include <exception>
#include <memory>

#include "SaveSystem.hpp"

namespace Hexeng
{

	SaveVar::SaveVar(uint64_t id_p, void* val_p, uint64_t unit_size_p,
		std::function<uint64_t(void** val_ptr)> get_size_p,
		std::function<void(uint64_t, void** val_ptr)> reserve_p)
		: id(id_p),
		value(val_p),
		get_size(get_size_p),
		reserve(reserve_p),
		unit_size(unit_size_p) {}

	SaveVar::SaveVar(uint64_t id_p, void* val_p, uint64_t size_p, uint64_t unit_size_p)
		: id(id_p),
		value(val_p),
		get_size([size_p](void**) {return size_p; }),
		reserve([](uint64_t, void**) {}),
		unit_size(unit_size_p) {}

	void SaveFile::add_var(const SaveVar& var)
	{
		m_vars.push_back(var);
		m_var_map.insert({ var.id, var });
	}

	static char32_t native_bom = U'\U0000FEFF';;

	void SaveFile::save(const std::string& filepath)
	{
		try
		{
			uint64_t total_size = 0;

			int m_vars_size = 0;
			void* garbage = nullptr;
			for (auto& var : m_vars)
			{
				uint64_t var_size = var.get_size(&garbage);
				m_vars_size += var_size ? 1 : 0;
				total_size += var_size;
			}

			if (!m_vars_size)
				return;

			uint64_t header_size = 24 * m_vars_size + 12;

			total_size += header_size;

			uint8_t* save_buffer = new uint8_t[total_size];
			uint64_t* header_buffer = (uint64_t*)(save_buffer + 4);

			*(char32_t*)save_buffer = native_bom;

			save_buffer += header_size;
			*header_buffer = header_size;

			for (auto& var : m_vars)
			{
				if (!var.get_size(&garbage))
					continue;

				header_buffer++;
				*(header_buffer) = var.id;
				header_buffer++;
				*(header_buffer) = var.get_size(&var.value);
				memcpy(save_buffer, var.value, *header_buffer);
				save_buffer += *header_buffer;
				header_buffer++;
				*(header_buffer) = var.unit_size;
			}

			save_buffer -= total_size;

			try
			{
				m_file.open(filepath, std::ios::binary | std::ios::out);
				m_file.write((const char*)save_buffer, total_size);
				m_file.close();
			}
			catch (const std::fstream::failure& e)
			{
				HXG_LOG_ERROR("Cannot save at location \"" + filepath + "\" : " + e.what());
			}

			delete[] save_buffer;

		}
		catch (const std::exception& e)
		{
			HXG_LOG_ERROR("Unnexpected exception saving at location \"" + filepath + "\" : " + e.what());
		}
	}

	static void swap_endianness(const int8_t* input, int8_t* output, size_t size)
	{
		for (size_t i = 0; i < size; i++)
			output[i] = input[size - i - 1];
	}

	void SaveFile::load(const std::string& filepath)
	{
		m_file.open(filepath, std::ios::binary | std::ios::in);
		if (!m_file)
			throw std::runtime_error("The file \"" + filepath + "\" may not exist.");

		try
		{
			m_file.seekg(0, std::ios::end);
			size_t total_size = m_file.tellg();
			m_file.seekg(0, std::ios::beg);

			int8_t* save_buffer = new int8_t[total_size];

			m_file.read((char*)save_buffer, total_size);

			m_file.close();


			if (*(char32_t*)save_buffer != native_bom)
			{
				int8_t* endian_correct = new int8_t[total_size];

				swap_endianness(save_buffer + 4, endian_correct + 4, 8);
				uint64_t header_size = *(uint64_t*)(endian_correct + 4);

				for (uint64_t i = 12; i < header_size; i += 24)
				{
					swap_endianness(save_buffer + i, endian_correct + i, 8);
					swap_endianness(save_buffer + i + 8, endian_correct + i + 8, 8);
					swap_endianness(save_buffer + i + 16, endian_correct + i + 16, 8);
				}

				uint64_t* header = (uint64_t*)(endian_correct + 12);
				uint64_t pos = header_size;

				for (uint64_t i = 0; i < (header_size - 12) / 8; i += 3)
				{
					for (uint64_t ele_offset = 0; ele_offset < header[i + 1]; ele_offset += header[i + 2])
						swap_endianness(save_buffer + pos + ele_offset, endian_correct + pos + ele_offset, header[i + 2]);
					pos += header[i + 1];
				}

				delete[] save_buffer;
				save_buffer = endian_correct;
			}

			uint64_t* header = (uint64_t*)(save_buffer + 4);

			uint64_t header_size = *header;

			int8_t* save_buffer_safe = save_buffer + header_size;

			for (size_t i = 1; i < (header_size - 4) / 8; i += 3)
			{
				auto& var = m_var_map[header[i]];
				var.reserve(header[i + 1], &var.value);
				try
				{
					memcpy(var.value, save_buffer_safe, header[i + 1]);
				}
				catch (const std::bad_alloc& e)
				{
					HXG_LOG_ERROR(std::string{ "Cannot set a var to its save : " } + e.what());
				}
				save_buffer_safe += header[i + 1];
			}

			delete[] save_buffer;

		}
		catch (const std::exception& e)
		{
			HXG_LOG_ERROR("Unexpected exception trying to load a save at location \"" + filepath + "\" : " + e.what());
		}
	}

}
