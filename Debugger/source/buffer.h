#pragma once

class c_buffer
{
public:
	c_buffer();
	c_buffer(size_t size);
	~c_buffer();

	void create(size_t size);
	void destroy();

	void expand(size_t size);
	void compact(size_t size);

	const size_t size()
	{
		return m_size;
	}

	template<typename t_type = unsigned char>
	const t_type* data()
	{
		return (const t_type*)m_data;
	}

	unsigned char* operator[](size_t offset)
	{
		if (!m_data)
			throw;

		return m_data + offset;
	}

	void operator=(c_buffer& from)
	{
		if (!from.data() || !from.size())
			return;

		if (!m_data)
			create(from.size());

		::memcpy(m_data, from.data(), from.size());
	}

private:
	size_t m_size;
	unsigned char* m_data;
};
