#include <main.h>

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
