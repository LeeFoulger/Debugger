#include <main.h>

#include <stdio.h>
#include <stdlib.h>

c_buffer::c_buffer() :
	m_size(0),
	m_data(nullptr)
{
}

c_buffer::c_buffer(size_t size) :
	m_size(0),
	m_data(nullptr)
{
	create(size);
}

c_buffer::~c_buffer()
{
	destroy();
}

void c_buffer::create(size_t size)
{
	destroy();

	m_size += size;
	m_data = new unsigned char[m_size];
	memset(m_data, 0, m_size);
}

void c_buffer::destroy()
{
	if (m_data)
		delete[] m_data;

	m_size = 0;
	m_data = nullptr;
}

void c_buffer::expand(size_t size)
{
	unsigned char* buffer = new unsigned char[m_size + size];
	memset(buffer, 0, m_size + size);

	if (m_data)
	{
		memcpy(buffer, m_data, m_size);
		delete[] m_data;
	}

	m_size += size;
	m_data = buffer;
}

void c_buffer::compact(size_t size)
{
	if (size > m_size)
		return;

	unsigned char* buffer = new unsigned char[m_size - size];
	memset(buffer, 0, m_size - size);

	if (m_data)
	{
		memcpy(buffer, m_data, m_size - size);
		delete[] m_data;
	}

	m_size -= size;
	m_data = buffer;
}

void c_buffer::read_from_file(const wchar_t* filename)
{
	FILE* file;
	_wfopen_s(&file, filename, L"rb");
	if (file)
	{
		_fseeki64(file, 0, SEEK_END);
		create(ftell(file));
		_fseeki64(file, 0, SEEK_SET);

		fread(m_data, sizeof(unsigned char), m_size, file);
		fflush(file);
		fclose(file);
	}
}

void c_buffer::read_from_file(const char* filename)
{
	size_t temp;
	wchar_t filename_wide[MAX_PATH]{};
	mbstowcs_s(&temp, filename_wide, MAX_PATH, filename, MAX_PATH);

	read_from_file(filename_wide);
}

