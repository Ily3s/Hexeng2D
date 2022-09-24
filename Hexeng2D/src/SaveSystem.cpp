#include "SaveSystem.hpp"

namespace Hexeng
{

	void SaveFile::add_var(const SaveVar& var)
	{
		m_vars.push_back(var);
		m_var_map.insert({ var.id, var });
	}

	void SaveFile::save(const std::string& filepath)
	{
		uint64_t header_size = 16 * m_vars.size() + 8;

		uint64_t total_size = header_size;

		void* garbage = nullptr;
		for (auto& var : m_vars)
			total_size += var.get_size(&garbage);

		uint8_t* save_buffer = new uint8_t[total_size];
		uint64_t* header_buffer = (uint64_t*)save_buffer;
		save_buffer += header_size;

		*header_buffer = header_size;

		for (auto& var : m_vars)
		{
			header_buffer++;
			*(header_buffer) = var.id;
			header_buffer++;
			*(header_buffer) = var.get_size(&var.value);
			memcpy(save_buffer, var.value, *header_buffer);
			save_buffer += *header_buffer;
		}

		save_buffer -= total_size;

		m_file.open(filepath, std::ios::binary | std::ios::out);
		m_file.write((const char*)save_buffer, total_size);
		m_file.close();

		delete[] save_buffer;
	}

	void SaveFile::load(const std::string& filepath)
	{
		m_file.open(filepath, std::ios::binary | std::ios::in);

		m_file.seekg(0, std::ios::end);
		size_t total_size = m_file.tellg();
		m_file.seekg(0, std::ios::beg);

		uint8_t* save_buffer = new uint8_t[total_size];
		uint64_t* header = (uint64_t*)save_buffer;

		m_file.read((char*)save_buffer, total_size);

		m_file.close();

		uint64_t header_size = *header;

		uint8_t* save_buffer_safe = save_buffer + header_size;

		for (size_t i = 1; i < (header_size-1)/8; i+=2)
		{
			auto& var = m_var_map[header[i]];
			var.reserve(header[i + 1], &var.value);
			memcpy(var.value, save_buffer_safe, header[i + 1]);
			save_buffer_safe += header[i + 1];
		}

		delete[] save_buffer;
	}

}